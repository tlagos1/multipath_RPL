/*
 * Copyright (c) 2018, Remous-Aris Koutsiamanis.
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
 *         Simple implementation of the parent selection strategy in Leapfrog collaboration
 * \author
 *         Remous-Aris Koutsiamanis <aris@ariskou.com>
 */

#include <string.h>

#include "contiki.h"
#include "net/ip/uip.h"
#include "net/ip/uip-debug.h"
#include "net/ipv6/uip-ds6.h"

#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include <stdlib.h>


/*---------------------------------------------------------------------------*/
static rpl_instance_t *temp_instance;
/*---------------------------------------------------------------------------*/
static int
leapfrog_parent_selection_parent_comparison_function(const void * a, const void * b)
{
  rpl_parent_t*best = temp_instance->of->best_parent( *(rpl_parent_t**)a, *(rpl_parent_t**)b );
  if(best == a) //a better than b
    return -1;
  else if(best == b)
    return 1;
  else
    return 0;
}
/*---------------------------------------------------------------------------*/
static int
leapfrog_parent_selection_sort_and_filter_parents(rpl_instance_t *instance, rpl_parent_t* out_valid_parents[], uint8_t in_valid_parents_size, uint8_t* out_valid_parents_count)
{
  PRINTF("leapfrog-parent-selection: sort_and_filter_parents\n");
  if(instance != NULL){
    int fresh_only = 0;
    rpl_dag_t *dag = instance->current_dag;
    rpl_parent_t *p;

    if(dag == NULL || dag->instance == NULL || dag->instance->of == NULL || instance->of == NULL) {
      PRINTF("leapfrog-parent-selection: sort_and_filter_parents: no dag & dag instance & dag instance objective function\n");
      return -1;
    }

    rpl_parent_t *current_preferred_parent = dag->preferred_parent;

    PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Current node has rank %d\n", dag->rank);
    rpl_print_neighbor_list();
    PRINTF("ds6_neighbors:\n");
    uip_ds6_nbr_t *nbr = nbr_table_head(ds6_neighbors);
    while(nbr != NULL){
      PRINTF("ds6_neighbor ");
      PRINT6ADDR(&nbr->ipaddr);
      PRINTF(" isrouter %i state: %i\n", nbr->isrouter, nbr->state);
      nbr = nbr_table_next(ds6_neighbors, nbr);
    }
    PRINTF("- Default routes:\n");
    uip_ds6_defrt_t *r;
    for(r = uip_ds6_defrt_head(); r != NULL; r = list_item_next(r)) {
      PRINTF("Defrt via IP address ");
      PRINT6ADDR(&r->ipaddr);
      uip_ds6_nbr_t *bestnbr = uip_ds6_nbr_lookup(&r->ipaddr);
      PRINTF(" lifetime +%lu*%lu sec infinite %i nbr.state %i\n",
             (unsigned long)r->lifetime.start,
             (unsigned long)r->lifetime.interval,
             r->isinfinite,
             bestnbr->state
             );
    }
    /* Search for the best parent according to the OF */
    for(p = nbr_table_head(rpl_parents); p != NULL; p = nbr_table_next(rpl_parents, p)) {

      const linkaddr_t *p_addr = rpl_get_parent_lladdr(p);
      PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has rank %d\n", p_addr->u8[LINKADDR_SIZE-1], p->rank);

      /* Exclude parents from other DAGs or announcing an infinite rank */
      if(p->dag != dag || p->rank == INFINITE_RANK || p->rank < ROOT_RANK(dag->instance) ||
          dag->instance->of->best_parent( p, p ) == NULL
          )
      {
        if(p->dag != dag)
          PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has p->dag != dag\n", p_addr->u8[LINKADDR_SIZE-1]);
        if(p->rank == INFINITE_RANK)
          PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has p->rank == INFINITE_RANK\n", p_addr->u8[LINKADDR_SIZE-1]);
        if(p->rank < ROOT_RANK(dag->instance))
          PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has p->rank < ROOT_RANK(dag->instance)\n", p_addr->u8[LINKADDR_SIZE-1]);
        if(dag->instance->of->best_parent( p, p ) == NULL)
          PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has best_parent( p, p ) == NULL\n", p_addr->u8[LINKADDR_SIZE-1]);
        if(p->rank < ROOT_RANK(dag->instance)) {
          PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has invalid rank %d\n", p_addr->u8[LINKADDR_SIZE-1], p->rank);
        }
        continue;
      }

      if(fresh_only && !rpl_parent_is_fresh(p)) {
        PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Parent %d has !rpl_parent_is_fresh(p)\n", p_addr->u8[LINKADDR_SIZE-1]);
        /* Filter out non-fresh parents if fresh_only is set */
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

      /* Now we have an acceptable parent, add to array */
      out_valid_parents[*out_valid_parents_count] = p;
      *out_valid_parents_count = *out_valid_parents_count + 1;
      if(*out_valid_parents_count >= in_valid_parents_size)
        break; //no more space. Stop here

    }

    temp_instance = instance;
    // Sort all the parents based on the objective function
    qsort(out_valid_parents, *out_valid_parents_count, sizeof(rpl_parent_t*), leapfrog_parent_selection_parent_comparison_function);

    PRINTF("leapfrog-parent-selection: sort_and_filter_parents: OK: parents count: %d\n", *out_valid_parents_count);
    return 0;
  }

  PRINTF("leapfrog-parent-selection: sort_and_filter_parents: Error: no default RPL instance\n");
  return -2;
}

void
leapfrog_parent_selection_select_replication_targets_local_addresses(uip_ipaddr_t*out_addresses, int in_addresses_count, int* out_addresses_count)
{
  *out_addresses_count = 0;
  printf("leapfrog-parent-selection: select_replication_targets_local_addresses: 1: %i\n", *out_addresses_count);

  rpl_instance_t * default_instance = rpl_get_default_instance();
  if(default_instance == NULL){
    return;
  }
  rpl_dag_t *dag = default_instance->current_dag;
  if(dag == NULL){
    return;
  }

  uip_ipaddr_t* default_parent_ip = uip_ds6_defrt_choose(); /* get the ip address of the (first) default route */

  if(default_parent_ip != NULL && in_addresses_count >= 1) { /* if it exists and there is enough space */
    uip_ipaddr_copy(&out_addresses[0], default_parent_ip); /* copy the found default into the target array, in the first element */
    *out_addresses_count = (*out_addresses_count) + 1;
    printf("leapfrog-parent-selection: select_replication_targets_local_addresses: 2: %i\n", *out_addresses_count);

    rpl_parent_t* valid_parents[NBR_TABLE_MAX_NEIGHBORS];
    uint8_t valid_parents_count = 0;
    if(leapfrog_parent_selection_sort_and_filter_parents(default_instance, valid_parents, NBR_TABLE_MAX_NEIGHBORS, &valid_parents_count) == 0) {
      uint8_t valid_parent_index;
      for(valid_parent_index = 0; (valid_parent_index < valid_parents_count) && ((*out_addresses_count) < in_addresses_count); valid_parent_index++) {
        uip_ipaddr_t* parent_ip = rpl_get_parent_ipaddr(valid_parents[valid_parent_index]);
        if(uip_ipaddr_cmp(default_parent_ip, parent_ip) == 0) { // if this is not the default ip, add to array
          uip_ipaddr_copy(&out_addresses[*out_addresses_count], parent_ip);
          *out_addresses_count = (*out_addresses_count) + 1;
          printf("leapfrog-parent-selection: select_replication_targets_local_addresses: 3: %i\n", *out_addresses_count);
        }
      }
    }
  }
}

/** @}*/

