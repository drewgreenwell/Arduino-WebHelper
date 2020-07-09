#include <Arduino.h>
#include <Client.h>
#include <WebHelper.h>

#define REQ_TYPE_LENGTH 8

void WebHelper::init()
{
     
}

const String WebHelper::RequestTypes[REQ_TYPE_LENGTH] = {
    "GET",
    "POST",
    "PUT",
    "PATCH",
    "DELETE",
    "OPTIONS",
    "CONNECT",
    "TRACE"
};

const char * WebHelper::REQ_USER_AGENT = "User-Agent:";
const char * WebHelper::REQ_CONTENT_TYPE = "Content-Type:";
const char * WebHelper::REQ_HOST = "Host:";
const char * WebHelper::REQ_CONTENT_LENGTH = "Content-Length:";
const char * WebHelper::REQ_AUTH = "Authorization:";
const char * WebHelper::REQ_ACCEPT = "Accept:";
const char * WebHelper::REQ_ACCEPT_ENC = "Accept-Encoding:";
const char * WebHelper::REQ_ACCEPT_LNG = "Accept-Language:";
const char * WebHelper::REQ_CONNECTION = "Connection:";


boolean WebHelper::setRequestValue(String & property, String &line, const char * match) {
    if(!line) {
        return false;
    }
    boolean found = false;
    if(line.startsWith(match)) {
        found = true;
        property = line.substring(strlen(match));
        property.trim();
    }
    return found;
}

boolean WebHelper::setRequestTypeAndUrl(RequestInfo & request, String &line) {
    if (!line) {
        return false;
    }
    boolean found = false;
    for (int i = 0; i < REQ_TYPE_LENGTH; i++) {
        if(line.startsWith(RequestTypes[i])) {
            found = true;
            request.requestMethod = RequestTypes[i];
            request.requestType = static_cast<RequestType>(i);
            request.url = line.substring(line.indexOf(' '), line.lastIndexOf(' '));
            request.url.trim();
            int idx = request.url.indexOf('?');
            if(idx != -1) {
                request.path = request.url.substring(0, idx);
                request.query = request.url.substring(idx);
            } else {
                request.path = request.url;
            }
            break;
        }
    }
    return found;
}

boolean WebHelper::setAuthorization(RequestInfo & request, String &line) {
    if(!line) {
        return false;
    }
    boolean found = false;
    if(line.startsWith(REQ_AUTH)) {
        found = true;
        int last = line.lastIndexOf(' ');
        request.authType = line.substring(strlen(REQ_AUTH), last);
        request.authType.trim();
        request.authCredential = line.substring(last);
        request.authCredential.trim();
    }
    return found;
}

boolean WebHelper::setContentLength(RequestInfo & request, String &line) {
    if(!line) {
        return false;
    }
    boolean found = false;
    if(line.startsWith(REQ_CONTENT_LENGTH)) {
        found = true;
        request.contentLength = line.substring(strlen(REQ_CONTENT_LENGTH)).toInt();        
    }
    return found;
}

RequestInfo WebHelper::parseRequest(Client &client)
{
    RequestInfo request = RequestInfo();
    request.requestType = RequestType::GET;
    String line = String();
    line = "";
    if (client)
    {
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected())
        {

            if (client.available())
            {
                char c = client.read();
                request.head += c;
                line += c;
                // end of http request is marked with a blank line
                if (c == '\n' && currentLineIsBlank)
                {
                    // check to see if a reauest body is being passed
                    if(request.contentLength > 0) {
                        // in case of asynchronous body post, wait up to 2 seconds on client
                        int timeOut = 2000;
                        unsigned long startTime = millis();
                        unsigned long currentTime;
                        while (!client.available()) {
                            currentTime = millis();
                            if (currentTime - startTime > timeOut)
                                break;
                        }
                        while (client.available())
                        {
                            // read the body of the request
                            for(int i = 0; i < request.contentLength; i++){
                                char b = client.read();
                                request.body += b;
                            }
                        }
                    }
                    break;
                }
                if (c == '\n')
                {
                    // starting a new line, check this line against the values
                    // negate any that have already run
                    boolean matched = false;
                    if (request.requestMethod.length() == 0) {
                        matched = setRequestTypeAndUrl(request, line);
                    }
                    if (!matched && request.authType.length() == 0) {
                        matched = setAuthorization(request, line);
                    }
                    // contentLength defaults to -1
                    if (!matched && request.contentLength == -1) {
                        matched = setContentLength(request, line);
                    }
                    if (!matched && request.userAgent.length() == 0) {
                        matched = setRequestValue(request.userAgent, line, REQ_USER_AGENT);
                    }
                    if (!matched && request.host.length() == 0) {
                        matched = setRequestValue(request.host, line, REQ_HOST);
                    }
                                      
                    if (!matched && request.contentType.length() == 0) {
                        matched = setRequestValue(request.contentType, line, REQ_CONTENT_TYPE);
                    }

                    if (!matched && request.accept.length() == 0) {
                        matched = setRequestValue(request.accept, line, REQ_ACCEPT);
                    }

                    if (!matched && request.acceptEncoding.length() == 0) {
                        matched = setRequestValue(request.acceptEncoding, line, REQ_ACCEPT_ENC);
                    }

                    if (!matched && request.acceptLanguage.length() == 0) {
                        matched = setRequestValue(request.acceptLanguage, line, REQ_ACCEPT_LNG);
                    }

                    if (!matched && request.connection.length() == 0) {
                        matched = setRequestValue(request.connection, line, REQ_CONNECTION);
                    }

                    line = "";
                    currentLineIsBlank = true;
                }
                else if (c != '\r')
                {
                     
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        return request;
    }    
}

boolean WebHelper::handleRoutes(Route *routes, int routes_length, RequestInfo &request, Client &client) {
    boolean match = false;
    
    for(int i = 0; i < routes_length; i++) {
        if(request.url == routes[i].route || request.path == routes[i].route) {
            match = true;
            routes[i].handler(request, client);
            break;
        }
    }
    return match;
}
