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
  /* output
    POST
    /my-url/?with-a-query=1
    192.168.1.123
    Mozilla/5.0 (Windows NT; Windows NT 10.0; en-US) WindowsPowerShell/5.1.18362.752
    Basic
    ApiKey
    15
    application/json
    {"oi": "hello"}  
   */

```

