#ifndef __STATIC_SCHEDULER__PRIVATE_H__
#define __STATIC_SCHEDULER__PRIVATE_H__

#include "net/mac/tsch/tsch-schedule.h" /* for struct tsch_slotframe */
#include <stdint.h>

/*---------------------------------------------------------------------------*/
struct static_scheduler_l3_packet_transmission {
  uint16_t from_l3_node; // the L3 address source of the packet
  uint16_t to_l3_node; // the L3 address destination of the packet. Often the RPL root.
  uint16_t l3_packet_transmission_global; // the global 1-based index of this packet transmission (global as in considering packet transmissions in the whole DODAG)
  uint16_t l3_packet_transmission_local; // the local (for the from_l3_node) 1-based index of this packet transmission
  uint16_t l3_packet_transmission_local_count; // the local (for the from_l3_node) count of packet transmissions
};
/*---------------------------------------------------------------------------*/
typedef struct static_scheduler_l3_packet_transmission static_scheduler_l3_packet_transmission_t;
/*---------------------------------------------------------------------------*/
struct static_scheduler_tsch_cell {
  uint16_t from_node;
  uint16_t to_node;
  uint8_t overhearing;
  uint8_t type;
  static_scheduler_l3_packet_transmission_t* l3_packet_transmission;
  uint16_t packet_transmission;
  uint16_t packet_count_in_queue;
  struct tsch_link * link;
};
/*---------------------------------------------------------------------------*/
typedef struct static_scheduler_tsch_cell static_scheduler_tsch_cell_t;
/*---------------------------------------------------------------------------*/
#define STATIC_SCHEDULER_TSCH_CELL_SKIPPED    0
#define STATIC_SCHEDULER_TSCH_CELL_SHARED     1
#define STATIC_SCHEDULER_TSCH_CELL_MULTICAST  2
#define STATIC_SCHEDULER_TSCH_CELL_UNICAST    3
#define STATIC_SCHEDULER_TSCH_CELL_UNICAST_KA 4
/*---------------------------------------------------------------------------*/
extern static_scheduler_l3_packet_transmission_t static_scheduler_l3_packet_transmissions[STATIC_SCHEDULER_L3_PACKET_TRANSMISSIONS_COUNT];
extern static_scheduler_tsch_cell_t static_scheduler_schedule[STATIC_SCHEDULER_CHANNEL_COUNT][STATIC_SCHEDULER_TIMESLOT_COUNT][STATIC_SCHEDULER_CELL_ENTRIES];

#endif /* __STATIC_SCHEDULER__PRIVATE_H__ */
