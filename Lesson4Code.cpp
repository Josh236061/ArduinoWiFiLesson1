/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * Osoyoo W5100 web server lesson 2
 * Send DHT11 temperature data to remote browser
 * tutorial url: https://osoyoo.com/?p=9988
 */
#include <SPI.h> 
#include<dht.h>
#define DHT11_PIN 2
dht DHT;

#include "WiFiEsp.h"
 
//#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial softserial(4, 5); // RX, TX
//#endif

char ssid[] = "NETGEAR00";            // your network SSID (name)
char pass[] = "weijianweiye";        // your network password
int status = WL_IDLE_STATUS;
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);
String readString; 

void setup(){
  Serial.begin(9600);
  pinMode(DHT11_PIN,INPUT);
  // initialize serial for ESP module
    softserial.begin(115200);
  softserial.write("AT+CIOBAUD=9600\r\n");
  softserial.write("AT+RST\r\n");
  softserial.begin(9600);
  // initialize ESP module
  WiFi.init(&softserial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

void loop(){ 
  // Create a client connection 
  WiFiEspClient client = server.available();
  if (client) { 
    while (client.connected()) { 
      if (client.available()) { 
        char c = client.read();
        //read char by char HTTP request 
        while (readString.length() < 100 && c != 0x0D  ) {
        //store characters to string 
        readString += c; 
        c = client.read();
      }
      Serial.println(readString);
      int chk = DHT.read11(DHT11_PIN);
      Serial. println (DHT.temperature);
      Serial. println (DHT.humidity);
      String msg="HTTP/1.1 200 OK\n\rContent-Type: text/html\n\r\n\r";
      msg +="<HTML><meta http-equiv=\"refresh\" content=\"20\"><BODY><H1 style=\"color:green;\">Arduino IoT Lesson 4</H1><br>";
      msg +="<p>Real time temperature: ";
      msg +=  DHT.temperature;
      msg +=  " C ;real time Humidity: " ;
      msg += DHT.humidity ;
      msg += "%</p></BODY></HTML>";
      client.println(msg);
      delay(100); 
      //stopping client 
      client.stop();
      //clearing string for next read 
      readString="";
    } 
  } 
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
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
