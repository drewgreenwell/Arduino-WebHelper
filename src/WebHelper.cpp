#include <Arduino.h>
#include <Client.h>
#include <WebHelper.h>
/*
POST / HTTP/1.1
User-Agent: Mozilla/5.0 (Windows NT; Windows NT 10.0; en-US) WindowsPowerShell/5.1.18362.752
Content-Type: application/json
Host: 192.168.1.123
Content-Length: 15
*/
void WebHelper::init()
{
     
}

String WebHelper::RequestTypes[8] = {
    "GET",
    "POST",
    "PUT",
    "PATCH",
    "DELETE",
    "OPTIONS",
    "CONNECT",
    "TRACE"
};

const char * WebHelper::REQ_USER_AGENT = "User-Agent: ";
const char * WebHelper::REQ_CONTENT_TYPE = "Content-Type: ";
const char * WebHelper::REQ_HOST = "Host: ";
const char * WebHelper::REQ_CONTENT_LENGTH = "Content-Length: ";
const char * WebHelper::REQ_AUTH = "Authorization: ";
const char * WebHelper::REQ_ACCEPT = "Accept: ";
const char * WebHelper::REQ_ACCEPT_ENC = "Accept-Encoding: ";
const char * WebHelper::REQ_ACCEPT_LNG = "Accept-Language: ";

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
                    if (request.requestMethod.length() == 0) {
                        setRequestTypeAndUrl(request, line);
                    }
                    if (request.authType.length() == 0) {
                        setAuthorization(request, line);
                    }
                    // contentLength defaults to -1
                    if (request.contentLength == -1) {
                        setContentLength(request, line);
                    }
                    if (request.userAgent.length() == 0) {
                        setRequestValue(request.userAgent, line, REQ_USER_AGENT);
                    }
                    if (request.host.length() == 0) {
                        setRequestValue(request.host, line, REQ_HOST);
                    }
                                      
                    if (request.contentType.length() == 0) {
                        setRequestValue(request.contentType, line, REQ_CONTENT_TYPE);
                    }

                    if (request.accept.length() == 0) {
                        setRequestValue(request.accept, line, REQ_ACCEPT);
                    }

                    if (request.acceptEncoding.length() == 0) {
                        setRequestValue(request.acceptEncoding, line, REQ_ACCEPT_ENC);
                    }

                    if (request.acceptLanguage.length() == 0) {
                        setRequestValue(request.acceptLanguage, line, REQ_ACCEPT_LNG);
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

void WebHelper::setRequestValue(String & property, String &line, const char * match) {
    if(!line) {
        return;
    }
    if(line.startsWith(match)) {
        property = line.substring(strlen(match));
        property.trim();
    }
}

void WebHelper::setRequestTypeAndUrl(RequestInfo & request, String &line) {
    if (!line) {
        return;
    }
    for (int i = 0; i < 8; i++) {
        if(line.startsWith(RequestTypes[i])) {
            request.requestMethod = RequestTypes[i];
            request.requestType = static_cast<RequestType>(i);
            request.url = line.substring(line.indexOf(' '), line.lastIndexOf(' '));
            request.url.trim();
            break;
        }
    }
}

void WebHelper::setAuthorization(RequestInfo & request, String &line) {
    if(!line) {
        return;
    }
    if(line.startsWith(REQ_AUTH)) {
        int last = line.lastIndexOf(' ');
        request.authType = line.substring(strlen(REQ_AUTH), last);
        request.authCredential = line.substring(last);
        request.authType.trim();
        request.authCredential.trim();
    }
}

void WebHelper::setContentLength(RequestInfo & request, String &line) {
    if(!line) {
        return;
    }
    if(line.startsWith(REQ_CONTENT_LENGTH)) {
        request.contentLength = line.substring(strlen(REQ_CONTENT_LENGTH)).toInt();
    }
}