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
#include "src/button/button_led.h"
#include "src/status/status_led.h"
#include "src/button/button_switch.h"

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

#include <stdbool.h>

String ssid_str = String(SECRET_SSID); //Network name (SSID), ascii string
String pass_str = String(SECRET_PASS); //password from shared secret, ascii string
unsigned short devid_str = (unsigned short)DEVICE_ID_NUMBER; //Device id
int wifi_status = WL_IDLE_STATUS; //This variable contains the response from any commands to the WiFi subsystem.
int udp_recv_port = (int)UDP_PORT_TO_LISTEN; //This variable holds the UDP port that the device will listen to for packets.
int udp_xmit_port = (int)UDP_PORT_TO_TRANSM; //This variable holds the UDP port number that the device will talk on.

WiFiUDP udp; // This variable sets the wifi status to UDP

bool dest_ip_addr_set = false;
IPAddress dest_ip_addr(CLIENT_IP_ADDRESS); // This is where the packets will be sent to


String Scenario = "0";

static button_switch_state_type local_switch_states[BUTTON_SWITCH_MAX] =
{
  BUTTON_SWITCH_STATE_CLOSED,
  BUTTON_SWITCH_STATE_CLOSED,
  BUTTON_SWITCH_STATE_CLOSED
};

void setup() {
  // Initialize pins
  button_led_cfg();
  button_switch_cfg();
  status_led_cfg();

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

  // wait 5 seconds for connection:
  //delay(5000);

  // Show WiFi information
  ap_wifi_status();

  Serial.println("Network ready for connections");

  // Indicate that the FOB is waiting for a network connection
  status_led_set_mode(STATUS_LED_YEL, STATUS_LED_MODE_BLINKING);

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

      button_led_set_mode(BUTTON_1_LED, LED_MODE_OFF);
      button_led_set_mode(BUTTON_2_LED, LED_MODE_OFF);
      button_led_set_mode(BUTTON_3_LED, LED_MODE_OFF);
    }
    else
    {
      Serial.print("The client has had a serial command issued to change scenarios from ");
      Serial.print(priorScenario);
      Serial.print(" to ");
      Serial.println(Scenario);

      if ("1a" == Scenario)
      {
        button_led_set_mode(BUTTON_1_LED, LED_MODE_BLINKING);
        button_led_set_mode(BUTTON_2_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_3_LED, LED_MODE_OFF);
      }
      else if ("1b" == Scenario)
      {
        button_led_set_mode(BUTTON_1_LED, LED_MODE_ON);
        button_led_set_mode(BUTTON_2_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_3_LED, LED_MODE_OFF);
      }
      else if ("2a" == Scenario)
      {
        button_led_set_mode(BUTTON_1_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_2_LED, LED_MODE_BLINKING);
        button_led_set_mode(BUTTON_3_LED, LED_MODE_OFF);
      }
      else if ("2b" == Scenario)
      {
        button_led_set_mode(BUTTON_1_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_2_LED, LED_MODE_ON);
        button_led_set_mode(BUTTON_3_LED, LED_MODE_OFF);
      }
      else if ("3a" == Scenario)
      {
        button_led_set_mode(BUTTON_1_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_2_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_3_LED, LED_MODE_BLINKING);
      }
      else if ("3b" == Scenario)
      {
        button_led_set_mode(BUTTON_1_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_2_LED, LED_MODE_OFF);
        button_led_set_mode(BUTTON_3_LED, LED_MODE_ON);
      }
      else
      {
        // For completeness
      }
    }
  }

  // Handle button 1 changes
  if (local_switch_states[0] != button_switch_state_get(BUTTON_1_SWITCH))
  {
    local_switch_states[0] = button_switch_state_get(BUTTON_1_SWITCH);
    if (BUTTON_SWITCH_STATE_CLOSED == local_switch_states[0])
    {
      String cmd;

      if ("1a" == Scenario)
      {
        cmd = udp_pkt_build_scenario_pkt(String("0"), (int)DEVICE_ID_NUMBER);
      }
      else
      {
        cmd = udp_pkt_build_scenario_pkt(String("1a"), (int)DEVICE_ID_NUMBER);
      }

      udp_xmit_pkt(cmd);
    }
  }

  // Handle button 2 changes
  if (local_switch_states[1] != button_switch_state_get(BUTTON_2_SWITCH))
  {
    local_switch_states[1] = button_switch_state_get(BUTTON_2_SWITCH);
    if (BUTTON_SWITCH_STATE_CLOSED == local_switch_states[1])
    {
      String cmd;

      if ("2a" == Scenario)
      {
        cmd = udp_pkt_build_scenario_pkt(String("0"), (int)DEVICE_ID_NUMBER);
        udp_xmit_pkt(cmd);
      }
      else
      {
        cmd = udp_pkt_build_scenario_pkt(String("2a"), (int)DEVICE_ID_NUMBER);
      }

      udp_xmit_pkt(cmd);
    }
  }

  // Handle button 3 changes
  if (local_switch_states[2] != button_switch_state_get(BUTTON_3_SWITCH))
  {
    local_switch_states[2] = button_switch_state_get(BUTTON_3_SWITCH);
    if (BUTTON_SWITCH_STATE_CLOSED == local_switch_states[2])
    {
      String cmd;

      if ("3a" == Scenario)
      {
        cmd = udp_pkt_build_scenario_pkt(String("0"), (int)DEVICE_ID_NUMBER);
        udp_xmit_pkt(cmd);
      }
      else
      {
        cmd = udp_pkt_build_scenario_pkt(String("3a"), (int)DEVICE_ID_NUMBER);
      }

      udp_xmit_pkt(cmd);
    }
  }

  // Handle serial commands
  serial_cmd_recv();

  // Handle button stuff
  button_led_action();
  button_switch_action();

  // Status LED
  status_led_action();
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

    // Command 'set'
    //           1         2
    // 01234567890123456789012345
    // set button_led # on
    // set button_led # off
    // set button_led # blink
    // set status_led c on
    cmd = String("set");
    if (0 == cmd.compareTo(temp.substring(0,3)))
    {
      cmd = String("button_led");
      if (0 == cmd.compareTo(temp.substring(4,14)))
      {
        button_leds_t led;
        if (0 == String("1").compareTo(temp.substring(15,16)))
        {
          led = BUTTON_1_LED;
        }
        else if (0 == String("2").compareTo(temp.substring(15,16)))
        {
          led = BUTTON_2_LED;
        }
        else
        {
          led = BUTTON_3_LED;
        }

        if (0 == String("on").compareTo(temp.substring(17,19)))
        {
          button_led_set_mode(led, LED_MODE_ON);
        }
        else if (0 == String("off").compareTo(temp.substring(17,20)))
        {
          button_led_set_mode(led, LED_MODE_OFF);
        }
        else if (0 == String("blink").compareTo(temp.substring(17,22)))
        {
          button_led_set_mode(led, LED_MODE_BLINKING);
        }
        else
        {
          ;
        }
      }

      //           1         2
      // 01234567890123456789012345
      // set status_led grn on
      // set status_led grn off
      // set status_led grn blink
      cmd = String("status_led");
      if (0 == cmd.compareTo(temp.substring(4,14)))
      {
        status_leds_t led;
        if (0 == String("grn").compareTo(temp.substring(15,18)))
        {
          led = STATUS_LED_GRN;
        }
        else
        {
          led = STATUS_LED_YEL;
        }

        if (0 == String("on").compareTo(temp.substring(19,21)))
        {
          status_led_set_mode(led, STATUS_LED_MODE_ON);
        }
        else if (0 == String("off").compareTo(temp.substring(19,22)))
        {
          status_led_set_mode(led, STATUS_LED_MODE_OFF);
        }
        else if (0 == String("blink").compareTo(temp.substring(19,24)))
        {
          status_led_set_mode(led, STATUS_LED_MODE_BLINKING);
        }
        else
        {
          ;
        }
      }
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
    IPAddress pkt_ip_addr;

    if (!dest_ip_addr_set)
    {
      pkt_ip_addr = udp.remoteIP();
    }

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
#if defined(DEBUG_DISPLAYS)
        serializeJson(doc, Serial);
        Serial.println("");
#endif
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
          if (!dest_ip_addr_set)
          {
            dest_ip_addr = pkt_ip_addr;
            dest_ip_addr_set = true;
          }

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

          // Indicate that the FOB has connected to a client
          status_led_set_mode(STATUS_LED_YEL, STATUS_LED_MODE_OFF);
          status_led_set_mode(STATUS_LED_GRN, STATUS_LED_MODE_ON);
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
