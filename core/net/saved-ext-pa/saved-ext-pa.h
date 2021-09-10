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
 *         Manage the sequence number.
 * \author
 *         Remous-Aris Koutsiamanis <aris@ariskou.com>
 */

#ifndef SAVED_EXT_PA_H
#define SAVED_EXT_PA_H


#include <stdint.h>

/* Hop-by-Hop extension header option as defined in IANA:
 * https://www.iana.org/assignments/ipv6-parameters/ipv6-parameters.xhtml#ipv6-parameters-2
 *
 *            Binary Value
 * Hex Value  act  chg   rest   Description                   Reference
 * 0x1E        00    0  11110   RFC3692-style Experiment [1]  [RFC4727]
 */
#define UIP_EXT_HDR_OPT_SAVED_PA   0x5E

/* Use the normal size of the RPL_HOP_BY_HOP_LEN plus another 8 bytes for the sequence number TLV.
 * IPv6 extension headers need to be multiples of 8 bytes. The default size of the Hop-By-Hop extension header
 * (RPL_HOP_BY_HOP) is a total of 8 bytes. Therefore we need another 8 bytes to reach the next possible values of 16 bytes.
 * The size of the Sequence number TVL is 1 byte for the type, 1 byte for the length, 4 bytes for the sequence number
 * and 2 bytes for padding which are unused at this time.
 * */
#define UIP_EXT_HDR_OPT_SAVED_PA_LEN    8



/*---------------------------------------------------------------------------*/
/* Public Sequence Number functions */
void saved_ext_pa_header_output(void *saved_pa_buf);
int saved_ext_pa_header_validation(void *saved_pa_buf, int routing_pp_ap);
//uint32_t saved_ext_pp_ap_header_input(void* sequence_number_buf);
//uint32_t sequence_number_get_number_from_packet_attributes();
//uint32_t sequence_number_get_number_from_queuebuf_attributes(struct queuebuf *qb);
/*---------------------------------------------------------------------------*/
#endif /* PRE_SEQUENCE_NUMBER_H */
