///
/// @file
///
/// @copydoc udp_pkt_build.cpp

#if !defined(OPTIMUS_UDP_PKT_BUILD_H)
#define OPTIMUS_UDP_PKT_BUILD_H

//
// Shared Includes
//
#include <Arduino.h>

#include <stdint.h>
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
/// Create status packet
///
/// @details
/// This function creates a serialized JSON packet that is to be sent via a UDP
/// connection. The packet's contents will be a status update from the device
/// to the server.
///
/// @param [in] scenario
/// This is the scenario number that will be sent to the server. It is a String
/// type.
///
/// @param [in] dev_id
/// This is the id number for the device. It is a int32_t type.
///
/// @returns
/// A type String that contains the serialized JSON to be transmitted.
String udp_pkt_build_status_pkt(String scenario, int dev_id);

/// @brief
/// Create FEBSS packet
///
/// @details
/// This function creates a serialized JSON packet that is to be sent via a UDP
/// connection. The packet's contents will be a command to tell the server to
/// either turn on or off the FEBSS.
///
/// @param [in] turn_on
/// This indicates if the FEBSS needs to be turned on (true) or off (false). It
/// is of type bool.
///
/// @param [in] dev_id
/// This is the id number for the device. It is a int32_t type.
///
/// @param [in] febss
/// This is the FEBSS that is being commanded. It is of type String.
///
/// @returns
/// A type String that contains the serialized JSON to be transmitted.
String udp_pkt_build_FEBSS_pkt(bool turn_on, int dev_id, String febss);

/// @brief
/// Create Scenario command packet
///
/// @details
/// This function creates a serialized json packet that is to be sent via a UDP
/// connection. This packet's contents will be a request for the client to
/// start a particular scenario.
///
/// @param [in] scenario
/// This is the scenario that the client is to switch to. It is a String type.
///
/// @param [in] dev_id
/// This is the id number for the device. It is a int32_t type.
///
/// @returns
/// A type String that contains the serialized JSON to be transmitted.
String udp_pkt_build_scenario_pkt(String scenario, int dev_id);

/// @brief
/// Create client ready packet
///
/// @details
/// This function creates a serialized json packet that is to be sent via a UDP
/// connection. This packet's contents will be to indicate that a client is
/// ready to receive commands.
///
/// @param [in] dev_id
/// This is the id number for the device. It is a int32_t type.
///
/// @returns
/// A type String that contains the serialized JSON to be transmitted.
String udp_pkt_build_client_ready_pkt(int dev_id);

//
// Shared Global Variables
//

#endif // OPTIMUS_UDP_PKT_BUILD_H
