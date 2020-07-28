/*
  WiFi Web Server

 A simple web server that shows the value of the analog input pins.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe

 Modified 07/20/2020
 by Drew Greenwell

 */
#include <SPI.h>
#include <Client.h>
#include <WiFiNINA.h>
#include <pgmStrToRAM.h>
#include <WebHelper.h>
#include "arduino_secrets.h" 
#include "html_template.h" 

#define ROUTE_SIZE 3
#define DEFAULT_HEADERS "Server: Arduino\r\nConnection: Close"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;


WiFiServer server(80);
Route routes[ROUTE_SIZE];

void setup() {
  // for all route matching options see WebHelper.h
  routes[0] = { .route = "/my-url/?task=doSomething", .handler = handleGenericRoute, .matchType = RouteMatchType::FULL };
  routes[1] = { .route = "/dashboard", .handler = handleDashboardRoute, .matchType = RouteMatchType::STARTS_WITH };
  routes[2] = { .route = "/favicon.ico", .handler = handleFavicon, .matchType = RouteMatchType::FULL };
  //Initialize serial and wait for port to open:
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

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();
  server.begin();
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {      
      if (client.available()) {
       RequestInfo request = WebHelper::parseRequest(client);
       printRequestInfo(Serial, request);
       boolean handled = WebHelper::handleRoutes(routes, ROUTE_SIZE, request, client);
       if(!handled) {
          WebHelper::respondWith(StatusCode::NOT_FOUND, "text/html", DEFAULT_HEADERS, "Page Not Found", client, NULL);
       }
       break;
      }      
    }
    client.stop();
    Serial.println("client disconnected");
  }
}


template<class T>
void printRequestInfo(T &printer, RequestInfo request) {
   printer.println("requestMethod: " + request.requestMethod);
   printer.println("url: " + request.url);
   printer.println("path: " + request.path);
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

void handleFavicon(RequestInfo &request, Client &client) {
  WebHelper::respondWith(StatusCode::FORBIDDEN, "text/html", DEFAULT_HEADERS, "No favicon for you", client, NULL);
}


void handleDashboardRoute(RequestInfo &request, Client &client) {
  Serial.println("handleDashboardRoute called");
  WebHelper::respondWith(StatusCode::OK, "text/html", DEFAULT_HEADERS, DASHBOARD_TEMPLATE, client, tokenHandler);
}

void handleGenericRoute(RequestInfo &request, Client &client) {
  Serial.println("handleGenericRoute called");
  WebHelper::respondWith(StatusCode::OK, "text/html", DEFAULT_HEADERS, "A handled route", client, tokenHandler);
}

void tokenHandler(String &token, Client &client) {
  Serial.print("tokenHandler called: ");
  Serial.println(token);
  if(token == "TITLE") {
    token = "Handled Route | Arduino";
  } else if (token == "BODY") {
    token = "some body content";
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
