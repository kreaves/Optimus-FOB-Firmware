///
/// @file
///
/// @details
/// This file contains the functions for building the Optimus project's UDP
/// packet traffic.
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
#include "udp_pkt_build.h"
#include "udp_pkt_defs.h"

#include <ArduinoJson.h>

//
// Local Definitions
//

/// @brief
/// Debug serial port
///
/// @details
/// This defines which serial port the debug messages will be displayed on.
#define DEBUG_PORT                        Serial

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

/// @brief
/// Display debug messages
///
/// @details
/// This local global variable indicates if the debug messages should be shown
/// or not. It is of type bool.
bool udp_pkt_build_debug = false;

//
// Code
//
String udp_pkt_build_status_pkt(String scenario, int dev_id)
{
  String packet;

  // Create the JSON packet
  StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc;
  doc["pkt_type"].set("status");
  doc["dev_id"].set(dev_id);
  doc["scenario"].set(scenario);
  serializeJson(doc, packet);

  if (udp_pkt_build_debug)
  {
    serializeJson(doc, DEBUG_PORT);
    DEBUG_PORT.println("");
  }

  return packet;
}

String udp_pkt_build_FEBSS_pkt(bool turn_on, int dev_id, String febss)
{
  String packet;

  // Create the JSON packet
  StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc;
  doc["pkt_type"].set("febss_cmd");
  doc["dev_id"].set(dev_id);
  doc["febss"].set(febss);
  doc["febss_state"].set(turn_on);

  serializeJson(doc, packet);

  if (udp_pkt_build_debug)
  {
    serializeJson(doc, DEBUG_PORT);
    DEBUG_PORT.println("");
  }

  return packet;
}

String udp_pkt_build_scenario_pkt(String scenario, int dev_id)
{
  String packet;

  // Create the JSON packet
  StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc;
  doc["pkt_type"].set("scenario_cmd");
  doc["dev_id"].set(dev_id);
  doc["scenario"].set(scenario);
  serializeJson(doc, packet);

  if (udp_pkt_build_debug)
  {
    serializeJson(doc, DEBUG_PORT);
    DEBUG_PORT.println("");
  }

  return packet;
}

String udp_pkt_build_client_ready_pkt(int dev_id)
{
  String packet;

  // Create the JSON packet
  StaticJsonDocument<UDP_PKT_JSON_MEMORY_SIZE_BYTES> doc;
  doc["pkt_type"].set("ready_status");
  doc["dev_id"].set(dev_id);
  doc["ready"].set((bool)true);
  serializeJson(doc, packet);

  if (udp_pkt_build_debug)
  {
    serializeJson(doc, DEBUG_PORT);
    DEBUG_PORT.println("");
  }

  return packet;
}
