#include "contiki.h"
#include "net/packetbuf.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/rpl/rpl-private.h"
#include "net/mac/tsch/tsch-queue.h"
#include "static-scheduler.h"
#include "static-scheduler-generated.h"
#include "static-scheduler-private.h"
#include "net/rime/rime.h" /* Needed for so-called rime-sniffer */
#include "node-id.h"

#if WITH_SEQUENCE_NUMBER
#include "net/sequence-number/sequence-number.h"
#endif /* WITH_SEQUENCE_NUMBER */

#define DEBUG DEBUG_NONE
#include "net/ip/uip-debug.h"

/*---------------------------------------------------------------------------*/
#if LINKADDR_SIZE == 8
#ifdef CONTIKI_TARGET_Z1
  /* Set node with MAC address c1:0c:00:00:00:00:01 as coordinator,
   * convenient in cooja for regression tests using z1 nodes
   * */
#define INT_TO_LINKADDR(i) ((linkaddr_t){ { 0xc1, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, i } })
#elif CONTIKI_TARGET_COOJA
#define INT_TO_LINKADDR(i) ((linkaddr_t){ { 0, 0, 0, 0, 0, 0, 0, i } })
#endif
#endif /*LINKADDR_SIZE == 8*/

/* for time slot offset calculations */
//#define DATA_BROADCAST_STATIC_SCHEDULER_TIMESLOT_COUNT ((STATIC_SCHEDULER_NODE_IDS_COUNT)*(TSCH_MAC_MAX_FRAME_RETRIES + 1))
//#define EB_STATIC_SCHEDULER_TIMESLOT_COUNT (TSCH_MAC_MAX_FRAME_RETRIES + 1)

/*---------------------------------------------------------------------------*/
uint32_t call_id = 0;
/*---------------------------------------------------------------------------*/
static void
set_int_to_ipv6addr(uint16_t node_identifier, uip_ipaddr_t * addr)
{
  uip_ip6addr(addr, UIP_DS6_DEFAULT_PREFIX, 0x0000, 0x0000, 0x0000, 0x0200 , 0x0000, 0x0000, node_identifier);
}
/*---------------------------------------------------------------------------*/
static void
static_scheduler_print_tsch_cell(const uint16_t slotframe_handle, const uint16_t channel, const uint16_t timeslot, const uint16_t cell_entry, static_scheduler_tsch_cell_t* cell)
{
  char* cell_frame_type_unicast = "UC";
  char* cell_frame_type_multicast = "MC";
  char* cell_frame_type_shared = "SH";
  char* cell_frame_type_skipped = "SK";
  char* cell_frame_type_str = NULL;
  switch(cell->type){
    case STATIC_SCHEDULER_TSCH_CELL_UNICAST:
      cell_frame_type_str = cell_frame_type_unicast;
      break;
    case STATIC_SCHEDULER_TSCH_CELL_MULTICAST:
      cell_frame_type_str = cell_frame_type_multicast;
      break;
    case STATIC_SCHEDULER_TSCH_CELL_SHARED:
      cell_frame_type_str = cell_frame_type_shared;
      break;
    case STATIC_SCHEDULER_TSCH_CELL_SKIPPED:
      cell_frame_type_str = cell_frame_type_skipped;
      break;
    case STATIC_SCHEDULER_TSCH_CELL_UNICAST_KA:
      cell_frame_type_str = cell_frame_type_unicast;
      break;
  }


  PRINTF("Sf %1u Ch %1u Ts %3u Ce %1u / L3 %3u -> %3u (TX G%3u L%3u LC%3u) - L2 (Ty %s) %3u -> %3u (TX %3u) %s Link %p - QC %2u",
         slotframe_handle,
         channel,
         timeslot,
         cell_entry,
         cell->l3_packet_transmission ? cell->l3_packet_transmission->from_l3_node : 0,
         cell->l3_packet_transmission ? cell->l3_packet_transmission->to_l3_node : 0,
         cell->l3_packet_transmission ? cell->l3_packet_transmission->l3_packet_transmission_global : 0,
         cell->l3_packet_transmission ? cell->l3_packet_transmission->l3_packet_transmission_local : 0,
         cell->l3_packet_transmission ? cell->l3_packet_transmission->l3_packet_transmission_local_count : 0,
         cell_frame_type_str,
         cell->from_node,
         cell->to_node,
         cell->packet_transmission,
         cell->overhearing?"OH":"NO",
         cell->link,
         cell->packet_count_in_queue
         );
}
/*---------------------------------------------------------------------------*/
static void
static_scheduler_packet_received(void)
{
  PRINTF("static-scheduler: packet_received\n");
}
/*---------------------------------------------------------------------------*/
static void
static_scheduler_packet_sent(int mac_status)
{
  PRINTF("static-scheduler: packet_sent: mac status %i\n", mac_status);
}
/*---------------------------------------------------------------------------*/
RIME_SNIFFER(static_scheduler_sniffer, static_scheduler_packet_received, static_scheduler_packet_sent);
/*---------------------------------------------------------------------------*/
//static int
//select_packet_unicast_non_udp(struct tsch_packet*p, uint16_t *out_slotframe_handle, uint16_t *out_timeslot, const static_scheduler_tsch_cell_t* candidate_tsch_cell, const linkaddr_t *packet_l2_src, const linkaddr_t *packet_l2_dest, packetbuf_attr_t packet_frame_type, packetbuf_attr_t packet_cell_type, packetbuf_attr_t packet_ipv6_protocol, packetbuf_attr_t packet_ipv6_tcp_udp_min_of_src_and_dest_port, packetbuf_attr_t packet_icmpv6_type, packetbuf_attr_t packet_icmpv6_code, const uip_ipaddr_t* packet_l3_src, const uip_ipaddr_t* packet_l3_dest, const uint32_t sequence_number)
//{
//	PRINTF("static-scheduler: select_packet_unicast_non_udp type = %i \n",candidate_tsch_cell->type);
//	 if(candidate_tsch_cell->from_node == 0 || /* need valid sender */
//	     candidate_tsch_cell->to_node == 0 || /* need unicast receiver */
//	     candidate_tsch_cell->overhearing == 1 /* need non-overhearing = normal TX cell */
//	    )
//	    return 0;
//
//	 const linkaddr_t cell_l2_src = INT_TO_LINKADDR(candidate_tsch_cell->from_node); /* cell L2 sender */
//     const linkaddr_t cell_l2_dest = INT_TO_LINKADDR(candidate_tsch_cell->to_node); /* cell L2 receiver */
//
//
//     if( linkaddr_cmp(packet_l2_src, &cell_l2_src) && /* same L2 src and dest */
//	       linkaddr_cmp(packet_l2_dest, &cell_l2_dest)){
//		  return 1;
//	  }
//	  else{
//		  return 0;
//	  }
//}
/*---------------------------------------------------------------------------*/
uint16_t
static_scheduler_unicast_slot_exists(const linkaddr_t *packet_l2_src, const linkaddr_t *packet_l2_dest){
	uint16_t channel, timeslot, cell_entry;
	for(timeslot = 0; timeslot < STATIC_SCHEDULER_TIMESLOT_COUNT; timeslot++) {
		for(channel = 0; channel < STATIC_SCHEDULER_CHANNEL_COUNT; channel++) {
			for(cell_entry = 0; cell_entry < STATIC_SCHEDULER_CELL_ENTRIES; cell_entry++) {
				static_scheduler_tsch_cell_t* candidate_tsch_cell = &static_scheduler_schedule[channel][timeslot][cell_entry];

				linkaddr_t cell_l2_src = INT_TO_LINKADDR(candidate_tsch_cell->from_node); /* cell L2 sender */
				linkaddr_t cell_l2_dest = INT_TO_LINKADDR(candidate_tsch_cell->to_node); /* cell L2 receiver */

				if( linkaddr_cmp(packet_l2_src, &cell_l2_src) && /* same L2 src and dest */
					linkaddr_cmp(packet_l2_dest, &cell_l2_dest)){
					return 1;

				}
			}
		}
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
select_packet_unicast(struct tsch_packet*p, uint16_t *out_slotframe_handle, uint16_t *out_timeslot, const static_scheduler_tsch_cell_t* candidate_tsch_cell, const linkaddr_t *packet_l2_src, const linkaddr_t *packet_l2_dest, packetbuf_attr_t packet_frame_type, packetbuf_attr_t packet_cell_type, packetbuf_attr_t packet_ipv6_protocol, packetbuf_attr_t packet_ipv6_tcp_udp_min_of_src_and_dest_port, packetbuf_attr_t packet_icmpv6_type, packetbuf_attr_t packet_icmpv6_code, const uip_ipaddr_t* packet_l3_src, const uip_ipaddr_t* packet_l3_dest, const uint32_t sequence_number)
{
  if(candidate_tsch_cell->from_node == 0 || /* need valid sender */
     candidate_tsch_cell->to_node == 0 || /* need unicast receiver */
     candidate_tsch_cell->overhearing == 1/* need non-overhearing = normal TX cell */
    )
    return 0;

  const linkaddr_t cell_l2_src = INT_TO_LINKADDR(candidate_tsch_cell->from_node); /* cell L2 sender */
  const linkaddr_t cell_l2_dest = INT_TO_LINKADDR(candidate_tsch_cell->to_node); /* cell L2 receiver */

  uip_ipaddr_t cell_l3_src, cell_l3_dest;
  set_int_to_ipv6addr(candidate_tsch_cell->l3_packet_transmission->from_l3_node, &cell_l3_src); /* cell L3 sender */
  set_int_to_ipv6addr(candidate_tsch_cell->l3_packet_transmission->to_l3_node, &cell_l3_dest); /* cell L3 receiver */

  uint16_t packet_l3_transmission = 1 +
    ((-1) % candidate_tsch_cell->l3_packet_transmission->l3_packet_transmission_local_count);

  PRINTF("static-scheduler: select_packet_unicast: candidate cell L3 src ");
  PRINT6ADDR(&cell_l3_src);
  PRINTF(" L3 dest ");
  PRINT6ADDR(&cell_l3_dest);
  PRINTF(" tx_seq_num_mod %u ptx_seq_num_mod %u ", packet_l3_transmission, candidate_tsch_cell->l3_packet_transmission->l3_packet_transmission_local);
  PRINTF("L2 src ");
  PRINTLLADDR((const uip_lladdr_t *)&cell_l2_src);
  PRINTF(" L2 dest ");
  PRINTLLADDR((const uip_lladdr_t *)&cell_l2_dest);
  PRINTF(" tx %u", candidate_tsch_cell->packet_transmission - 1);
  PRINTF(" p_transmissions %u\n", p->transmissions);

  if (p->transmissions != 0){
	  if( linkaddr_cmp(packet_l2_src, &cell_l2_src) && /* same L2 src and dest */
			  linkaddr_cmp(packet_l2_dest, &cell_l2_dest) &&
			  uip_ipaddr_cmp(packet_l3_src, &cell_l3_src) && /* same L3 src and dest */
			  uip_ipaddr_cmp(packet_l3_dest, &cell_l3_dest) &&
			  packet_l3_transmission == candidate_tsch_cell->l3_packet_transmission->l3_packet_transmission_local && /* same L2 and L3 transmission number */
#if  ROOT_CHILDREN_OVERH_COMPENSATION_ENABLED
              p->transmissions == candidate_tsch_cell->packet_transmission - 1)
#else
              (p->transmissions%2) == candidate_tsch_cell->packet_transmission - 1)
#endif
	  {
		  return 1;
	  }
  }
  else{
	  if( linkaddr_cmp(packet_l2_src, &cell_l2_src) && /* same L2 src and dest */
	      linkaddr_cmp(packet_l2_dest, &cell_l2_dest) &&
	      uip_ipaddr_cmp(packet_l3_src, &cell_l3_src) && /* same L3 src and dest */
	      uip_ipaddr_cmp(packet_l3_dest, &cell_l3_dest) &&
	      packet_l3_transmission == candidate_tsch_cell->l3_packet_transmission->l3_packet_transmission_local && /* same L2 and L3 transmission number */
		  p->transmissions ==  candidate_tsch_cell->packet_transmission - 1)
	  {
		  return 1;
	  }
  }
    return 0;
}
/*---------------------------------------------------------------------------*/
static int
//select_packet_multicast(struct tsch_packet*p, uint16_t *out_slotframe_handle, uint16_t *out_timeslot, const static_scheduler_tsch_cell_t* candidate_tsch_cell, const linkaddr_t *packet_l2_src, linkaddr_t *packet_l2_dest, packetbuf_attr_t packet_frame_type, packetbuf_attr_t packet_cell_type, packetbuf_attr_t packet_ipv6_protocol, packetbuf_attr_t packet_ipv6_tcp_udp_min_of_src_and_dest_port, packetbuf_attr_t packet_icmpv6_type, packetbuf_attr_t packet_icmpv6_code, const uip_ipaddr_t* packet_l3_src, const uip_ipaddr_t* packet_l3_dest, const uint32_t sequence_number)
select_packet_multicast(struct tsch_packet*p, uint16_t *out_slotframe_handle, uint16_t *out_timeslot, const static_scheduler_tsch_cell_t* candidate_tsch_cell, const linkaddr_t *packet_l2_src, linkaddr_t *packet_l2_dest, packetbuf_attr_t packet_frame_type, packetbuf_attr_t packet_cell_type, packetbuf_attr_t packet_ipv6_protocol, packetbuf_attr_t packet_ipv6_tcp_udp_min_of_src_and_dest_port, packetbuf_attr_t packet_icmpv6_type, packetbuf_attr_t packet_icmpv6_code, const uip_ipaddr_t* packet_l3_src, const uip_ipaddr_t* packet_l3_dest, const uint32_t sequence_number)
{
    const linkaddr_t cell_l2_src = INT_TO_LINKADDR(candidate_tsch_cell->from_node); /* cell L2 sender */

    PRINTF("static-scheduler: select_packet_multicast - packet_l2_src : ");
    PRINTLLADDR((uip_lladdr_t*)packet_l2_src);
    PRINTF(" - cell_l2_src : ");
    PRINTLLADDR((uip_lladdr_t*)&cell_l2_src);
    PRINTF(" p->transmission : %u , candidate_tsch_cell->packet_transmission: %u\n",p->transmissions,candidate_tsch_cell->packet_transmission - 1);

    if( linkaddr_cmp(packet_l2_src, &cell_l2_src) //&& /* same L2 src */
        //p->transmissions == candidate_tsch_cell->packet_transmission - 1 /* same L2 transmission number */
      )
      return 1;
    else
      return 0;
}
/*---------------------------------------------------------------------------*/
static int
select_packet_shared(struct tsch_packet*p, uint16_t *out_slotframe_handle, uint16_t *out_timeslot, const static_scheduler_tsch_cell_t* candidate_tsch_cell, const linkaddr_t *packet_l2_src, const linkaddr_t *packet_l2_dest, packetbuf_attr_t packet_frame_type, packetbuf_attr_t packet_cell_type, packetbuf_attr_t packet_ipv6_protocol, packetbuf_attr_t packet_ipv6_tcp_udp_min_of_src_and_dest_port, packetbuf_attr_t packet_icmpv6_type, packetbuf_attr_t packet_icmpv6_code)
{
  if( p->transmissions == candidate_tsch_cell->packet_transmission - 1 /* same L2 transmission number */
    )
    return 1;
  else
    return 0;
}
/*---------------------------------------------------------------------------*/

void
static_scheduler_callback_tsch_packet_ready_or_retx(struct tsch_packet*p)
{
  call_id++;
  PRINTF("static-scheduler: callback_tsch_packet_ready_or_retx: call id %i\n", call_id);
//  if((node_id == 4 && call_id == 1) || (node_id == 2 && call_id == 3))
//    asm("int $3");

  const linkaddr_t *packet_l2_src = queuebuf_addr(p->qb, PACKETBUF_ADDR_SENDER);
  const linkaddr_t *packet_l2_dest = queuebuf_addr(p->qb, PACKETBUF_ADDR_RECEIVER);
  linkaddr_t *packet_l2_dest_multicast = queuebuf_addr(p->qb, PACKETBUF_ADDR_RECEIVER);

  packetbuf_attr_t packet_frame_type = queuebuf_attr(p->qb, PACKETBUF_ATTR_FRAME_TYPE);
  packetbuf_attr_t packet_cell_type = queuebuf_attr(p->qb, PACKETBUF_ATTR_STATIC_SCHEDULER_TSCH_CELL_TYPE);
  packetbuf_attr_t packet_ipv6_protocol = queuebuf_attr(p->qb, PACKETBUF_ATTR_NETWORK_ID);


  packetbuf_attr_t packet_ipv6_tcp_udp_min_of_src_and_dest_port = 0;
  packetbuf_attr_t packet_icmpv6_type = 0;
  packetbuf_attr_t packet_icmpv6_code = 0;

  if(packet_ipv6_protocol == UIP_PROTO_UDP || packet_ipv6_protocol == UIP_PROTO_TCP){
    packet_ipv6_tcp_udp_min_of_src_and_dest_port = queuebuf_attr(p->qb, PACKETBUF_ATTR_CHANNEL);
  }else if(packet_ipv6_protocol == UIP_PROTO_ICMP6){
    packet_icmpv6_type = queuebuf_attr(p->qb, PACKETBUF_ATTR_CHANNEL) >> 8;
    packet_icmpv6_code = queuebuf_attr(p->qb, PACKETBUF_ATTR_CHANNEL) & 0xff;
  }
  PRINTF("static-scheduler: callback_tsch_packet_ready_or_retx: deciding TSCH cell for packet L2 src ");
  PRINTLLADDR((uip_lladdr_t*)packet_l2_src);
  PRINTF(" L2 dest ");
  PRINTLLADDR((uip_lladdr_t*)packet_l2_dest);
  PRINTF(" type ");
  if(packet_frame_type == FRAME802154_BEACONFRAME)
    PRINTF("EB");
  else if(packet_frame_type == FRAME802154_DATAFRAME){
    PRINTF("DATA ");
    if(packet_ipv6_protocol == UIP_PROTO_UDP || packet_ipv6_protocol == UIP_PROTO_TCP){
      PRINTF("protocol %s min_of_src_and_dest_port %i", packet_ipv6_protocol == UIP_PROTO_UDP? "UDP" : "TCP", packet_ipv6_tcp_udp_min_of_src_and_dest_port);
    }else if(packet_ipv6_protocol == UIP_PROTO_ICMP6){
      PRINTF("protocol ICMPv6 type %i code %i", packet_icmpv6_type, packet_icmpv6_code);
    }else{
      PRINTF("protocol %i",packet_ipv6_protocol);
    }
  }else
    PRINTF("OTHER(%u)", packet_frame_type);
  queuebuf_attr(p->qb, PACKETBUF_ATTR_STATIC_SCHEDULER_TSCH_CELL_TYPE);

  PRINTF(" static sched cell type %u tx %u\n", packet_cell_type, p->transmissions);

  //select_packet(p, &slotframe_handle, &timeslot, packet_l2_src, packet_l2_dest, packet_frame_type, packet_cell_type, packet_ipv6_protocol, packet_ipv6_tcp_udp_min_of_src_and_dest_port, packet_icmpv6_type, packet_icmpv6_code);

  if(
      packet_cell_type != STATIC_SCHEDULER_TSCH_CELL_UNICAST &&
	  packet_cell_type != STATIC_SCHEDULER_TSCH_CELL_UNICAST_KA &&
      packet_cell_type != STATIC_SCHEDULER_TSCH_CELL_MULTICAST &&
      packet_cell_type != STATIC_SCHEDULER_TSCH_CELL_SHARED)
    PRINTF("static-scheduler: static_scheduler_callback_tsch_packet_ready_or_retx: error! invalid packet tsch cell type: %u\n", packet_cell_type);

  uip_ipaddr_t packet_l3_src;
  uip_ipaddr_t packet_l3_dest;
  uint32_t packet_sequence_number;
  if(packet_cell_type == STATIC_SCHEDULER_TSCH_CELL_UNICAST){/* extract IPv6 info for this case here instead of inside loop as an optimisation */
    uip_ip6addr(&packet_l3_src,
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_0),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_1),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_2),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_3),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_4),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_5),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_6),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_SRC_7)
                ); /* get IPv6 sender */
    uip_ip6addr(&packet_l3_dest,
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_0),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_1),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_2),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_3),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_4),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_5),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_6),
                queuebuf_attr(p->qb, PACKETBUF_ATTR_IPV6_DEST_7)
                ); /* get IPv6 receiver */
    packet_sequence_number = sequence_number_get_number_from_queuebuf_attributes(p->qb); /* get - if it exists - the sequence number */

    PRINTF("static-scheduler: static_scheduler_callback_tsch_packet_ready_or_retx: packet L3 src ");
    PRINT6ADDR(&packet_l3_src);
    PRINTF(" L3 dest ");
    PRINT6ADDR(&packet_l3_dest);
    PRINTF(" rpl_seq_num %u\n", packet_sequence_number);
  }else if( packet_cell_type == STATIC_SCHEDULER_TSCH_CELL_MULTICAST && /* going to the multicast slots */
      !linkaddr_cmp(packet_l2_dest, &tsch_eb_address) && !linkaddr_cmp(packet_l2_dest, &tsch_broadcast_address) /* but is a unicast packet */
    ){
	PRINTF("static-scheduler: static_scheduler_callback_tsch_packet_ready_or_retx: Unicast PACKET ");
	PRINTLLADDR((uip_lladdr_t*)packet_l2_dest);
	PRINTF(" to MULTICAST ");
    linkaddr_copy(packet_l2_dest_multicast, &tsch_broadcast_address); /* set the dest to convert to multicast */

    PRINTLLADDR((uip_lladdr_t*)packet_l2_dest);
//    queuebuf_set_attr(p->qb, PACKETBUF_ADDR_RECEIVER, &tsch_broadcast_address);
    PRINTF("\n");
  }

  uint16_t channel, timeslot, cell_entry, slotframe_handle = 0;
  int successful_match = 0;

  for(timeslot = 0; timeslot < STATIC_SCHEDULER_TIMESLOT_COUNT; timeslot++) {
    for(channel = 0; channel < STATIC_SCHEDULER_CHANNEL_COUNT; channel++) {
      for(cell_entry = 0; cell_entry < STATIC_SCHEDULER_CELL_ENTRIES; cell_entry++) {
        if(static_scheduler_schedule[channel][timeslot][cell_entry].type != packet_cell_type)
          continue;
        else{
          static_scheduler_tsch_cell_t* candidate_tsch_cell = &static_scheduler_schedule[channel][timeslot][cell_entry];
          switch(packet_cell_type){
            case STATIC_SCHEDULER_TSCH_CELL_UNICAST:
              successful_match = select_packet_unicast(p, &slotframe_handle, &timeslot, candidate_tsch_cell, packet_l2_src, packet_l2_dest, packet_frame_type, packet_cell_type, packet_ipv6_protocol, packet_ipv6_tcp_udp_min_of_src_and_dest_port, packet_icmpv6_type, packet_icmpv6_code, &packet_l3_src, &packet_l3_dest, packet_sequence_number);
              break;
            case STATIC_SCHEDULER_TSCH_CELL_MULTICAST:
              successful_match = select_packet_multicast(p, &slotframe_handle, &timeslot, candidate_tsch_cell, packet_l2_src, packet_l2_dest_multicast, packet_frame_type, packet_cell_type, packet_ipv6_protocol, packet_ipv6_tcp_udp_min_of_src_and_dest_port, packet_icmpv6_type, packet_icmpv6_code, &packet_l3_src, &packet_l3_dest, packet_sequence_number);
              break;
            case STATIC_SCHEDULER_TSCH_CELL_SHARED:
              successful_match = select_packet_shared(p, &slotframe_handle, &timeslot, candidate_tsch_cell, packet_l2_src, packet_l2_dest, packet_frame_type, packet_cell_type, packet_ipv6_protocol, packet_ipv6_tcp_udp_min_of_src_and_dest_port, packet_icmpv6_type, packet_icmpv6_code);
              break;
            case STATIC_SCHEDULER_TSCH_CELL_SKIPPED:
              break;

          }
          if(successful_match){ /* found a matching cell */
            PRINTF("static-scheduler: static_scheduler_callback_tsch_packet_ready_or_retx: selected TSCH cell: ");
            static_scheduler_print_tsch_cell(slotframe_handle, channel, timeslot, cell_entry, candidate_tsch_cell);
            PRINTF("\n");
            break;
          }
        }
      }
      if(successful_match)
        break;
    }
    if(successful_match)
      break;
  }
  if(slotframe_handle == 0 && timeslot == STATIC_SCHEDULER_TIMESLOT_COUNT){
    PRINTF("static-scheduler: callback_tsch_packet_ready_or_retx: could not find an appropriate cell\n");
//    asm("int $3");
  }

  /* the equivalent of the non-existent queuebuf_set_attr(p->qb, PACKETBUF_ATTR_TSCH_SLOTFRAME, slotframe_handle); */
  queuebuf_set_attr(p->qb, PACKETBUF_ATTR_TSCH_SLOTFRAME, slotframe_handle);
  /* the equivalent of the non-existent queuebuf_set_attr(p->qb, PACKETBUF_ATTR_TSCH_TIMESLOT, timeslot); */
  queuebuf_set_attr(p->qb, PACKETBUF_ATTR_TSCH_TIMESLOT, timeslot);

}
/*---------------------------------------------------------------------------*/
/* Public Interface */
void
static_scheduler_callback_child_added(const linkaddr_t *addr)
{
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_child_removed(const linkaddr_t *addr)
{
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_callback_new_time_source(const struct tsch_neighbor *old, const struct tsch_neighbor *new)
{
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_init(void)
{
  PRINTF("static-scheduler: initializating\n");

  rime_sniffer_add(&static_scheduler_sniffer);

  /* Slotframe for all transmissions, size is eb_timeslot_count (for EB) + STATIC_SCHEDULER_NODE_IDS_COUNT (for DIO/ 1 node broadcasts others listen) + generated ones */
  const uint16_t slotframe_handle = 0;
  struct tsch_slotframe * static_scheduler_slotframe = tsch_schedule_add_slotframe(slotframe_handle, STATIC_SCHEDULER_TIMESLOT_COUNT);

  PRINTF("static-scheduler: init: Current MAC ");
  PRINTLLADDR((const uip_lladdr_t *)&linkaddr_node_addr);
  PRINTF("\n");

  uint16_t channel, timeslot, cell_entry;
  for(channel = 0; channel < STATIC_SCHEDULER_CHANNEL_COUNT; channel++) {
    for(timeslot = 0; timeslot < STATIC_SCHEDULER_TIMESLOT_COUNT; timeslot++) {
      for(cell_entry = 0; cell_entry < STATIC_SCHEDULER_CELL_ENTRIES; cell_entry++) {
        static_scheduler_tsch_cell_t* current_tsch_cell = &static_scheduler_schedule[channel][timeslot][cell_entry];

        linkaddr_t from_node_linkaddr = INT_TO_LINKADDR(current_tsch_cell->from_node);
        linkaddr_t to_node_linkaddr = INT_TO_LINKADDR(current_tsch_cell->to_node);


        PRINTF("static-scheduler: init: Cell: ");
        static_scheduler_print_tsch_cell(slotframe_handle, channel, timeslot, cell_entry, current_tsch_cell);

        PRINTF(" L2 src MAC ");
        PRINTLLADDR((const uip_lladdr_t *)&from_node_linkaddr);
        PRINTF(" L2 dest MAC ");
        PRINTLLADDR((const uip_lladdr_t *)&to_node_linkaddr);

        if(current_tsch_cell->type == STATIC_SCHEDULER_TSCH_CELL_UNICAST || current_tsch_cell->type == STATIC_SCHEDULER_TSCH_CELL_UNICAST_KA){ /* unicast cell */
          /* TX */
          if(linkaddr_cmp(&from_node_linkaddr, &linkaddr_node_addr)) {
            if(current_tsch_cell->overhearing){
              PRINTF(" overhearing unicast TX: no timeslot created. Skipping\n");
            }else{
              PRINTF(" normal unicast TX: node %u TX towards node %u\n",
                     current_tsch_cell->from_node,
                     current_tsch_cell->to_node
                     );
              current_tsch_cell->link = tsch_schedule_add_link(static_scheduler_slotframe,
                  LINK_OPTION_TX,
                  LINK_TYPE_NORMAL, &to_node_linkaddr,
                  timeslot, channel);
              current_tsch_cell->link->data = current_tsch_cell; /* make the static_schedule cell accessible from the link */
            }
          }
          /* RX */
          else if(linkaddr_cmp(&to_node_linkaddr, &linkaddr_node_addr)) {
            if(current_tsch_cell->overhearing){
#if TSCH_WITH_OVERHEARING
              PRINTF(" overhearing unicast RX: node %u RX from node %u overhearing %u\n",
                     current_tsch_cell->to_node,
                     current_tsch_cell->from_node,
                     current_tsch_cell->overhearing
                     );
              current_tsch_cell->link = tsch_schedule_add_link(static_scheduler_slotframe,
                  LINK_OPTION_RX | LINK_OPTION_OVERHEARING, /* add overhearing to cell options */
                  LINK_TYPE_NORMAL, &tsch_broadcast_address, /* listen for TX from any address */
                  timeslot, channel);
              current_tsch_cell->link->data = current_tsch_cell; /* make the static_schedule cell accessible from the link */
#else /* TSCH_WITH_OVERHEARING */
              PRINTF(" overhearing unicast RX: node %u RX from node %u overhearing %u, but overhearing disabled: no timeslot created. Skipping\n",
                     current_tsch_cell->to_node,
                     current_tsch_cell->from_node,
                     current_tsch_cell->overhearing
                     );
#endif /* TSCH_WITH_OVERHEARING */
            }else{
              PRINTF(" normal unicast RX: node %u RX from node %u\n",
                     current_tsch_cell->to_node,
                     current_tsch_cell->from_node,
                     current_tsch_cell->overhearing
                     );
              current_tsch_cell->link = tsch_schedule_add_link(static_scheduler_slotframe,
                  LINK_OPTION_RX,
                  LINK_TYPE_NORMAL, &from_node_linkaddr,
                  timeslot, channel);
              current_tsch_cell->link->data = current_tsch_cell; /* make the static_schedule cell accessible from the link */
            }
          }else{
            PRINTF(" irrelevant unicast: this node %u is not involved: no timeslot created. Skipping.\n");
          }
        }else if(current_tsch_cell->type == STATIC_SCHEDULER_TSCH_CELL_MULTICAST){ /* multicast cell */
          if(linkaddr_cmp(&from_node_linkaddr, &linkaddr_node_addr)){ //TX cell
            PRINTF(" multicast TX: node %u TX towards node ALL\n",
                   current_tsch_cell->from_node
                   );
            current_tsch_cell->link = tsch_schedule_add_link(static_scheduler_slotframe,
                LINK_OPTION_SHARED | LINK_OPTION_TX,
                LINK_TYPE_NORMAL, &tsch_broadcast_address,
                timeslot, channel);
            current_tsch_cell->link->data = current_tsch_cell; /* make the static_schedule cell accessible from the link */
            break;
          }else{ // RX cell
            PRINTF(" multicast RX: node %u TX towards node ALL, including node %u\n",
                   current_tsch_cell->from_node,
                   node_id
                   );
            current_tsch_cell->link = tsch_schedule_add_link(static_scheduler_slotframe,
                LINK_OPTION_SHARED | LINK_OPTION_RX,
                LINK_TYPE_NORMAL, &from_node_linkaddr,
                timeslot, channel);
            current_tsch_cell->link->data = current_tsch_cell; /* make the static_schedule cell accessible from the link */
          }
        }else if(current_tsch_cell->type == STATIC_SCHEDULER_TSCH_CELL_SHARED){ /* shared cell */
          PRINTF(" shared: node %u TX/RX towards/from node ALL\n",
                 node_id
                 );
          current_tsch_cell->link = tsch_schedule_add_link(static_scheduler_slotframe,
                                 LINK_OPTION_SHARED | LINK_OPTION_TX | LINK_OPTION_RX,
                                 LINK_TYPE_ADVERTISING, &tsch_broadcast_address,
                                 timeslot, channel);
          current_tsch_cell->link->data = current_tsch_cell; /* make the static_schedule cell accessible from the link */
        }else{ /* error cell */
          PRINTF(" error!\n");
        }
      }
    }
  }

  PRINTF("static-scheduler: initialized\n");
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_print(){
  const uint16_t slotframe_handle = 0;

  PRINTF("static-scheduler: print schedule: START\n");
  uint16_t channel, timeslot, cell_entry;
  for(channel = 0; channel < STATIC_SCHEDULER_CHANNEL_COUNT; channel++) {
    for(timeslot = 0; timeslot < STATIC_SCHEDULER_TIMESLOT_COUNT; timeslot++) {
      for(cell_entry = 0; cell_entry < STATIC_SCHEDULER_CELL_ENTRIES; cell_entry++) {
        if(static_scheduler_schedule[channel][timeslot][cell_entry].link != NULL){ /* is this added to the schedule? */
          PRINTF("static-scheduler: print schedule: ");
          static_scheduler_print_tsch_cell(slotframe_handle, channel, timeslot, cell_entry, &static_scheduler_schedule[channel][timeslot][cell_entry]);
          PRINTF("\n");
        }
      }
    }
  }
  PRINTF("static-scheduler: print schedule: END\n");
}
/*---------------------------------------------------------------------------*/
static packetbuf_attr_t
decide_packet_cell_type(const linkaddr_t *packet_l2_dest, packetbuf_attr_t packet_frame_type, packetbuf_attr_t packet_ipv6_protocol)
{
  uint8_t packet_cell_type;
  PRINTF("static-scheduler: decide_packet_cell_type: packet cell type: ");
  if(
      packet_frame_type == FRAME802154_DATAFRAME /* 802.15.4 data */
      && !linkaddr_cmp(packet_l2_dest, &tsch_eb_address) && !linkaddr_cmp(packet_l2_dest, &tsch_broadcast_address) /* unicast */
      && packet_ipv6_protocol == UIP_PROTO_UDP /* UDP */
    ){
    PRINTF("UNICAST\n");
    packet_cell_type = STATIC_SCHEDULER_TSCH_CELL_UNICAST;
  }
  else if(
      packet_frame_type == FRAME802154_DATAFRAME /* 802.15.4 data */
    ){
    PRINTF("MULTICAST\n");
    packet_cell_type = STATIC_SCHEDULER_TSCH_CELL_MULTICAST;
  }
  else if(
      packet_frame_type == FRAME802154_BEACONFRAME /* 802.15.4 EB */
    ){
    PRINTF("SHARED\n");
    packet_cell_type = STATIC_SCHEDULER_TSCH_CELL_SHARED;
  }
  else{ /* skipped case, used as filler */
    PRINTF("SKIPPED\n");
    packet_cell_type = STATIC_SCHEDULER_TSCH_CELL_SKIPPED;
  }
  return packet_cell_type;
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_sicslowpan_callback_set_packet_attr_addr(){
  const linkaddr_t *packet_l2_dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  packetbuf_attr_t packet_frame_type = FRAME802154_DATAFRAME; /* implied since this is called from siclowpan, but this value is set later in the stack processing, in TSCH */
  packetbuf_attr_t packet_ipv6_protocol = packetbuf_attr(PACKETBUF_ATTR_NETWORK_ID);

  packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER_OVERRIDE_IN_FRAMER, packet_l2_dest);

  packetbuf_attr_t packet_cell_type = decide_packet_cell_type(packet_l2_dest, packet_frame_type, packet_ipv6_protocol);
  packetbuf_set_attr(PACKETBUF_ATTR_STATIC_SCHEDULER_TSCH_CELL_TYPE, packet_cell_type);

  switch(packet_cell_type){
    case STATIC_SCHEDULER_TSCH_CELL_UNICAST:
      PRINTF("static-scheduler: static_scheduler_sicslowpan_callback_set_packet_attr_addr: packet is UNICAST. No action.\n");
      break;
    case STATIC_SCHEDULER_TSCH_CELL_MULTICAST:
      PRINTF("static-scheduler: static_scheduler_sicslowpan_callback_set_packet_attr_addr: packet is MULTICAST:  packet is set to broadcast in L2\n");
      packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER, &tsch_broadcast_address);
      break;
    case STATIC_SCHEDULER_TSCH_CELL_SHARED:
    case STATIC_SCHEDULER_TSCH_CELL_SKIPPED:
      PRINTF("static-scheduler: static_scheduler_sicslowpan_callback_set_packet_attr_addr: packet is MISCATEGORISED! Error!\n");
      break;
  }
}
/*---------------------------------------------------------------------------*/
void
static_scheduler_tsch_callback_eb_set_packet_attr_addr(){
  const linkaddr_t *packet_l2_dest = packetbuf_addr(PACKETBUF_ADDR_RECEIVER);
  packetbuf_attr_t packet_frame_type = packetbuf_attr(PACKETBUF_ATTR_FRAME_TYPE); /* FRAME802154_BEACONFRAME implied since this is called from TSCH for EB */
  packetbuf_attr_t packet_ipv6_protocol = packetbuf_attr(PACKETBUF_ATTR_NETWORK_ID);

  packetbuf_set_addr(PACKETBUF_ADDR_RECEIVER_OVERRIDE_IN_FRAMER, packet_l2_dest);

  packetbuf_attr_t packet_cell_type = decide_packet_cell_type(packet_l2_dest, packet_frame_type, packet_ipv6_protocol);
  packetbuf_set_attr(PACKETBUF_ATTR_STATIC_SCHEDULER_TSCH_CELL_TYPE, packet_cell_type);

  switch(packet_cell_type){
    case STATIC_SCHEDULER_TSCH_CELL_UNICAST:
    case STATIC_SCHEDULER_TSCH_CELL_MULTICAST:
    case STATIC_SCHEDULER_TSCH_CELL_SKIPPED:
      PRINTF("static-scheduler: static_scheduler_tsch_callback_eb_set_packet_attr_addr: packet is MISCATEGORISED! Error!\n");
      break;
    case STATIC_SCHEDULER_TSCH_CELL_SHARED:
      PRINTF("static-scheduler: static_scheduler_tsch_callback_eb_set_packet_attr_addr: packet is SHARED: No action.\n");
      break;
  }
}
/*---------------------------------------------------------------------------*/
uint8_t
static_scheduler_tsch_queue_should_use_strict_timeslot_match(struct tsch_link* l)
{
	const int strict_match = 1;
//  const int strict_match = ((static_scheduler_tsch_cell_t*)(l->data))->type == STATIC_SCHEDULER_TSCH_CELL_UNICAST;
  PRINTF("static-scheduler: tsch_queue_should_use_strict_timeslot_match: %i\n", strict_match);
  return strict_match;
}
/*---------------------------------------------------------------------------*/
