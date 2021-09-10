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
#include "net/saved-ext-pp-ap/saved-ext-pp-ap.h"


#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#include <limits.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Store here the next available sequence number.
 * This value is used when a new packet is sent and a new sequence number needs to be created for it to support PRE. */

/*---------------------------------------------------------------------------*/
/* Sequence number option in Hop-By-Hop header */

/*---------------------------------------------------------------------------*/
#define UIP_EXT_HDR_OPT_SAVED_PP_AP_DATA_LEN    (2 + 4) /* 2 (unused) + 4 (sequence_number) */
/*---------------------------------------------------------------------------*/

typedef struct uip_ext_hdr_opt_saved_pp_aa {
  uint8_t opt_type;
  uint8_t opt_len;
  uint16_t pp_ap; /* used for padding and alignment */
  uint8_t ap_mac_address[2];
  uint8_t pp_mac_address[2];
} uip_ext_hdr_opt_saved_pp_aa;

/*---------------------------------------------------------------------------*/
void
saved_ext_pp_ap_set_from_buffer_to_packet_attributes(uip_ext_hdr_opt_saved_pp_aa * hdr_saved_pp_ap)
{
  printf("saved_ext_pp_ap_set_from_buffer_to_packet_attributes\n");
//  packetbuf_set_attr(PACKETBUF_ATTR_PP_AP, << 4 | hdr_saved_pp_ap->mac_address );
}
/*---------------------------------------------------------------------------*/
void
saved_ext_pp_ap_header_output(void* saved_pp_ap_buf)
{
	rpl_instance_t *instance = rpl_get_default_instance();
	rpl_dag_t *dag;
	dag = instance->current_dag;
	if(dag == NULL){
		return;
	}
#if ADAPTIVE_CA
	rpl_parent_t* preferred_parent = dag->aux_preferred_parent;
#else
	rpl_parent_t* preferred_parent = dag->preferred_parent;
#endif

  /* Add Sequence number option in RPL option */
	uip_ext_hdr_opt_saved_pp_aa * hdr_saved_pp_ap = (uip_ext_hdr_opt_saved_pp_aa *)saved_pp_ap_buf;
	hdr_saved_pp_ap->opt_type = UIP_EXT_HDR_OPT_SAVED_PP_AP;
	hdr_saved_pp_ap->opt_len = UIP_EXT_HDR_OPT_SAVED_PP_AP_DATA_LEN;
	hdr_saved_pp_ap->pp_ap = 0;

	if(preferred_parent != NULL){
		uip_ipaddr_t* p_parent_addr = rpl_get_parent_ipaddr(preferred_parent);
		printf("saved_ext_pp_ap_header_output %x - %x \n",p_parent_addr->u8[14],p_parent_addr->u8[15]);
		hdr_saved_pp_ap->pp_mac_address[0] = (p_parent_addr->u8[14]);
		hdr_saved_pp_ap->pp_mac_address[1] = (p_parent_addr->u8[15]);
#if WITH_ALTERNATIVE_PARENT_SELECTION
		if(dag->alternative_parent){
			hdr_saved_pp_ap->pp_ap = 1;
			uip_ipaddr_t* a_parent_addr = rpl_get_parent_ipaddr(dag->alternative_parent);
            printf("saved_ext_pp_ap_header_output %x - %x \n",a_parent_addr->u8[14],a_parent_addr->u8[15]);
			hdr_saved_pp_ap->ap_mac_address[0] = a_parent_addr->u8[14];
			hdr_saved_pp_ap->ap_mac_address[1] = a_parent_addr->u8[15];
		}
#endif
	}
	//saved_ext_pp_ap_set_from_buffer_to_packet_attributes(hdr_saved_pp_ap);
}
/*---------------------------------------------------------------------------*/

int
saved_ext_pp_ap_header_validation(void* saved_pp_ap_buf){
	int i;
	uint8_t state;
	uip_ext_hdr_opt_saved_pp_aa * hdr_saved_pp_ap = (uip_ext_hdr_opt_saved_pp_aa *)saved_pp_ap_buf;
	for (i = 0; i < UIP_DS6_ADDR_NB; i++){
		state = uip_ds6_if.addr_list[i].state;

		if (uip_ds6_if.addr_list[i].isused && (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
			if(hdr_saved_pp_ap->pp_ap == 1){
				printf("saved_ext_pp_ap_header_validation: preferred mac %x - %x \n",hdr_saved_pp_ap->pp_mac_address[0] ,hdr_saved_pp_ap->pp_mac_address[1]);
				printf("saved_ext_pp_ap_header_validation: alternative mac %x - %x \n",hdr_saved_pp_ap->ap_mac_address[0] ,hdr_saved_pp_ap->ap_mac_address[1]);
				if(hdr_saved_pp_ap->pp_mac_address[0] == uip_ds6_if.addr_list[i].ipaddr.u8[14] && hdr_saved_pp_ap->pp_mac_address[1] == uip_ds6_if.addr_list[i].ipaddr.u8[15]){
					return 1;
				}
				else if(hdr_saved_pp_ap->ap_mac_address[0] == uip_ds6_if.addr_list[i].ipaddr.u8[14] && hdr_saved_pp_ap->ap_mac_address[1] == uip_ds6_if.addr_list[i].ipaddr.u8[15]){
					return -1;
				}
			}
			else if(hdr_saved_pp_ap->pp_ap == 0){
				printf("saved_ext_pp_ap_header_validation: preferred mac %x - %x \n",hdr_saved_pp_ap->pp_mac_address[0] ,hdr_saved_pp_ap->pp_mac_address[1]);
				if(hdr_saved_pp_ap->pp_mac_address[0] == uip_ds6_if.addr_list[i].ipaddr.u8[14] && hdr_saved_pp_ap->pp_mac_address[1] == uip_ds6_if.addr_list[i].ipaddr.u8[15]){
					return 1;
				}
			}
		}
	}
	return 0;
}

//uint32_t
//sequence_number_header_input(void* sequence_number_buf)
//{
//  uip_ext_hdr_opt_sequence_number * hdr_sequence_number = (uip_ext_hdr_opt_sequence_number *)sequence_number_buf;
//  if(hdr_sequence_number->opt_type != UIP_EXT_HDR_OPT_SEQUENCE_NUMBER){
//    PRINTF("SeqNum: opt type %d is not valid\n", hdr_sequence_number->opt_type);
//    return 0;
//  }
//
//  if(hdr_sequence_number->opt_len != UIP_EXT_HDR_OPT_SEQUENCE_NUMBER_DATA_LEN){
//    PRINTF("SeqNum: opt len %d is not valid\n", hdr_sequence_number->opt_len);
//    return 0;
//  }
//
//  if(hdr_sequence_number->unused != 0){
//    PRINTF("SeqNum: unused %d is not valid\n", hdr_sequence_number->unused);
//    return 0;
//  }
//  sequence_number_set_from_buffer_to_packet_attributes(hdr_sequence_number);
//  if(sequence_number_get_number_from_packet_attributes() == 0){
//    PRINTF("SeqNum: sequence number %d is not valid\n", sequence_number_get_number_from_packet_attributes());
//    return 0;
//  }
//  PRINTF("SeqNum: sequence number 0x%04x\n", sequence_number_get_number_from_packet_attributes());
//  return sequence_number_get_number_from_packet_attributes();
//}
/*---------------------------------------------------------------------------*/
//uint32_t
//sequence_number_get_number_from_packet_attributes()
//{
//  return packetbuf_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_LOW) +
//         (packetbuf_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_HIGH) << 16);
//}
/*---------------------------------------------------------------------------*/
//uint32_t
//sequence_number_get_number_from_queuebuf_attributes(struct queuebuf *qb)
//{
//  return queuebuf_attr(qb, PACKETBUF_ATTR_SEQUENCE_NUMBER_LOW) +
//         (queuebuf_attr(qb, PACKETBUF_ATTR_SEQUENCE_NUMBER_HIGH) << 16);
//}

/** @}*/
