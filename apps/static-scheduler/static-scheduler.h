#ifndef __STATIC_SCHEDULER_H__
#define __STATIC_SCHEDULER_H__

#include "net/mac/tsch/tsch.h"
#include "net/mac/tsch/tsch-conf.h"
#include "net/mac/tsch/tsch-schedule.h"
#include "static-scheduler-conf.h"


/* Call from application to start static-scheduler */
void static_scheduler_init(void);
/* Callbacks required for static-scheduler to operate */
/* Set with #define SICSLOWPAN_CALLBACK_SET_PACKET_ATTR_ADDR static_scheduler_sicslowpan_callback_set_packet_attr_addr */
void static_scheduler_sicslowpan_callback_set_packet_attr_addr();
/* Set with #define TSCH_CALLBACK_EB_SET_PACKET_ATTR_ADDR static_scheduler_tsch_callback_eb_set_packet_attr_addr */
void static_scheduler_tsch_callback_eb_set_packet_attr_addr();
/* check if the link local addresses belong in the scheduling */
uint16_t static_scheduler_unicast_slot_exists(const linkaddr_t *packet_l2_src, const linkaddr_t *packet_l2_dest);

/* Set with #define TSCH_CALLBACK_QUEUE_SHOULD_USE_STRICT_TIMESLOT_MATCH static_scheduler_tsch_queue_should_use_strict_timeslot_match */
uint8_t static_scheduler_tsch_queue_should_use_strict_timeslot_match(struct tsch_link* l);

/* Set with #define TSCH_CONF_CALLBACK_TSCH_PACKET_READY_OR_RETX static_scheduler_callback_tsch_packet_ready_or_retx */
/* Called by TSCH every time a tsch packet is ready to be added to the send queue or retransmitted */
void static_scheduler_callback_tsch_packet_ready_or_retx(struct tsch_packet*p);
/* Set with #define TSCH_CALLBACK_NEW_TIME_SOURCE static_scheduler_callback_new_time_source */
void static_scheduler_callback_new_time_source(const struct tsch_neighbor *old, const struct tsch_neighbor *new);
/* Set with #define NETSTACK_CONF_ROUTING_NEIGHBOR_ADDED_CALLBACK static_scheduler_callback_child_added */
void static_scheduler_callback_child_added(const linkaddr_t *addr);
/* Set with #define NETSTACK_CONF_ROUTING_NEIGHBOR_REMOVED_CALLBACK static_scheduler_callback_child_removed */
void static_scheduler_callback_child_removed(const linkaddr_t *addr);

/* Print out the current schedule */
void static_scheduler_print();

#endif /* __STATIC_SCHEDULER_H__ */
