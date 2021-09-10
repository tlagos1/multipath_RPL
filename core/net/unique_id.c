#include "net/unique_id.h"
#include "contiki-net.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static packet_id_t last_packet_id = {0, 0};

void unique_id_new_l3_packet(){
    ++last_packet_id.l3_id;
    last_packet_id.l2_id = 0;
}

void unique_id_new_l2_packet(){
    ++last_packet_id.l2_id;
}

packet_id_t unique_id_get_packet_id(){
    return last_packet_id;
}

//void unique_id_print_packet(){
//    LOG_INFO_("unique_packet %u/%u", last_packet_id.l3_id, last_packet_id.l2_id);
//}

void unique_id_print_spaced_packet(){
    PRINTF(" unique_packet %u/%u", last_packet_id.l3_id, last_packet_id.l2_id);
}

void unique_id_print_spaced_given_packet(packet_id_t* packet_id){
    PRINTF(" unique_packet %u/%u", packet_id->l3_id, packet_id->l2_id);
}


uint8_t unique_id_is_l3_only(){
    return last_packet_id.l2_id == 0;
}



