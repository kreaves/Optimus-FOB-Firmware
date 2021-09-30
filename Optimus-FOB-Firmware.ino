/* k. reaves with contributions from j. melton
 *
 *
 * This is a merging of the UDP client, sensing, and an addition of scenario logic.
 */


/* // Secrets doc is pasted below
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
*/

#include "secrets.h"
#include "src/udp_pkt/udp_pkt_build.h"
#include "src/udp_pkt/udp_pkt_parse.h"

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

String ssid_str = String(SECRET_SSID); //Network name (SSID), ascii string
String pass_str = String(SECRET_PASS); //password from shared secret, ascii string
unsigned short devid_str = (unsigned short)DEVICE_ID_NUMBER; //Device id
int wifi_status = WL_IDLE_STATUS; //This variable contains the response from any commands to the WiFi subsystem.
int udp_recv_port = (int)UDP_PORT_TO_LISTEN; //This variable holds the UDP port that the device will listen to for packets.
int udp_xmit_port = (int)UDP_PORT_TO_TRANSM; //This variable holds the UDP port number that the device will talk on.

WiFiUDP udp; // This variable sets the wifi status to UDP

IPAddress dest_ip_addr(CLIENT_IP_ADDRESS); // This is where the packets will be sent to


String Scenario = "0";

void setup() {
 // Initialize pins

  //Initialize serial and wait for port to open:
  Serial.begin(19200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Add number to network name
  ssid_str += String(devid_str, DEC);

  // Display the banner
  Serial.print("UDP server for ");
  Serial.println(ssid_str);

  // Verify that there is WiFi
  if (WiFi.status() == WL_NO_MODULE)
  {
    // No Wifi detected so halt
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  // Create the network
  char ssid[SSID_BUFF_SIZE] = { 0 };
  char pass[PASS_BUFF_SIZE] = { 0 };
  ssid_str.toCharArray(ssid, (unsigned int)SSID_BUFF_SIZE);
  pass_str.toCharArray(pass, (unsigned int)PASS_BUFF_SIZE);
  wifi_status = WiFi.beginAP(ssid, pass);
  if (wifi_status != WL_AP_LISTENING)
  {
    // Failed to create the network so halt
    Serial.println("Creating access point failed");
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // Show WiFi information
  ap_wifi_status();

  Serial.println("Network ready for connections");

  // Start UDP server
  udp.begin(udp_recv_port);

  Scenario = "0";
}

void loop() {

  wifi_status_update(); // Update WiFi

  String priorScenario = Scenario;
  udp_recv_pkt();  // Recv packets
  if (Scenario != priorScenario)
  {
    if (Scenario == "0")
    {
      Serial.print("Scenario ");
      Serial.print(priorScenario);
      Serial.println(" completed successfully, returning to Scenario 0.");
    }
    else
    {
      Serial.print("The client has had a serial command issued to change scenarios from ");
      Serial.print(priorScenario);
      Serial.print(" to ");
      Serial.println(Scenario);
    }
  }

  // Handle serial commands
  serial_cmd_recv();
}

/// @brief
/// Main serial data handler
///
/// @details
/// This function handles the serial port that is connected to the computer.
/// Via this interface, commands may be sent to an attached client. These
/// serial commands need to be sent with a trailing '\n' character.
///
/// Current commands
///
/// Command: '?'
///   Usage: ?
/// Example: "?"
/// This command displays the most recent scenario reported by the client.
///
/// Command: 'c'
///   Usage: c <scenario desired>
/// Example: "c 2a"
/// This command will send a request to the client to start running the
/// supplied scenario. In the above example, will request the client to start
/// running the "2a" scenario.
///
/// Command: 'z'
///   Usage: z
/// Example: "z"
/// This command will send a request to the client to switch back to scenario
/// 0.
void serial_cmd_recv(void)
{
  if (0 < Serial.available())
  {
    String temp = Serial.readStringUntil('\n');
    temp.trim();

    // Command '?'
    String cmd = String("?");
    if (0 == cmd.compareTo(temp.substring(0,1)))
    {
      Serial.print("Currently in scenario ");
      Serial.println(Scenario);
    }

    // Command 'c'
    cmd = String("c");
    if (0 == cmd.compareTo(temp.substring(0,1)))
    {
      Serial.print("Requesting scenario: ");
      Serial.println(temp.substring(2, temp.length()));
      cmd = udp_pkt_build_scenario_pkt(
                      temp.substring(2, temp.length()), (int)DEVICE_ID_NUMBER);
      udp_xmit_pkt(cmd);
    }

    // Command 'z'
    cmd = String("z");
    if (0 == cmd.compareTo(temp.substring(0,1)))
    {
      cmd = udp_pkt_build_scenario_pkt(String("0"), (int)DEVICE_ID_NUMBER);
      udp_xmit_pkt(cmd);
    }
  }
}

/// @brief
/// UDP RX packet handler
///
/// @details
/// This function handles the reception of UDP packets.
void udp_recv_pkt(void)
{
  int pkt_size = udp.parsePacket();

  if (pkt_size)
  {
    char pkt_buf[UDP_RX_PACKET_SIZE_BYTES] = { 0 };
    int pkt_len = 0;

    while (pkt_len < pkt_size)
    {
      pkt_len += udp.read(&pkt_buf[pkt_len], ((int)UDP_RX_PACKET_SIZE_BYTES - pkt_len));
    }

    if (pkt_len > 0)
    {
      pkt_buf[pkt_len] = '\0';

      StaticJsonDocument<JSON_MEMORY_SIZE_BYTES> doc;
      DeserializationError err = deserializeJson(doc, pkt_buf);

      // Only operate on the packet if it is valid
      if (DeserializationError::Ok == err)
      {
//#if defined(DEBUG_DISPLAYS)
        serializeJson(doc, Serial);
        Serial.println("");
//#endif
        // Grab the type of packet received
        String pkt_type_str = udp_pkt_parse_get_pkt_type(doc);

        if (0 == pkt_type_str.compareTo("status"))
        {
          Scenario = udp_pkt_parse_get_scenario(doc);
#if defined(DEBUG_DISPLAYS)
          const int client_dev_id = doc["dev_id"];
          Serial.print(client_dev_id);
          Serial.print(" Status: scenario = ");
          Serial.println(Scenario);
#endif
        }
        else if (0 == pkt_type_str.compareTo("febss_cmd"))
        {
          const bool FEBSS_active = udp_pkt_parse_get_febss_state(doc);
          String febss_txt = udp_pkt_parse_get_febss_num(doc);
#if defined(DEBUG_DISPLAYS)
          const int client_dev_id = doc["dev_id"];
          Serial.print(client_dev_id);
          Serial.print(" ");
#endif
          Serial.print(febss_txt); Serial.print(": ");
          if (FEBSS_active)
          {
            Serial.println("ON");
          }
          else
          {
            Serial.println("OFF");
          }
        }
        else if (0 == pkt_type_str.compareTo("ready_status"))
        {
          const bool ready_status = udp_pkt_parse_get_ready(doc);
#if defined(DEBUG_DISPLAYS)
          const int client_dev_id = doc["dev_id"];
          Serial.print(client_dev_id);
          Serial.print(" ");
#endif
          Serial.print("Client is ");
          if (ready_status)
          {
            Serial.print("ready to receive commands");
          }
          else
          {
            Serial.print("not ready to receive commands");
          }
          Serial.println("");
        }
        else
        {
          Serial.println("Unhandled packet");
        }
      }
    }
  }
}

/// @brief
/// Xmit a UDP packet
///
/// @details
/// This function handles the transmission of a json serialized String type via
/// a UDP connection.
///
/// @param [in] packet
/// The JSON serialized packet in a String type to transmit.
void udp_xmit_pkt(String packet)
{
  char tx_buf[UDP_TX_PACKET_SIZE_BYTES] = { 0 };

  // Create the byte array for WiFi transmission
  packet.toCharArray(tx_buf, (unsigned int)UDP_TX_PACKET_SIZE_BYTES);

  // Send the packet via UDP
  udp.beginPacket(dest_ip_addr, udp_xmit_port);
  udp.write(tx_buf);
  udp.endPacket();
}

/// @brief
/// Display WiFi info
///
/// @details
/// This function prints out to the serial port information about the access point WiFi
/// network that has been created.
void wifi_status_update(void)
{
  // compare the previous status to the current status
  if (wifi_status != WiFi.status())
  {
    // it has changed update the variable
    wifi_status = WiFi.status();

    if (wifi_status == WL_AP_CONNECTED)
    {
      // a device has connected to the AP
      Serial.println("WiFi: Device connected to AP");
    }
    else
    {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("WiFi: Device disconnected from AP");
    }
  }
}

/// @brief
/// Display WiFi info
///
/// @details
/// This function displays the current WiFi information that another
/// device would need in order to connect.
void ap_wifi_status(void)
{
  // print the SSID of the network created
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("Send UDP packets to ");
  Serial.print(ip);
  Serial.print(" port ");
  Serial.println(udp_recv_port);
}
