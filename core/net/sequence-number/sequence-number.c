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
#include "net/sequence-number/sequence-number.h"


#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

#include <limits.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Store here the next available sequence number.
 * This value is used when a new packet is sent and a new sequence number needs to be created for it to support PRE. */
uint32_t next_sequence_number;
/*---------------------------------------------------------------------------*/
/* Sequence number option in Hop-By-Hop header */
typedef struct uip_ext_hdr_opt_sequence_number {
  uint8_t opt_type;
  uint8_t opt_len;
  uint16_t flag_ap; /* used for padding and alignment */
  uint32_t sequence_number;
} uip_ext_hdr_opt_sequence_number;
/*---------------------------------------------------------------------------*/
#define UIP_EXT_HDR_OPT_SEQUENCE_NUMBER_DATA_LEN    (2 + 4) /* 2 (unused) + 4 (sequence_number) */
/*---------------------------------------------------------------------------*/
void
sequence_number_init()
{
  next_sequence_number = 1;
}
/*---------------------------------------------------------------------------*/
static uint32_t
sequence_number_next()
{
  return next_sequence_number++;
}
/*---------------------------------------------------------------------------*/
static uint32_t
sequence_number_set_from_buffer_to_packet_attributes(uip_ext_hdr_opt_sequence_number * hdr_sequence_number)
{
  printf("sequence_number_set_from_buffer_to_packet_attributes\n");
  uint32_t seq_num = uip_ntohl(hdr_sequence_number->sequence_number);
  uint16_t flag = hdr_sequence_number->flag_ap;
#if WITH_PACKET_REPLICATION_AND_ELIMINATION
  packetbuf_set_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_AP_LOW, flag & 0x00ff);
  packetbuf_set_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_AP_HIGH, (flag & 0xff00) >> 8);

  packetbuf_set_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_LOW, seq_num & 0x0000ffff);
  packetbuf_set_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_HIGH, (seq_num & 0xffff0000) >> 16);
#endif
  return seq_num;
}
/*---------------------------------------------------------------------------*/
void
sequence_number_header_output_ap_used(void* sequence_number_buf, uint32_t seq_num){
	  uip_ext_hdr_opt_sequence_number * hdr_sequence_number = (uip_ext_hdr_opt_sequence_number *)sequence_number_buf;
	  hdr_sequence_number->opt_type = UIP_EXT_HDR_OPT_SEQUENCE_NUMBER;
	  hdr_sequence_number->opt_len = UIP_EXT_HDR_OPT_SEQUENCE_NUMBER_DATA_LEN;
	  hdr_sequence_number->flag_ap = 0;
	  hdr_sequence_number->sequence_number = uip_htonl(seq_num);
	  sequence_number_set_from_buffer_to_packet_attributes(hdr_sequence_number);
}
/*---------------------------------------------------------------------------*/
void
sequence_number_header_output(void* sequence_number_buf)
{
  /* Add Sequence number option in RPL option */
  uip_ext_hdr_opt_sequence_number * hdr_sequence_number = (uip_ext_hdr_opt_sequence_number *)sequence_number_buf;
  hdr_sequence_number->opt_type = UIP_EXT_HDR_OPT_SEQUENCE_NUMBER;
  hdr_sequence_number->opt_len = UIP_EXT_HDR_OPT_SEQUENCE_NUMBER_DATA_LEN;
  hdr_sequence_number->flag_ap = 1;
  uint32_t seq_num = sequence_number_next();
  hdr_sequence_number->sequence_number = uip_htonl(seq_num);
  sequence_number_set_from_buffer_to_packet_attributes(hdr_sequence_number);
}
/*---------------------------------------------------------------------------*/
uint32_t
sequence_number_header_input(void* sequence_number_buf)
{
  uip_ext_hdr_opt_sequence_number * hdr_sequence_number = (uip_ext_hdr_opt_sequence_number *)sequence_number_buf;
  if(hdr_sequence_number->opt_type != UIP_EXT_HDR_OPT_SEQUENCE_NUMBER){
    PRINTF("SeqNum: opt type %d is not valid\n", hdr_sequence_number->opt_type);
    return 0;
  }

  if(hdr_sequence_number->opt_len != UIP_EXT_HDR_OPT_SEQUENCE_NUMBER_DATA_LEN){
    PRINTF("SeqNum: opt len %d is not valid\n", hdr_sequence_number->opt_len);
    return 0;
  }

//  if(hdr_sequence_number->flag_ap != 0){
//    PRINTF("SeqNum: unused %d is not valid\n", hdr_sequence_number->unused);
//    return 0;
//  }
  sequence_number_set_from_buffer_to_packet_attributes(hdr_sequence_number);
  if(sequence_number_get_number_from_packet_attributes() == 0){
    PRINTF("SeqNum: sequence number %d is not valid\n", sequence_number_get_number_from_packet_attributes());
    return 0;
  }
  PRINTF("SeqNum: sequence number 0x%04x\n", sequence_number_get_number_from_packet_attributes());
  return sequence_number_get_number_from_packet_attributes();
}
/*---------------------------------------------------------------------------*/
uint16_t
sequence_number_get_number_from_packet_attributes_ap(){
#if WITH_PACKET_REPLICATION_AND_ELIMINATION
	printf("ap_sequence_number\n");
  return packetbuf_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_AP_LOW) +
		  (packetbuf_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_AP_HIGH) << 8);
#else
  return 0;
#endif
}
/*---------------------------------------------------------------------------*/
uint32_t
sequence_number_get_number_from_packet_attributes()
{
#if WITH_PACKET_REPLICATION_AND_ELIMINATION
  return packetbuf_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_LOW) +
         (packetbuf_attr(PACKETBUF_ATTR_SEQUENCE_NUMBER_HIGH) << 16);
#else
  return 0;
#endif
}
/*---------------------------------------------------------------------------*/
uint32_t
sequence_number_get_number_from_queuebuf_attributes(struct queuebuf *qb)
{
#if WITH_PACKET_REPLICATION_AND_ELIMINATION
  return queuebuf_attr(qb, PACKETBUF_ATTR_SEQUENCE_NUMBER_LOW) +
         (queuebuf_attr(qb, PACKETBUF_ATTR_SEQUENCE_NUMBER_HIGH) << 16);
#else
  return 0;
#endif
}

/** @}*/
