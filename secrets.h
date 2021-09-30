#if !defined(SECRETS_H)
#define SECRETS_H

#define SECRET_SSID "OPTIMUS_HEM"
#define SECRET_PASS "1234567890_33634"

#define DEVICE_ID_NUMBER 0x000A

#define SSID_BUFF_SIZE  32
#define PASS_BUFF_SIZE  32

#define CLIENT_IP_ADDRESS 192, 168, 4, 2
#define SERVER_IP_ADDRESS 192, 168, 4, 1
#define UDP_PORT_TO_LISTEN 1234
#define UDP_PORT_TO_TRANSM 1234
#define UDP_RX_PACKET_SIZE_BYTES 200
#define UDP_TX_PACKET_SIZE_BYTES 200
#define STATUS_UDP_PKT_DELAY_MS 2000

#define JSON_MEMORY_SIZE_BYTES 200

/// @details
/// If the following is defined, then there will be debug messages displayed
/// while the code is running.
#undef DEBUG_DISPLAYS

#endif // SECRETS_H
