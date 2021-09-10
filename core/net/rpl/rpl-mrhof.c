/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         The Minimum Rank with Hysteresis Objective Function (MRHOF), RFC6719
 *
 *         This implementation uses the estimated number of
 *         transmissions (ETX) as the additive routing metric,
 *         and also provides stubs for the energy metric.
 *
 * \author Joakim Eriksson <joakime@sics.se>, Nicolas Tsiftes <nvt@sics.se>
 */

/**
 * \addtogroup uip6
 * @{
 */

#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include "net/nbr-table.h"
#include "net/link-stats.h"
#include "static-scheduler.h"

#if RPL_DAG_MC_NSA_PS
#include <stdlib.h>
#endif /* RPL_DAG_MC_NSA_PS */

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

/* RFC6551 and RFC6719 do not mandate the use of a specific formula to
 * compute the ETX value. This MRHOF implementation relies on the value
 * computed by the link-stats module. It has an optional feature,
 * RPL_MRHOF_CONF_SQUARED_ETX, that consists in squaring this value.
 * This basically penalizes bad links while preserving the semantics of ETX
 * (1 = perfect link, more = worse link). As a result, MRHOF will favor
 * good links over short paths. Recommended when reliability is a priority.
 * Without this feature, a hop with 50% PRR (ETX=2) is equivalent to two
 * perfect hops with 100% PRR (ETX=1+1=2). With this feature, the former
 * path obtains ETX=2*2=4 and the former ETX=1*1+1*1=2. */
#ifdef RPL_MRHOF_CONF_SQUARED_ETX
#define RPL_MRHOF_SQUARED_ETX RPL_MRHOF_CONF_SQUARED_ETX
#else /* RPL_MRHOF_CONF_SQUARED_ETX */
#define RPL_MRHOF_SQUARED_ETX 0
#endif /* RPL_MRHOF_CONF_SQUARED_ETX */

#if !RPL_MRHOF_SQUARED_ETX
/* Configuration parameters of RFC6719. Reject parents that have a higher
 * link metric than the following. The default value is 512 but we use 1024. */
#define MAX_LINK_METRIC     1024 /* Eq ETX of 8 */
/* Hysteresis of MRHOF: the rank must differ more than PARENT_SWITCH_THRESHOLD_DIV
 * in order to switch preferred parent. Default in RFC6719: 192, eq ETX of 1.5.
 * We use a more aggressive setting: 96, eq ETX of 0.75.
 */
#define PARENT_SWITCH_THRESHOLD 96 /* Eq ETX of 0.75 */
#else /* !RPL_MRHOF_SQUARED_ETX */
#define MAX_LINK_METRIC     2048 /* Eq ETX of 4 */
#define PARENT_SWITCH_THRESHOLD 160 /* Eq ETX of 1.25 (results in a churn comparable
to the threshold of 96 in the non-squared case) */
#endif /* !RPL_MRHOF_SQUARED_ETX */

/* Reject parents that have a higher path cost than the following. */
#define MAX_PATH_COST      32768   /* Eq path ETX of 256 */

/*---------------------------------------------------------------------------*/
static void
reset(rpl_dag_t *dag)
{
    PRINTF("RPL: Reset MRHOF\n");
}
/*---------------------------------------------------------------------------*/
#if RPL_WITH_DAO_ACK
static void
dao_ack_callback(rpl_parent_t *p, int status)
{
  if(status == RPL_DAO_ACK_UNABLE_TO_ADD_ROUTE_AT_ROOT) {
    return;
  }
  /* here we need to handle failed DAO's and other stuff */
  PRINTF("RPL: MRHOF - DAO ACK received with status: %d\n", status);
  if(status >= RPL_DAO_ACK_UNABLE_TO_ACCEPT) {
    /* punish the ETX as if this was 10 packets lost */
    link_stats_packet_sent(rpl_get_parent_lladdr(p), MAC_TX_OK, 10);
  } else if(status == RPL_DAO_ACK_TIMEOUT) { /* timeout = no ack */
    /* punish the total lack of ACK with a similar punishment */
    link_stats_packet_sent(rpl_get_parent_lladdr(p), MAC_TX_OK, 10);
  }
}
#endif /* RPL_WITH_DAO_ACK */
/*---------------------------------------------------------------------------*/
static uint16_t
parent_link_metric(rpl_parent_t *p)
{
    const struct link_stats *stats = rpl_get_parent_link_stats(p);
    if(stats != NULL) {
#if RPL_MRHOF_SQUARED_ETX
        uint32_t squared_etx = ((uint32_t)stats->etx * stats->etx) / LINK_STATS_ETX_DIVISOR;
    return (uint16_t)MIN(squared_etx, 0xffff);
#else /* RPL_MRHOF_SQUARED_ETX */
        return stats->etx;
#endif /* RPL_MRHOF_SQUARED_ETX */
    }
    return 0xffff;
}
/*---------------------------------------------------------------------------*/
static uint16_t
parent_path_cost(rpl_parent_t *p)
{
    uint16_t base;

    if(p == NULL || p->dag == NULL || p->dag->instance == NULL) {
        return 0xffff;
    }

#if RPL_WITH_MC
    /* Handle the different MC types */
    switch(p->dag->instance->mc.type) {
        case RPL_DAG_MC_ETX:
            base = p->mc.obj.etx;
            break;
        case RPL_DAG_MC_ENERGY:
            base = p->mc.obj.energy.energy_est << 8;
            break;
        default:
            base = p->rank;
            break;
    }
#else /* RPL_WITH_MC */
    base = p->rank;
#endif /* RPL_WITH_MC */

    /* path cost upper bound: 0xffff */
    return MIN((uint32_t)base + parent_link_metric(p), 0xffff);
}
/*---------------------------------------------------------------------------*/
static uint16_t
unicast_slot_exists(rpl_parent_t *p)
{
    const linkaddr_t* packet_l2_dest = rpl_get_parent_lladdr(p);
    const linkaddr_t* packet_l2_src = &linkaddr_node_addr;

    return static_scheduler_unicast_slot_exists(packet_l2_src, packet_l2_dest);
}
/*---------------------------------------------------------------------------*/
static rpl_rank_t
rank_via_parent(rpl_parent_t *p)
{
    uint16_t min_hoprankinc;
    uint16_t path_cost;

    if(p == NULL || p->dag == NULL || p->dag->instance == NULL) {
        return INFINITE_RANK;
    }

    min_hoprankinc = p->dag->instance->min_hoprankinc;
    path_cost = parent_path_cost(p);

    // PRINTF("rank_via_parent: rank -> %d \n", MAX(MIN((uint32_t)p->rank + min_hoprankinc, 0xffff), path_cost) );
    /* Rank lower-bound: parent rank + min_hoprankinc */
    return MAX(MIN((uint32_t)p->rank + min_hoprankinc, 0xffff), path_cost);
}
/*---------------------------------------------------------------------------*/
static int
parent_is_acceptable(rpl_parent_t *p)
{
    uint16_t link_metric = parent_link_metric(p);
    uint16_t path_cost = parent_path_cost(p);
    uint16_t unicast_slot = unicast_slot_exists(p);
    /* Exclude links with too high link metrics or path cost (RFC6719, 3.2.2) */
    return link_metric <= MAX_LINK_METRIC && path_cost <= MAX_PATH_COST && unicast_slot;
}
/*---------------------------------------------------------------------------*/
static int
parent_has_usable_link(rpl_parent_t *p)
{
    uint16_t link_metric = parent_link_metric(p);
    /* Exclude links with too high link metrics  */
    return link_metric <= MAX_LINK_METRIC;
}
/*---------------------------------------------------------------------------*/
static rpl_parent_t *
best_parent(rpl_parent_t *p1, rpl_parent_t *p2)
{
    rpl_dag_t *dag;
    uint16_t p1_cost;
    uint16_t p2_cost;
    int p1_is_acceptable;
    int p2_is_acceptable;

    p1_is_acceptable = p1 != NULL && parent_is_acceptable(p1);
    p2_is_acceptable = p2 != NULL && parent_is_acceptable(p2);

    if(!p1_is_acceptable) {
        return p2_is_acceptable ? p2 : NULL;
    }
    if(!p2_is_acceptable) {
        return p1_is_acceptable ? p1 : NULL;
    }

    dag = p1->dag; /* Both parents are in the same DAG. */
    p1_cost = parent_path_cost(p1);
    p2_cost = parent_path_cost(p2);

    /* Maintain stability of the preferred parent in case of similar ranks. */
    if(p1 == dag->preferred_parent || p2 == dag->preferred_parent) {
        if(p1_cost < p2_cost + PARENT_SWITCH_THRESHOLD &&
           p1_cost > p2_cost - PARENT_SWITCH_THRESHOLD) {
            return dag->preferred_parent;
        }
    }

    return p1_cost < p2_cost ? p1 : p2;
}
/*---------------------------------------------------------------------------*/
static rpl_dag_t *
best_dag(rpl_dag_t *d1, rpl_dag_t *d2)
{
    if(d1->grounded != d2->grounded) {
        return d1->grounded ? d1 : d2;
    }

    if(d1->preference != d2->preference) {
        return d1->preference > d2->preference ? d1 : d2;
    }

    return d1->rank < d2->rank ? d1 : d2;
}
/*---------------------------------------------------------------------------*/
#if RPL_DAG_MC_NSA_PS
/*---------------------------------------------------------------------------*/
void sort_neighbors(rpl_dag_t *dag, rpl_parent_t *neighbor_set[], int size, rpl_parent_t *p){

    int parent_sort_index, parent_sort_backwards_index;
    rpl_parent_t *PP;
#if ADAPTIVE_CA
    if(dag->aux_preferred_parent == NULL){
        PP = dag->preferred_parent;
    } else {
        PP = dag->aux_preferred_parent;
    }
#else
    PP = dag->preferred_parent;
#endif
    if(size == 0){
        neighbor_set[size] = p;
        return;
    }
    else{
        if(p == PP){
            for(parent_sort_backwards_index = size; parent_sort_backwards_index > 0; parent_sort_backwards_index--){
                neighbor_set[parent_sort_backwards_index] = neighbor_set[parent_sort_backwards_index-1];
            }
            neighbor_set[0] = p;
            return;
        }
#if WITH_ALTERNATIVE_PARENT_SELECTION
        else if(p == dag->alternative_parent){
            for(parent_sort_backwards_index = size; parent_sort_backwards_index > 1; parent_sort_backwards_index--){
                neighbor_set[parent_sort_backwards_index] = neighbor_set[parent_sort_backwards_index-1];
            }
            neighbor_set[1] = p;
            return;
        }
#endif
        else{
            for(parent_sort_index = 0;parent_sort_index < size; parent_sort_index++){
                if(p->rank < neighbor_set[parent_sort_index]->rank){
                    for(parent_sort_backwards_index = size; parent_sort_backwards_index > parent_sort_index; parent_sort_backwards_index--){
                        neighbor_set[parent_sort_backwards_index] = neighbor_set[parent_sort_backwards_index-1];
                    }
                    neighbor_set[parent_sort_index] = p;
                    return;
                }
            }
            neighbor_set[size] = p;
        }
    }
}
/*---------------------------------------------------------------------------*/
static int
record_alternative_parents_in_mc_nsa(rpl_instance_t *instance)
{

    PRINTF("RPL: MRHOF: record_parents_in_mc_nsa\n");
    if(instance == NULL){
        return -1;
    }

    rpl_dag_t *dag = instance->current_dag;

    if(dag == NULL || dag->instance->of == NULL || dag->preferred_parent == NULL) {
        PRINTF("RPL: MRHOF: record_parents_in_mc_nsa: no dag & dag instance & dag instance objective function\n");
        return -1;
    }

    rpl_parent_t* valid_parents[NBR_TABLE_MAX_NEIGHBORS];
    uint8_t valid_parents_count = 0;

    rpl_parent_t *p;

    /* Search for valid parents */
    for(p = nbr_table_head(rpl_parents); p != NULL; p = nbr_table_next(rpl_parents, p)) {

        /* Exclude parents from other DAGs or announcing an infinite rank */
        if (p->dag != dag || p->rank == INFINITE_RANK || p->rank < ROOT_RANK(dag->instance) ||
            best_parent(p, p) == NULL || dag->rank <= p->rank) {
            continue;
        }


#if UIP_ND6_SEND_NS
        {
        uip_ds6_nbr_t *nbr = rpl_get_nbr(p);
        /* Exclude links to a neighbor that is not reachable at a NUD level */
        if(nbr == NULL || nbr->state != NBR_REACHABLE) {
          continue;
        }
        }
#endif /* UIP_ND6_SEND_NS */

        /* Sort all the parents based on the rank */
        sort_neighbors(dag, valid_parents, valid_parents_count, p);
        valid_parents_count++;
    }

    valid_parents_count = MIN(valid_parents_count, RPL_DAG_MC_NSA_PS_MAX_ADDRESSES);
    instance->mc_constraint.obj.nsa.parent_set.addresses_count = valid_parents_count;

    uint8_t address_index;

    for(address_index = 0; address_index < valid_parents_count; address_index++){
        uip_ipaddr_t* mc_addr = &instance->mc_constraint.obj.nsa.parent_set.addresses[address_index];
        uip_ipaddr_t* parent_addr = rpl_get_parent_ipaddr(valid_parents[address_index]);

        uip_ipaddr_copy(mc_addr, parent_addr);

        if(instance->current_dag->prefix_info.length != 0 && instance->current_dag->prefix_info.flags & UIP_ND6_RA_FLAG_AUTONOMOUS) {
            memcpy(mc_addr, &instance->current_dag->prefix_info.prefix, (instance->current_dag->prefix_info.length + 7) / 8);
        } else{
            uip_ipaddr_t temp_mc_addr;
            uip_ip6addr(&temp_mc_addr, UIP_DS6_DEFAULT_PREFIX, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
            memcpy(mc_addr, &temp_mc_addr, 8);
        }
        PRINTF("RPL: MRHOF: record_parents_in_mc_nsa: copying addr: %i: ", address_index);
        PRINT6ADDR(&instance->mc_constraint.obj.nsa.parent_set.addresses[address_index]);
        PRINTF("\n");
    }
    return 0;
}
#endif /* RPL_DAG_MC_NSA_PS */
/*---------------------------------------------------------------------------*/
#if !RPL_WITH_MC
static void
update_metric_container(rpl_instance_t *instance)
{
  instance->mc.type = RPL_DAG_MC_NONE;
}
#else /* RPL_WITH_MC */
static void
update_metric_container(rpl_instance_t *instance)
{
    rpl_dag_t *dag;
    uint16_t path_cost;
    uint8_t type;

    dag = instance->current_dag;
    if(dag == NULL || !dag->joined) {
        PRINTF("RPL: Cannot update the metric container when not joined\n");
        return;
    }

    if(dag->rank == ROOT_RANK(instance)) {
        /* Configure MC at root only, other nodes are auto-configured when joining */
        instance->mc.type = RPL_DAG_MC;
        instance->mc.flags = 0;
        instance->mc.aggr = RPL_DAG_MC_AGGR_ADDITIVE;
        instance->mc.prec = 0;
#if RPL_DAG_MC_NSA_PS
        instance->mc_constraint.type = RPL_DAG_MC_NSA;
        instance->mc_constraint.flags = (0<<3 /* P */) | (1<<2 /* C */) | (0<<1 /* O */) | (0<<0 /* R */);
        instance->mc_constraint.aggr = RPL_DAG_MC_AGGR_ADDITIVE;
        instance->mc_constraint.prec = 1;
#endif /* RPL_DAG_MC_NSA_PS */
        path_cost = dag->rank;
    } else {
        path_cost = parent_path_cost(dag->preferred_parent);
    }

    /* Handle the different MC types */
    switch(instance->mc.type) {
        case RPL_DAG_MC_NONE:
            break;
        case RPL_DAG_MC_ETX:
            instance->mc.length = sizeof(instance->mc.obj.etx);
            instance->mc.obj.etx = path_cost;
            break;
        case RPL_DAG_MC_ENERGY:
            instance->mc.length = sizeof(instance->mc.obj.energy);
            if(dag->rank == ROOT_RANK(instance)) {
                type = RPL_DAG_MC_ENERGY_TYPE_MAINS;
            } else {
                type = RPL_DAG_MC_ENERGY_TYPE_BATTERY;
            }
            instance->mc.obj.energy.flags = type << RPL_DAG_MC_ENERGY_TYPE;
            /* Energy_est is only one byte, use the least significant byte of the path metric. */
            instance->mc.obj.energy.energy_est = path_cost >> 8;
            break;
        default:
            PRINTF("RPL: MRHOF, non-supported MC %u\n", instance->mc.type);
            break;
    }

#if RPL_DAG_MC_NSA_PS
    /* Handle the different constraint MC types */
    switch(instance->mc_constraint.type) {
        case RPL_DAG_MC_NONE:
            break;
        case RPL_DAG_MC_NSA:
            record_alternative_parents_in_mc_nsa(instance);
            instance->mc_constraint.obj.nsa.reserved = 0;
            instance->mc_constraint.obj.nsa.flags = 0;
            instance->mc_constraint.obj.nsa.parent_set.type = 0x01;
            break;
        default:
            PRINTF("RPL: MRHOF, non-supported constraint MC %u\n", instance->mc.type);
            break;
    }
#endif /* RPL_DAG_MC_NSA_PS */
}
#endif /* RPL_WITH_MC */
/*---------------------------------------------------------------------------*/
rpl_of_t rpl_mrhof = {
        reset,
#if RPL_WITH_DAO_ACK
        dao_ack_callback,
#endif
        parent_link_metric,
        parent_has_usable_link,
        parent_path_cost,
        rank_via_parent,
        best_parent,
        best_dag,
        update_metric_container,
        RPL_OCP_MRHOF
};

/** @}*/
