#include "contiki.h"
#include "node-id.h"
#include "powertrace.h"
#include "net/rpl/rpl.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/mac/tsch/tsch.h"
#include "net/rpl/rpl-private.h"
#if WITH_ORCHESTRA
#include "orchestra.h"
#endif /* WITH_ORCHESTRA */
#if WITH_STATIC_SCHEDULER
#include "static-scheduler.h"
#endif /* WITH_STATIC_SCHEDULER */
#if WITH_SEQUENCE_NUMBER
#include "net/sequence-number/sequence-number.h"
#endif /* WITH_SEQUENCE_NUMBER */
#if WITH_PACKET_REPLICATION_AND_ELIMINATION
#include "net/pre/pre-history.h"
#endif /* WITH_PACKET_REPLICATION_AND_ELIMINATION */

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"

#define CONFIG_VIA_BUTTON PLATFORM_HAS_BUTTON
#if CONFIG_VIA_BUTTON
#include "button-sensor.h"
#endif /* CONFIG_VIA_BUTTON */

#include "simple-udp.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "net/mac/tsch/tsch-private.h"
#include "apps/static-scheduler/static-scheduler-generated.h"
#include "net/unique_id.h"

#define UDP_PORT 1234

#define WAIT_FOR_RPL_INTERVAL (10 * CLOCK_SECOND)
#define START_SEND_INTERVAL (90 * CLOCK_SECOND)
#define SEND_INTERVAL (18 * CLOCK_SECOND)


#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

static struct simple_udp_connection unicast_connection;
static uint8_t sender_node_array[SENDER_NODE_ARRAY_COUNT] = SENDER_NODE_ARRAY;
/*---------------------------------------------------------------------------*/
PROCESS(node_process, "RPL Node");
PROCESS(unicast_sender_process, "Unicast sender example process");
AUTOSTART_PROCESSES(&node_process, &unicast_sender_process);


/*---------------------------------------------------------------------------*/
static void
print_network_status(void)
{
    int i;
    uint8_t state;
    uip_ds6_defrt_t *default_route;
#if RPL_WITH_STORING
    uip_ds6_route_t *route;
#endif /* RPL_WITH_STORING */
#if RPL_WITH_NON_STORING
    rpl_ns_node_t *link;
#endif /* RPL_WITH_NON_STORING */
    PRINTF("--- Network status ---\n");
    PRINTF("uip_lladdr: ");
    for(i = 0; i < sizeof(uip_lladdr); i++)
        PRINTF(" %02hhX", *(((uint8_t *)&uip_lladdr) + i));
    PRINTF("\n");

    PRINTF("linkaddr_node_addr: ");
    for(i = 0; i < sizeof(linkaddr_node_addr); i++)
        PRINTF(" %02hhX", *(((uint8_t *)&linkaddr_node_addr) + i));
    PRINTF("\n");

    /* Our IPv6 addresses */
    PRINTF("- Server IPv6 addresses:\n");
    for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
        state = uip_ds6_if.addr_list[i].state;
        if(uip_ds6_if.addr_list[i].isused && (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {
            PRINTF("-- ");
            PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
            PRINTF("\n");
        }
    }

    /* Our default route */
    PRINTF("- Default route:\n");
    default_route = uip_ds6_defrt_lookup(uip_ds6_defrt_choose());
    if(default_route != NULL) {
        PRINTF("-- ");
        PRINT6ADDR(&default_route->ipaddr);
        PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)default_route->lifetime.interval);
    } else {
        PRINTF("-- None\n");
    }

#if RPL_WITH_STORING
    /* Our routing entries */
    PRINTF("- Routing entries (%u in total):\n", uip_ds6_route_num_routes());
    route = uip_ds6_route_head();
    while(route != NULL) {
        PRINTF("-- ");
        PRINT6ADDR(&route->ipaddr);
        PRINTF(" via ");
        PRINT6ADDR(uip_ds6_route_nexthop(route));
        PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)route->state.lifetime);
        route = uip_ds6_route_next(route);
    }
#endif

#if RPL_WITH_NON_STORING
  /* Our routing links */
    PRINTF("- Routing links (%u in total):\n", rpl_ns_num_nodes());
    link = rpl_ns_node_head();
    while(link != NULL) {
        uip_ipaddr_t child_ipaddr;
        uip_ipaddr_t parent_ipaddr;
        rpl_ns_get_node_global_addr(&child_ipaddr, link);
        rpl_ns_get_node_global_addr(&parent_ipaddr, link->parent);
        PRINTF("-- ");
        PRINT6ADDR(&child_ipaddr);
        if(link->parent == NULL) {
            memset(&parent_ipaddr, 0, sizeof(parent_ipaddr));
            PRINTF(" --- DODAG root ");
        } else {
            PRINTF(" to ");
            PRINT6ADDR(&parent_ipaddr);
        }
        PRINTF(" (lifetime: %lu seconds)\n", (unsigned long)link->lifetime);
        link = rpl_ns_node_next(link);
    }
#endif

    tsch_schedule_print_names();

    PRINTF("----------------------\n");
}
/*---------------------------------------------------------------------------*/
static void
net_init(uip_ipaddr_t *br_prefix)
{
    uip_ipaddr_t global_ipaddr;

    if(br_prefix) { /* We are RPL root. Will be set automatically
                     as TSCH pan coordinator via the tsch-rpl module */
        memcpy(&global_ipaddr, br_prefix, 16);
        uip_ds6_set_addr_iid(&global_ipaddr, &uip_lladdr);
        uip_ds6_addr_add(&global_ipaddr, 0, ADDR_AUTOCONF);
        rpl_set_root(RPL_DEFAULT_INSTANCE, &global_ipaddr);
        rpl_set_prefix(rpl_get_any_dag(), br_prefix, 64);
        rpl_repair_root(RPL_DEFAULT_INSTANCE);
    }
    NETSTACK_MAC.on();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(node_process, ev, data)
{
    static struct etimer et;
    PROCESS_BEGIN();

  /* 3 possible roles:
   * - role_6ln: simple node, will join any network, secured or not
   * - role_6dr: DAG root, will advertise (unsecured) beacons
   * - role_6dr_sec: DAG root, will advertise secured beacons
   * */
    static int is_coordinator = 0;
    static enum { role_6ln, role_6dr, role_6dr_sec } node_role;
    node_role = role_6ln;
    static int coordinator_candidate = 0;
#ifdef CONTIKI_TARGET_Z1
    /* Set node with MAC address c1:0c:00:00:00:00:01 as coordinator,
     * convenient in cooja for regression tests using z1 nodes
     * */
      extern unsigned char node_mac[8];
      unsigned char coordinator_mac[8] = { 0xc1, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18 };

      coordinator_candidate = (memcmp(node_mac, coordinator_mac, 8) == 0);
#elif CONTIKI_TARGET_COOJA
    coordinator_candidate = (node_id == DODAG_ROOT_ID);
#endif
    if (coordinator_candidate) {
        if (LLSEC802154_ENABLED) {
            node_role = role_6dr_sec;
        } else {
            node_role = role_6dr;
        }
    } else {
        node_role = role_6ln;
    }

#if CONFIG_VIA_BUTTON
    {
#define CONFIG_WAIT_TIME 5

      SENSORS_ACTIVATE(button_sensor);
      etimer_set(&et, CLOCK_SECOND * CONFIG_WAIT_TIME);

      while(!etimer_expired(&et)) {
        printf("Init: current role: %s. Will start in %u seconds. Press user button to toggle mode.\n",
               node_role == role_6ln ? "6ln" : (node_role == role_6dr) ? "6dr" : "6dr-sec",
               CONFIG_WAIT_TIME);
        PROCESS_WAIT_EVENT_UNTIL(((ev == sensors_event) &&
                                  (data == &button_sensor) && button_sensor.value(0) > 0)
                                 || etimer_expired(&et));
        if(ev == sensors_event && data == &button_sensor && button_sensor.value(0) > 0) {
          node_role = (node_role + 1) % 3;
          if(LLSEC802154_ENABLED == 0 && node_role == role_6dr_sec) {
            node_role = (node_role + 1) % 3;
          }
          etimer_restart(&et);
        }
      }
    }
#endif /* CONFIG_VIA_BUTTON */

    PRINTF("Init: node starting with role %s\n",
           node_role == role_6ln ? "6ln" : (node_role == role_6dr) ? "6dr" : "6dr-sec");

    tsch_set_pan_secured(LLSEC802154_ENABLED && (node_role == role_6dr_sec));
    is_coordinator = node_role > role_6ln;

    if (is_coordinator) {
        uip_ipaddr_t prefix;
        uip_ip6addr(&prefix, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
        net_init(&prefix);
    } else {
        net_init(NULL);
    }

#if WITH_ORCHESTRA
    orchestra_init();
#endif /* WITH_ORCHESTRA */
#if WITH_STATIC_SCHEDULER
    static_scheduler_init();
    static_scheduler_print();
#endif /* WITH_STATIC_SCHEDULER */

#if WITH_SEQUENCE_NUMBER
    sequence_number_init();
#endif /* WITH_SEQUENCE_NUMBER */

#if WITH_PACKET_REPLICATION_AND_ELIMINATION
    pre_history_init();
#endif /* WITH_PACKET_REPLICATION_AND_ELIMINATION */


    /* Print out routing tables every minute */
    etimer_set(&et, CLOCK_SECOND * 60);
    while (1) {
        print_network_status();
        if (rpl_get_default_instance() != NULL) {
            break;
        }
        PROCESS_YIELD_UNTIL(etimer_expired(&et));
        etimer_reset(&et);
    }
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
static void
receiver(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

	char *appdata;
	if(uip_newdata()) {
	    appdata = (char *)uip_appdata;
	    appdata[uip_datalen()] = 0;
	    PRINTF("DATA recv '%s' from ", appdata);
        PRINTF("%d", UIP_IP_BUF->srcipaddr.u8[sizeof(UIP_IP_BUF->srcipaddr.u8) - 1]);
        PRINTF("\n");
	  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_sender_process, ev, data)
{
    static struct etimer rpl_wait_timer;
    static struct etimer periodic_timer;

    uint8_t sender_node_index;
    static uint8_t is_source = 0 , dodag_waiting_time = 0, first_packet_tx = 1;
    uip_ipaddr_t addr;
    char buf[20];
    static uint16_t message_number = 1;
    struct tsch_asn_divisor_t mod_timeslot_count;
    TSCH_ASN_DIVISOR_INIT(mod_timeslot_count, STATIC_SCHEDULER_TIMESLOT_COUNT);
    static uint16_t is_synchronous = 1, current_timeslot = 0;

    PROCESS_BEGIN();

    while(1) {
        etimer_set(&rpl_wait_timer, WAIT_FOR_RPL_INTERVAL);
        while (1) { /* wait until connected to RPL instance and DODAG */
            PROCESS_WAIT_EVENT();
            if (etimer_expired(&rpl_wait_timer)) {
                if (rpl_get_default_instance() != NULL && rpl_get_default_instance()->current_dag !=NULL) { /* connected to RPL instance and DODAG: start sending packets */
                    break;
                } else {
#ifdef CONTIKI_TARGET_COOJA
                    for (sender_node_index = 0; sender_node_index < SENDER_NODE_ARRAY_COUNT; sender_node_index++) {
                        if (node_id == sender_node_array[sender_node_index]) {
                            PRINTF("RPL Node: RPL connection not yet achieved. Waiting another %lu ticks\n", WAIT_FOR_RPL_INTERVAL);
                            etimer_reset(&rpl_wait_timer);
                            is_source = 1;
                            break;
                        }
                    }
                    if (!is_source) {
                        break;
                    }
#elif CONTIKI_TARGET_Z1
                    etimer_reset(&rpl_wait_timer);
#endif
                }
            }
        }

        uip_ip6addr(&addr, UIP_DS6_DEFAULT_PREFIX, 0x0000, 0x0000, 0x0000, 0x0200, 0x0000, 0x0000, 0x0000);

        simple_udp_register(&unicast_connection, UDP_PORT,NULL, UDP_PORT, receiver);


        /* missing implementation for Z1*/
        /*                              */
        /*    insert implementation     */
        /*            for               */
        /*         is_source            */


        if (!is_source) {
            PRINTF("RPL Node: RPL connection achieved\n");
            break;
        } else {
            PRINTF("RPL Node: RPL connection achieved. Starting to send packets in %lu ticks\n", START_SEND_INTERVAL);

            etimer_set(&periodic_timer, START_SEND_INTERVAL); /* start sending in SEND_INTERVAL */
            while (1) {
                if (rpl_get_default_instance() == NULL) {
                    PRINTF("RPL Node lost its instances \n");
                    break;
                }

                if (rpl_get_default_instance()->current_dag == NULL) {
                    PRINTF("RPL Node does not belong to a DODAG, waiting for reincorporation\n");
                    if(!dodag_waiting_time) {
                        etimer_set(&periodic_timer, WAIT_FOR_RPL_INTERVAL); /* start sending in SEND_INTERVAL */
                        dodag_waiting_time = 1;
                    }
                    PROCESS_WAIT_EVENT();
                    etimer_reset(&periodic_timer);
                    continue;
                }



                PROCESS_WAIT_EVENT();
                if (etimer_expired(&periodic_timer)) {
                    current_timeslot = TSCH_ASN_MOD(tsch_current_asn, mod_timeslot_count);
                    if (current_timeslot < 33) {
                        unique_id_new_l3_packet();
                        uip_ip6addr_copy(&addr, &rpl_get_default_instance()->current_dag->dag_id);
                        PRINTF("Sending data Hello %i", message_number);
                        unique_id_print_spaced_packet();
                        PRINTF("\n");
                        sprintf(buf, "Hello %d", message_number);
                        simple_udp_sendto(&unicast_connection, buf, strlen(buf) + 1, &addr);
                        message_number++;
                        if (!is_synchronous){
                            is_synchronous = 1;
                            etimer_set(&periodic_timer, SEND_INTERVAL);
                        } else {
                            etimer_reset(&periodic_timer);
                        }

                    } else {
                        int diff_timeslots = STATIC_SCHEDULER_TIMESLOT_COUNT - current_timeslot;
                        etimer_set(&periodic_timer, diff_timeslots*CLOCK_SECOND/100);
                        is_synchronous = 0;
                    }

                }

                if (rpl_get_default_instance()->current_dag != NULL) {
                    if (dodag_waiting_time || first_packet_tx) {
                        first_packet_tx = 0;
                        dodag_waiting_time = 0;
                        etimer_set(&periodic_timer, SEND_INTERVAL);
                    }
                }
            }
        }
    }
    PROCESS_END();
}
