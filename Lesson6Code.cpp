/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * Osoyoo Wifi IoT  lesson 6
 * Remote control Servo over UDP
 * tutorial url: https://osoyoo.com/?p=32612
 */

#include "WiFiEsp.h"
#include "WiFiEspUdp.h"
#include "SoftwareSerial.h"

#include <Servo.h> 
Servo servo_obj;  // create servo object to control a servo 
#define servo_pin 9
int angle=90;

SoftwareSerial softserial(4, 5); // A9 to ESP_TX, A8 to ESP_RX by default
char ssid[] = "***";            // your network SSID (name)
char pass[] = "***";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiEspUDP Udp;
unsigned int localPort = 8888;              // local port to listen on
char packetBuffer[5];      
 
void setup() {
  servo_obj.attach(servo_pin);
  servo_obj.write(90);
  delay (1000);
  Serial.begin(9600);   // initialize serial for debugging
    softserial.begin(115200);
  softserial.write("AT+CIOBAUD=9600\r\n");
  softserial.write("AT+RST\r\n");
  softserial.begin(9600);    // initialize serial for ESP module
  WiFi.init(&softserial);    // initialize ESP module

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();
 
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  
  Serial.print(" target port ");
  Serial.println(localPort);
     
}
 
void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {                               // if you get a client,
     Serial.print("Received packet of size ");
    Serial.println(packetSize);
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
      char c=packetBuffer[0];
      switch (c)    //serial control instructions
      {  
 
        case 'A': angle=angle+5 ;break; //▲ button pressed, rotate 5 degree counterclockwise
        case 'B': angle=angle-5 ;break; //▼ button pressed, rotate 5 degree clockwise
        case 'L': angle=180 ;break; //< button pressed, rotate to 180 degree position
        case 'R': angle=0 ;break; //> button pressed, rotate to 0 degree position
        case 'E': angle=90 ;break; //SQUARE button pressed, rotate to 90 degree position
 
        default:break;
      }
      if (angle<0) angle=0;
      if (angle>180) angle=180;
       servo_obj.write(angle);
    }
}
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("please set your UDP APP target IP to: ");
  Serial.print(ip);
 
}
