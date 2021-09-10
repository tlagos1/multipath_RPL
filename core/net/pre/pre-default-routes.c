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
#include "net/pre/pre-default-routes.h"
#include "lib/memb.h"
#include "lib/list.h"

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

#include <limits.h>
#include <string.h>

LIST(predefaultrouterlist);
static list_t saveddefaultrouterlist;
static uip_ds6_defrt_t predefaultrouter;

void
pre_default_routes_save_system()
{
  list_init(predefaultrouterlist);
  saveddefaultrouterlist = NULL;
  memset(&predefaultrouter, 0, sizeof(uip_ds6_defrt_t));
  list_push(predefaultrouterlist, &predefaultrouter);
  saveddefaultrouterlist = uip_ds6_defrt_swap_to_list(predefaultrouterlist);
}
/*---------------------------------------------------------------------------*/
void
pre_default_routes_set_target(uip_ipaddr_t* address)
{
  uip_ipaddr_copy(&predefaultrouter.ipaddr, address);
  predefaultrouter.isinfinite = 1;
}
/*---------------------------------------------------------------------------*/
void
pre_default_routes_restore_system()
{
   uip_ds6_defrt_swap_to_list(saveddefaultrouterlist);
}

/** @}*/
