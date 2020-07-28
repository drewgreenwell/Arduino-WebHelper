Arduino WebHelper
=================

This is a library for easily handling incoming web requests and routing them on the Arduino platform. It is intended to get you up and running quickly by handling the request parsing so you can focus on what you want to do.

Arduino WebHelper has been tested on the Arduino MKR WiFi 1010 but should work with any network interface based on Client.

## Basic Usage

*from basic sketch in examples*

This example parses a request, prints the information to serial and responds with HTML. In this example you would would look at the request object and determine what code you needed to execute. 

> For more advanecd Route handling see the Route handling example. 

```c++
#include <WebHelper.h>

...

void loop() {
    WiFiClient client = server.available(); // or ethernet client etc.
    if (client) {
      while (client.connected()) {      
        Serial.println("new client connected");
        if (client.available()) {
        // get details about the current request
        RequestInfo request = WebHelper::parseRequest(client);
        // print the details to serial
        printRequestInfo(Serial, request);
        // respond to the client
        char html[] = "<html><body>Success</body></html>";
        // StatusCode, char * contentType, char * headers, char * body, Client, TokenHandler
        WebHelper::respondWith(StatusCode::OK, "text/html", "", html, client, NULL);
        break;
        }
      }
      client.stop();
      Serial.println("client disconnected");
    }
  }

  printRequestInfo(RequestInfo & request) {
    Serial.println("requestMethod: " + request.requestMethod);
    Serial.println("url: " + request.url);
    Serial.println("path: " + request.path);
    Serial.println("query: " + request.query);
    Serial.println("userAgent: " + request.userAgent);
    Serial.println("host: " + request.host);
    Serial.print("contentLength: ");
    Serial.println(request.contentLength);
    Serial.println("contentType: " + request.contentType);
    Serial.println("authType: " + request.authType);
    Serial.println("authCredential: " + request.authCredential);
    Serial.println("connection: " + request.connection);
    Serial.println("accept: " + request.accept);
    Serial.println("acceptLanguage: " + request.acceptLanguage);
    Serial.println("acceptEncodingethod: " + request.acceptEncoding);
    Serial.println("body: " + request.body); 
  }

```

## Example GET Output
```
    requestMethod: GET
    url: /my-url/?with-a-query=1
    path: /my-url/
    query: ?with-a-query=1
    userAgent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:77.0) Gecko/20100101 Firefox/77.0
    host: 192.168.1.123
    contentLength: -1
    contentType: 
    authType: 
    authCredential: 
    connection: keep-alive
    accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
    acceptLanguage: en-US,en;q=0.5
    acceptEncodingethod: gzip, deflate
    body:
```

## Example POST Output
```
    requestMethod: POST
    url: /my-url/?with-a-query=1
    path: /my-url/
    query: ?with-a-query=1
    userAgent: Mozilla/5.0 (Windows NT; Windows NT 10.0; en-US) WindowsPowerShell/5.1.18362.752
    host: 192.168.1.123
    contentLength: 15
    contentType: application/json
    authType: Bearer
    authCredential: my-api-key
    connection: 
    accept: 
    acceptLanguage: 
    acceptEncodingethod: 
    body: {"oi": "hello"}

```

## Route Handling

*from route-handling sketch in examples*

```c++
Route routes[ROUTE_SIZE];
...
void setup() {
  // for all route matching options see WebHelper.h
  routes[0] = { .route = "/favicon.ico", .handler = handleFavicon, .matchType = RouteMatchType::FULL };
  routes[1] = { .route = "/dashboard", .handler = handleDashboardRoute, .matchType = RouteMatchType::STARTS_WITH };
  routes[2] = { .route = "/command?task=restart", .handler = handleGenericRoute, .matchType = RouteMatchType::FULL };
  ...
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

void handleFavicon(RequestInfo &request, Client &client) {
  Serial.println("handleFavicon called");
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
/* 
  If a TokenHandler is passed to respondWith, any time a string is encountered with 
  a matching START_TOKEN and END_TOKEN the token value is passed to the TokenHandler
  The default values for START_TOKEN and END_TOKEN can be overidden with a #define.
  #define START_TOKEN "{-{"
  #define END_TOKEN "}-}"
*/ 
void tokenHandler(String &token, Client &client) {
  Serial.print("tokenHandler called: ");
  Serial.println(token);
  if(token == "TITLE") {
    token = "Handled Route | Arduino";
  } else if (token == "BODY") {
    // run a process, insert some JSON or some html
    token = "some body content";
  }
}
```
