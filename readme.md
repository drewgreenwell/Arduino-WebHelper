Arduino WebHelper
=================

This is a library for easily handling incoming web requests on the Arduino platform. It is intended to get you up and running quickly by handling the request parsing so you can focus on what you want to do.

This has been tested on the Arduino MKR WiFi 1010 but should work with any network interface based on Client.

## Basic Usage
```c++
  WiFiClient client = server.available(); // or ethernet client etc.
  if (client) {
    while (client.connected()) {      
      Serial.println("new client connected");
      if (client.available()) {
       RequestInfo request = WebHelper::parseRequest(client);
       Serial.println(request.requestMethod);
       Serial.println(request.url);
       Serial.println(request.host);
       Serial.println(request.userAgent);
       Serial.println(request.authType);
       Serial.println(request.authCredential);
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
```

## Example GET Output
```
    requestMethod: GET
    rawUrl: /my-url/?with-a-query=1
    url: /my-url/
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
    connection: keep-alive
    body:
```

## Example POST Output
```
    requestMethod: POST
    rawUrl: /my-url/?with-a-query=1
    url: /my-url/
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
    connection: 
    body: {"oi": "hello"}

```

