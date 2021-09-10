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

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

#include <limits.h>
#include <string.h>

void
pre_hooks_default_select_replication_targets_local_addresses(uip_ipaddr_t*out_addresses, int in_addresses_count, int* out_addresses_count)
{
  uip_ipaddr_t*nexthop = uip_ds6_defrt_choose(); /* get the ip address of the (first) default route */
  PRINTF("PRE: pre_hooks_default_select_replication_targets_local_addresses: nexthop: ");
  PRINT6ADDR(nexthop);
  PRINTF("\n");
  if(nexthop != NULL && in_addresses_count >= 1){ /* if it exists and there is enough space */
    PRINTF("PRE: pre_hooks_default_select_replication_targets_local_addresses: using nexthop\n");
    uip_ipaddr_copy(&out_addresses[0], nexthop); /* copy the found default into the target array, in the first element */
    *out_addresses_count = 1; /* set the count to report that one ip address is being reported */

  }else{
    PRINTF("PRE: pre_hooks_default_select_replication_targets_local_addresses: no nexthop. Dropping!\n");
    *out_addresses_count = 0; /* no addresses used, perform no replication (i.e., drop the packet) */
  }
}

/** @}*/
