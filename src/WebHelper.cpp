#include <Arduino.h>
#include <inttypes.h>
#include <Client.h>
#include <WebHelper.h>
#include <avr/pgmspace.h>

using namespace std;

#define REQ_TYPE_LENGTH 8
#define RESP_TYPE_LENGTH 15
#define START_TOKEN "{{"
#define END_TOKEN "}}"
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

const ResponseType WebHelper::ResponseTypes[RESP_TYPE_LENGTH] = {
    { "OK", 200, StatusCode::OK },
    { "Multiple Choices", 300, StatusCode::MULTIPLE_CHOICES },
    { "Moved Permanentl", 301, StatusCode::MOVED_PERMANENTLY },
    { "Found", 302, StatusCode::FOUND },
    { "Not Modified", 304, StatusCode::NOT_MODIFIED },
    { "Temporary Redirect", 307, StatusCode::TEMPORARY_REDIRECT },
    { "Bad Request", 400, StatusCode::BAD_REQUEST },
    { "Unauthorized", 401, StatusCode::UNAUTHORIZED },
    { "Forbidden", 403, StatusCode:: FORBIDDEN },
    { "Not Found", 404, StatusCode::NOT_FOUND },
    { "Gone", 410, StatusCode::GONE },
    { "Internal Server Error", 500, StatusCode::SERVER_ERROR },
    { "Not Implemented", 501, StatusCode::NOT_IMPLEMENTED },
    { "Service Unavailable", 503, StatusCode::SERVICE_UNAVAILABLE },
    { "Permission Denied", 550, StatusCode::PERMISSION_DENIED }
};


const char * WebHelper::REQ_USER_AGENT PROGMEM = "User-Agent:";
const char * WebHelper::REQ_CONTENT_TYPE PROGMEM = "Content-Type:";
const char * WebHelper::REQ_HOST PROGMEM = "Host:";
const char * WebHelper::REQ_CONTENT_LENGTH PROGMEM = "Content-Length:";
const char * WebHelper::REQ_AUTH PROGMEM = "Authorization:";
const char * WebHelper::REQ_ACCEPT PROGMEM = "Accept:";
const char * WebHelper::REQ_ACCEPT_ENC PROGMEM = "Accept-Encoding:";
const char * WebHelper::REQ_ACCEPT_LNG PROGMEM = "Accept-Language:";
const char * WebHelper::REQ_CONNECTION PROGMEM = "Connection:";


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

void WebHelper::respondWith(StatusCode statusCode, String contentType, String headers, const char * body, Client &client) {
    ResponseType resp = ResponseTypes[statusCode];
    client.print("HTTP/1.1 ");
    client.print(resp.statusCode);
    client.print(" ");
    client.println(resp.message);
    client.print("Content-Type: ");
    client.println(contentType);
    if(headers.length() > 0) {
        client.println(headers);
    }
    client.println();
    // read back a char
    char bChar;
    size_t len = strlen_P(body);
    for (uint32_t k = 0; k < len; k++) {
        bChar = pgm_read_byte_far(body + k);
        client.print(bChar);
    }
    client.println();
    // client.println(body);
    client.println();
}
