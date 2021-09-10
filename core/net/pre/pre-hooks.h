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
 *         Allows PRE to use different mechanisms for selecting the replication targets.
 * \author
 *         Remous-Aris Koutsiamanis <aris@ariskou.com>
 */

#ifndef PRE_HOOKS_H
#define PRE_HOOKS_H


#include "pre-conf.h"
#include "net/ip/uip.h"

#include <stdint.h>
/*---------------------------------------------------------------------------*/
/* This function can be overriden by setting PRE_CONF_SELECT_REPLICATION_TARGETS_LOCAL_ADDRESSES
 * to another appropriate function with the same signature.
 * The function is provided with:
 *  - out_addresses: a preallocated array of uip_ipaddr_t
 *  - in_addresses_count: the count of available entries in the out_addresses array
 *  - out_addresses_count: a pointer to a preallocated integer to store the used array entries
 *
 * The callback is called when PRE needs to do replication, and it requires a list of *local* IPv6
 * addresses to which to send the replicas. Before the callback is called, an array is allocated with size
 * reported in in_addresses_count. The callback implementation needs to use any way it prefers to produce
 * zero or more replication targets, fill them in the out_addresses array and set the value of the
 * out_addresses_count pointer to the number of entries used in the out_addresses array.
 * See the implementation of pre_hooks_default_select_replication_targets_local_addresses for a simple example.
 */
void pre_hooks_default_select_replication_targets_local_addresses(uip_ipaddr_t*out_addresses, int in_addresses_count, int* out_addresses_count);
/*---------------------------------------------------------------------------*/
#endif /* PRE_HOOKS_H */
