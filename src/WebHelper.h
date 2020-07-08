#ifndef WebHelper_h
#define WebHelper_h

enum RequestType {
    GET = 1,
    POST = 2,
    PUT = 3,
    PATCH = 4,
    DELETE = 5,
    OPTIONS = 6,
    CONNECT = 7,
    TRACE = 8
};

struct RequestInfo {  
  RequestInfo() : contentLength(-1) { }
  int contentLength;
  String contentType;
  String host;
  String url;
  String rawUrl;
  String query;
  String userAgent;
  String requestMethod;
  String body;
  String head;
  String authType;
  String authCredential;
  String accept;
  String acceptEncoding;
  String acceptLanguage;
  String connection;
  enum RequestType requestType;
};

class WebHelper
{
    private:
        static void init();
        static boolean setRequestValue(String &property, String &line, const char * match);
        static boolean setRequestTypeAndUrl(RequestInfo &request, String &line);        
        static boolean setAuthorization(RequestInfo &request, String &line);    
        static boolean setContentLength(RequestInfo &request, String &line);
    public:
        static const char * REQ_USER_AGENT;
        static const char * REQ_CONTENT_TYPE;
        static const char * REQ_HOST;
        static const char * REQ_CONTENT_LENGTH;
        static const char * REQ_AUTH;
        static const char * REQ_ACCEPT;
        static const char * REQ_ACCEPT_ENC;
        static const char * REQ_ACCEPT_LNG;
        static const char * REQ_CONNECTION;
        static const String RequestTypes[];
        static RequestInfo parseRequest(Client &client);
        static boolean checkRoutes(char * routes[], void (*handlers[])(RequestInfo), RequestInfo request);
};

#endif