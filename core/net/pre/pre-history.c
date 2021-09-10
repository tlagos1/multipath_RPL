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
#include "net/ipv6/uip-ds6.h"
#include "net/packetbuf.h"
#include "net/queuebuf.h"
#include "net/sequence-number/sequence-number.h"
#include "lib/memb.h"
#include "lib/list.h"
#include "net/pre/pre-conf.h"

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

#include <limits.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Structure for recording seen packets */
typedef struct unique_packet {
  struct unique_packet *next;
  uip_ipaddr_t src;
  uip_ipaddr_t dst;
  uint16_t src_port;/* The source port number in host byte order. */
  uint16_t dst_port;/* The destination port number in host byte order. */
  uint32_t sequence_number;
} unique_packet;
/*---------------------------------------------------------------------------*/
/* Memory block for storage of mutiple unique packets seen */
MEMB(pre_history_unique_packets, unique_packet, PRE_UNIQUE_PACKET_HISTORY_SIZE);
/*---------------------------------------------------------------------------*/
/* Linked list storing pointers to unique_packets in the order they are encountered */
LIST(pre_history_unique_packets_list);
/*---------------------------------------------------------------------------*/
/* Copied from net/ipv6/uip6.c in order to be able to access the IPv6 and UDP packet headers */
#define UIP_IP_BUF  ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
/*---------------------------------------------------------------------------*/
static void
pre_history_print_unique_packet(unique_packet * current_packet){
  PRINTF("next %i, src ", current_packet->next);
  PRINT6ADDR(&current_packet->src);
  PRINTF(" dst ");
  PRINT6ADDR(&current_packet->dst);
  PRINTF(" src port %i dst port %i SeqNum %u", current_packet->src_port, current_packet->dst_port, current_packet->sequence_number);
}
/*---------------------------------------------------------------------------*/
static void
pre_history_populate_unique_packet_from_packetbuf(unique_packet * current_packet){
  current_packet->next = 0;
  current_packet->src = UIP_IP_BUF->srcipaddr;
  current_packet->dst = UIP_IP_BUF->destipaddr;
#if WITH_PACKET_REPLICATION_AND_ELIMINATION
  current_packet->src_port = packetbuf_attr(PACKETBUF_ATTR_UDP_SRC_PORT);;
  current_packet->dst_port = packetbuf_attr(PACKETBUF_ATTR_UDP_DEST_PORT);;
#endif
  current_packet->sequence_number = sequence_number_get_number_from_packet_attributes();
  printf("pre_history_populate_unique_packet_from_packetbuf %i\n",current_packet->sequence_number);
}
/*---------------------------------------------------------------------------*/
static int
pre_history_compare_unique_packets(unique_packet* p1, unique_packet* p2){
  return
      uip_ipaddr_cmp(&p1->src, &p2->src) &&
      uip_ipaddr_cmp(&p1->dst, &p2->dst) &&
      (p1->src_port == p2->src_port) &&
      (p1->dst_port == p2->dst_port) &&
      (p1->sequence_number == p2->sequence_number);
}
/*---------------------------------------------------------------------------*/
int
pre_history_is_in_history(){
  unique_packet current_packet;
  pre_history_populate_unique_packet_from_packetbuf(&current_packet);
  PRINTF("PRE: pre_history_is_in_history: current_packet: ");
  pre_history_print_unique_packet(&current_packet);
  PRINTF("\n");

  unique_packet* seen_packet = list_head(pre_history_unique_packets_list);
  while(seen_packet != NULL){
    PRINTF("PRE: pre_history_is_in_history: seen_packet: ");
    pre_history_print_unique_packet(seen_packet);
    PRINTF("\n");
    if(pre_history_compare_unique_packets(seen_packet, &current_packet)){
      PRINTF("PRE: Already in history\n");
      return 1;
    }else
      seen_packet = list_item_next(seen_packet);
  }
  PRINTF("PRE: Not already in history (new packet)\n");
  return 0;
}
/*---------------------------------------------------------------------------*//*---------------------------------------------------------------------------*/
int
pre_history_validate_and_update_history(){
  unique_packet current_packet;
  pre_history_populate_unique_packet_from_packetbuf(&current_packet);
  PRINTF("PRE: pre_history_validate_and_update_history: current_packet: ");
  pre_history_print_unique_packet(&current_packet);
  PRINTF("\n");

  unique_packet* seen_packet = list_head(pre_history_unique_packets_list);
  while(seen_packet != NULL){
    PRINTF("PRE: pre_history_validate_and_update_history: seen_packet: ");
    pre_history_print_unique_packet(seen_packet);
    PRINTF("\n");
    if(pre_history_compare_unique_packets(seen_packet, &current_packet)){
      PRINTF("PRE: Identified a duplicate packet (already in history). Dropping.\n");
      return 0;
    }else
      seen_packet = list_item_next(seen_packet);
  }

  /* The current packet is not in the history. Keep and add to history */
  unique_packet* new_seen_packet = memb_alloc(&pre_history_unique_packets);
  if(new_seen_packet == NULL){
    PRINTF("SeqNum: History memory size exhausted. Freeing oldest seen packet.\n");
    unique_packet* oldest_seen_packet = list_chop(pre_history_unique_packets_list);
    if(oldest_seen_packet == NULL){
      printf("PRE: Error: could not remove the oldest seen packet!\n");
      return 0;
    }
    memb_free(&pre_history_unique_packets, oldest_seen_packet);
    new_seen_packet = memb_alloc(&pre_history_unique_packets);
    if(new_seen_packet == NULL){
      printf("PRE: Error: reallocation attempt after freeing oldest packet failed!\n");
      return 0;
    }
  }
  memcpy(new_seen_packet, &current_packet, sizeof(unique_packet));
  list_push(pre_history_unique_packets_list, new_seen_packet);
  PRINTF("PRE: Identified a new packet. Adding to history and keeping it.\n");
  return 1;
}
/*---------------------------------------------------------------------------*/
void
pre_history_init()
{
  memb_init(&pre_history_unique_packets);
  list_init(pre_history_unique_packets_list);
}
/*---------------------------------------------------------------------------*/


