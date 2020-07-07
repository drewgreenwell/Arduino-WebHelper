#ifndef WebHelper_h
#define WebHelper_h

enum RequestType {
    GET = 1,
    POST = 2,
    PUT = 3,
    PATCH = 4,
    DELETE = 5,
    OPTIONS = 6,
    CONNECT = 8,
    TRACE = 9,    
};

struct RequestInfo {  
  int contentLength;
  String contentType;
  String host;
  String url;
  String userAgent;
  String requestMethod;
  String body;
  enum RequestType requestType;
};

class WebHelper
{
    private:
        static void init();
        static void setRequestTypeAndUrl(RequestInfo &request, String &line);        
        static void setHost(RequestInfo &request, String &line);
        static void setUserAgent(RequestInfo &request, String &line);        
        static void setContentLength(RequestInfo &request, String &line);
        static void setContentType(RequestInfo &request, String &line);
    public:
        static const char * REQ_USER_AGENT;
        static const char * REQ_CONTENT_TYPE;
        static const char * REQ_HOST;
        static const char * REQ_CONTENT_LENGTH;
        static String RequestTypes[];
        static RequestInfo parseRequest(Client &client);

};

#endif