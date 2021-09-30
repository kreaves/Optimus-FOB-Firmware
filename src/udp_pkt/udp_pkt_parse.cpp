///
/// @file
///
/// @details
/// This file contains the functions for parsing the Optimus project's UDP
/// packets.
///
/// @copyright
/// Copyright 2021 SynDaver as unpublished work. All rights reserved.
///
/// The information contained herein is confidential property of SynDaver. The
/// use, copying, transfer or disclosure of such information is prohibited
/// except by express written agreement with SynDaver.

//
// Local Includes
//
#include "udp_pkt_parse.h"

//
// Local Definitions
//

//
// Local Structures & Enumerations
//

//
// Local Typedefs
//

//
// Local Function Prototypes
//

//
// Local Global Variables
//

//
// Code
//
String udp_pkt_parse_get_pkt_type(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc)
{
    const char * pkt_type_char_array = doc["pkt_type"];

    return String(pkt_type_char_array);
}

int udp_pkt_parse_get_dev_id(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc)
{
    int dev_id = doc["dev_id"];

    return dev_id;
}

String udp_pkt_parse_get_scenario(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc)
{
    const char * scenario_requested_char_array = doc["scenario"];

    return String(scenario_requested_char_array);
}

String udp_pkt_parse_get_febss_num(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc)
{
    const char * febss_requested_char_array = doc["febss"];

    return String(febss_requested_char_array);
}

bool udp_pkt_parse_get_febss_state(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc)
{
    bool febss_state = doc["febss_state"];

    return febss_state;
}

bool udp_pkt_parse_get_ready(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc)
{
    bool ready = doc["ready"];

    return ready;
}
