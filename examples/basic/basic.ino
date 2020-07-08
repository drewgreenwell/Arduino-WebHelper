/*
  basic example based on WifiWebServer example from WiFiNINA
  see loop() for usage
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
       // get details about the current request
       RequestInfo request = WebHelper::parseRequest(client);
       // print the details to serial
       printRequestInfo(Serial, request);       
       client.println("HTTP/1.1 200 OK");
       client.println("Content-type: text/html");
       client.println();
       client.println("<html><body><h1>Results</h1><textarea>");
       // print the details to the html page
       printRequestInfo(client, request);
       client.println("</textarea></body></html>");
       client.println();
       break;
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}

template<class T>
void printRequestInfo(T printer, RequestInfo request) {
   printer.println("requestMethod: " + request.requestMethod);
   printer.println("rawUrl: " + request.rawUrl);
   printer.println("url: " + request.url);
   printer.println("query: " + request.query);
   printer.println("userAgent: " + request.userAgent);
   printer.println("host: " + request.host);
   printer.print("contentLength: ");
   printer.println(request.contentLength);
   printer.println("contentType: " + request.contentType);
   printer.println("authType: " + request.authType);
   printer.println("authCredential: " + request.authCredential);
   printer.println("connection: " + request.connection);
   printer.println("accept: " + request.accept);
   printer.println("acceptLanguage: " + request.acceptLanguage);
   printer.println("acceptEncodingethod: " + request.acceptEncoding);
   printer.println("connection: " + request.connection);
   printer.println("body: " + request.body);  
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
