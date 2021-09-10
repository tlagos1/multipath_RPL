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
#include "net/ipv6/uip-ds6.h"

#include "net/rpl/rpl.h"
#include "net/rpl/rpl-private.h"
#include "net/sequence-number/sequence-number.h"
#include "net/packetbuf.h"
#include <stdlib.h>

void
dynamic_parent_selection_select_replication_targets_local_addresses(uip_ipaddr_t*out_addresses, int in_addresses_count, int* out_addresses_count)
{

	*out_addresses_count = 0;
	rpl_parent_t *alternative_parent;
	rpl_instance_t * default_instance = rpl_get_default_instance();
	if(default_instance == NULL){
		return;
	}
	rpl_dag_t *dag = default_instance->current_dag;
	if(dag == NULL){
		return;
	}

	uip_ipaddr_t*nexthop = uip_ds6_defrt_choose(); /* get the ip address of the (first) default route */
	uip_ipaddr_t*alternativehop;

	if(nexthop != NULL && in_addresses_count >= 1){ /* if it exists and there is enough space */
		uip_ipaddr_copy(&out_addresses[0], nexthop); /* copy the found default into the target array, in the first element */
#if defined(WITH_ALTERNATIVE_PARENT_SELECTION) & WITH_ALTERNATIVE_PARENT_SELECTION
		if(dag->alternative_parent != NULL){
			alternativehop = rpl_get_parent_ipaddr(dag->alternative_parent);
			uip_ipaddr_copy(&out_addresses[1], alternativehop);
			*out_addresses_count = 2; /* set the count to report that one ip address is being reported */
		}else{
			*out_addresses_count = 1;

		}
#else
		*out_addresses_count = 1;
#endif
	}
}


/** @}*/

