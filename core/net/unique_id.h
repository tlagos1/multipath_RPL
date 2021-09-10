#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

#include "contiki.h"


typedef uint32_t l3id_t;
typedef uint32_t l2id_t;

struct packet_id {
    l3id_t l3_id;
    l2id_t l2_id;
};

typedef struct packet_id packet_id_t;

void unique_id_new_l3_packet();
void unique_id_new_l2_packet();
packet_id_t unique_id_get_packet_id();
//void unique_id_print_packet();
void unique_id_print_spaced_packet();
void unique_id_print_spaced_given_packet(packet_id_t* packet_id);
uint8_t unique_id_is_l3_only();

#endif //UNIQUE_ID_H
