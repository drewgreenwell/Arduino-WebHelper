/*
  basic example based on WifiWebServer example from WiFiNINA
*/
#include <SPI.h>
#include <WiFiNINA.h>
#include <WebHelper.h>
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  // setup code for network interface and serial for debug
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:    
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
   WiFiClient client = server.available(); // or ethernet client
  if (client) {
    while (client.connected()) {
      Serial.println("new client connected");
      if (client.available()) {
       RequestInfo request = WebHelper::parseRequest(client);
       Serial.println(request.requestMethod);
       Serial.println(request.url);
       Serial.println(request.userAgent);
       Serial.println(request.host);
       Serial.println(info.authType);
       Serial.println(info.authCredential);
       Serial.println(request.contentLength);
       Serial.println(request.contentType);
       Serial.println(request.body);
       client.println("HTTP/1.1 200 OK");
       client.println("Content-type:text/html");
       client.println();
       client.println("<html><body>Success</body></html>");
       client.println();
       break;
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
