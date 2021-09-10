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
 *         Manage the sequence number used in PRE.
 * \author
 *         Remous-Aris Koutsiamanis <aris@ariskou.com>
 */

#include "net/ip/uip.h"
#include "net/ip/tcpip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include "net/rpl/rpl-ns.h"
#include "net/packetbuf.h"
#include "net/queuebuf.h"
#include "net/saved-ext-pa/saved-ext-pa.h"


#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#include <limits.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Store here the next available sequence number.
 * This value is used when a new packet is sent and a new sequence number needs to be created for it to support PRE. */

/*---------------------------------------------------------------------------*/
/* Preferred ancestor option in Hop-By-Hop header */

/*---------------------------------------------------------------------------*/
#define UIP_EXT_HDR_OPT_SAVED_PA_DATA_LEN    (2 + 4) /* 2 (unused) + 4 (sequence_number) */
/*---------------------------------------------------------------------------*/

typedef struct uip_ext_hdr_opt_saved_pa {
  uint8_t opt_type;
  uint8_t opt_len;
  uint16_t ap; /* used to know if there is any alternative parent */
  uint8_t pp_address[2];
  uint8_t ap_address[2];
} uip_ext_hdr_opt_saved_pa;
#if ADAPTIVE_CA
/*---------------------------------------------------------------------------*/
void
saved_ext_pa_set_from_buffer_to_packet_attributes(uip_ext_hdr_opt_saved_pa * hdr_saved_pa)
{
  printf("saved_ext_pa_set_from_buffer_to_packet_attributes\n");
//  packetbuf_set_attr(PACKETBUF_ATTR_PP_AP, << 4 | hdr_saved_pp_ap->mac_address );
}
/*---------------------------------------------------------------------------*/
void
saved_ext_pa_header_output(void* saved_pa_buf)
{
	rpl_instance_t *instance = rpl_get_default_instance();
	rpl_dag_t *dag;
	uint8_t addr_offset;
	uint8_t pp_mc_addr_number,ap_mc_addr_number;
	uint8_t pp_mc_addr_number_offset,ap_mc_addr_number_offset;


	uip_ext_hdr_opt_saved_pa * hdr_saved_pa = (uip_ext_hdr_opt_saved_pa *)saved_pa_buf;
	hdr_saved_pa->opt_type = UIP_EXT_HDR_OPT_SAVED_PA;
	hdr_saved_pa->opt_len = UIP_EXT_HDR_OPT_SAVED_PA_DATA_LEN;
	hdr_saved_pa->ap = 0;


	dag = instance->current_dag;
	if(dag == NULL){
		return;
	}
	if(dag->preferred_parent == NULL){
			return;
	}
	if(dag->aux_preferred_parent == NULL){
        rpl_set_aux_preferred_parent(dag, dag->preferred_parent);
	}
	pp_mc_addr_number = dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses_count;
	if(pp_mc_addr_number == 0){
		return;
	}


	PRINTF("saved_ext_pa_header_output: IP ");
	PRINT6ADDR(&dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[0]);
	PRINTF("\n");

	hdr_saved_pa->pp_address[0] = dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[0].u8[13];
	hdr_saved_pa->pp_address[1] = dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[0].u8[15];
	hdr_saved_pa->ap_address[0] = 0;
	hdr_saved_pa->ap_address[1] = 0;

//	if(pp_mc_addr_number > 1){
//		PRINTF("saved_ext_pa_header_output: Alternative IP ");
//		PRINT6ADDR(&dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[1]);
//		PRINTF("\n");
//		hdr_saved_pa->ap = 1;
//		hdr_saved_pa->ap_address[0] = dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[1].u8[13];
//		hdr_saved_pa->ap_address[1] = dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[1].u8[15];
//
//		return;
//	}
//
	if(dag->alternative_parent != NULL){
		ap_mc_addr_number = dag->alternative_parent->mc_constraint.obj.nsa.parent_set.addresses_count;
		for(pp_mc_addr_number_offset = 1; pp_mc_addr_number_offset < pp_mc_addr_number; pp_mc_addr_number_offset++){
			for(ap_mc_addr_number_offset = 0; ap_mc_addr_number_offset < ap_mc_addr_number; ap_mc_addr_number_offset++){
				PRINTF("saved_ext_pa_header_output: preferred AP MC IP ");
				PRINT6ADDR(&dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[pp_mc_addr_number_offset]);
				PRINTF(" saved_ext_pa_header_output: alternative AP MC IP ");
				PRINT6ADDR(&dag->alternative_parent->mc_constraint.obj.nsa.parent_set.addresses[ap_mc_addr_number_offset]);
				PRINTF("\n");
				if(memcmp(&dag->aux_preferred_parent->mc_constraint.obj.nsa.parent_set.addresses[pp_mc_addr_number_offset],&dag->alternative_parent->mc_constraint.obj.nsa.parent_set.addresses[ap_mc_addr_number_offset],16) == 0){

					PRINTF("saved_ext_pa_header_output: IP ");
					PRINT6ADDR(&dag->alternative_parent->mc_constraint.obj.nsa.parent_set.addresses[ap_mc_addr_number_offset]);
					PRINTF("\n");
					hdr_saved_pa->ap = 1;
					hdr_saved_pa->ap_address[0] = dag->alternative_parent->mc_constraint.obj.nsa.parent_set.addresses[ap_mc_addr_number_offset].u8[13];
					hdr_saved_pa->ap_address[1] = dag->alternative_parent->mc_constraint.obj.nsa.parent_set.addresses[ap_mc_addr_number_offset].u8[15];

					return;
				}
			}
		}
	}
}
/*---------------------------------------------------------------------------*/

int
saved_ext_pa_header_validation(void* saved_pa_buf, int routing_pp_ap){
	int i;
	uint8_t state;
	uip_ext_hdr_opt_saved_pa * hdr_saved_pa = (uip_ext_hdr_opt_saved_pa *)saved_pa_buf;
	uip_ipaddr_t preferred_ancestor_addr, possible_alternative_ancestor_addr;
	rpl_instance_t *instance = rpl_get_default_instance();
	rpl_dag_t *dag;
	dag = instance->current_dag;
	if(dag == NULL){
		return 0;
	}
	if(dag->preferred_parent == NULL){
		return 0;
	}

	uip_ip6addr(&preferred_ancestor_addr, 0xfe80,0x0000,0x0000,0x0000,0x0200,0x0000,0x0000,0x0000);
//	uip_ip6addr_u8(&preferred_ancestor_addr, 0xfe,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x02,hdr_saved_pa->pp_address[0],0x00,hdr_saved_pa->pp_address[1],0x00,hdr_saved_pa->pp_address[2],0x00,hdr_saved_pa->pp_address[3]);

	preferred_ancestor_addr.u8[13] = hdr_saved_pa->pp_address[0];
	preferred_ancestor_addr.u8[15] = hdr_saved_pa->pp_address[1];

	PRINTF("saved_ext_pa_header_validation: exchange_parent IP ");
	PRINT6ADDR(&preferred_ancestor_addr);
	PRINTF("\n");

	if(hdr_saved_pa->ap == 1){
		uip_ip6addr(&possible_alternative_ancestor_addr, 0xfe80,0x0000,0x0000,0x0000,0x0200,0x0000,0x0000,0x0000);

		possible_alternative_ancestor_addr.u8[13] = hdr_saved_pa->ap_address[0];
		possible_alternative_ancestor_addr.u8[15] = hdr_saved_pa->ap_address[1];

		PRINTF("saved_ext_pa_header_validation: possible_alternative_ancestor_addr IP ");
		PRINT6ADDR(&possible_alternative_ancestor_addr);
		PRINTF("\n");

		if(triangular_adaption(&preferred_ancestor_addr, &possible_alternative_ancestor_addr,dag) == 1){
				return 1;
		}
	}
	else{
		if(triangular_adaption(&preferred_ancestor_addr, NULL, dag) == 1){
			return 1;
		}
	}
	return 0;
}

#endif
