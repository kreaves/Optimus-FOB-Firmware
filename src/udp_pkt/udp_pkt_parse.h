///
/// @file
///
/// @copydoc udp_pkt_parse.cpp

#if !defined(OPTIMUS_UDP_PKT_PARSE_H)
#define OPTIMUS_UDP_PKT_PARSE_H

//
// Shared Includes
//
#include "udp_pkt_defs.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#include <stdbool.h>

//
// Shared Definitions
//

//
// Shared Structures & Enumerations
//

//
// Shared Typedefs
//

//
// Shared Method Definitions
//

/// @brief
/// Get packet type from JSON document
///
/// @details
/// This function grabs the packet type information from a JSON document.
///
/// @param [in] doc
/// This is the JSON document to extract the information from. It is a
/// StaticJsonDocument type.
///
/// @return
/// The value returned is the packet type value. It is of type String.
String udp_pkt_parse_get_pkt_type(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc);

/// @brief
/// Get device ID from JSON document
///
/// @details
/// This function grabs the device ID information from a JSON document.
///
/// @param [in] doc
/// This is the JSON document to extract the information from. It is a
/// StaticJsonDocument type.
///
/// @return
/// The value returned is the device ID value. It is of type int.
int udp_pkt_parse_get_dev_id(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc);

/// @brief
/// Get scenario from JSON document
///
/// @details
/// This function grabs the scenario information from a JSON document.
///
/// @param [in] doc
/// This is the JSON document to extract the information from. It is a
/// StaticJsonDocument type.
///
/// @return
/// The value returned is the scenario value. It is of type String.
String udp_pkt_parse_get_scenario(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc);

/// @brief
/// Get FEBSS pin from JSON document
///
/// @details
/// This function grabs the FEBSS pin information from a JSON document.
///
/// @param [in] doc
/// This is the JSON document to extract the information from. It is a
/// StaticJsonDocument type.
///
/// @return
/// The value returned is the FEBSS pin value. It is of type String.
String udp_pkt_parse_get_febss_num(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc);

/// @brief
/// Get FEBSS state from JSON document
///
/// @details
/// This function grabs the FEBSS state information from a JSON document.
///
/// @param [in] doc
/// This is the JSON document to extract the information from. It is a
/// StaticJsonDocument type.
///
/// @return
/// The value returned is the FEBBS state. It is of type bool.
///
/// @retval false
/// The FEBSS pin should be turned off.
///
/// @retval true
/// The FEBSS pin should be turned on.
bool udp_pkt_parse_get_febss_state(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc);

/// @brief
/// Get client ready status from a JSON document
///
/// @details
/// This function grabs the client's ready status information from a JSON
/// document.
///
/// @param [in] doc
/// This is the JSON document to extract the information from. It is a
/// StaticJsonDocument type.
///
/// @return
/// The value returned is the client's ready status. It is of type bool.
///
/// @retval false
/// The client is not ready to receive commands.
///
/// @retval true
/// The client is ready to receive commands.
bool udp_pkt_parse_get_ready(
    StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc);

//
// Shared Global Variables
//

#endif // OPTIMUS_UDP_PKT_PARSE_H
