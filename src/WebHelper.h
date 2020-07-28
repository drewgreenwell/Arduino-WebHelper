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

enum StatusCode {
    OK = 0,
    MULTIPLE_CHOICES = 1,
    MOVED_PERMANENTLY = 2,
    FOUND = 3,
    NOT_MODIFIED = 4,
    TEMPORARY_REDIRECT = 5,
    BAD_REQUEST = 6,
    UNAUTHORIZED = 7,
    FORBIDDEN = 8,
    NOT_FOUND = 9,
    GONE = 10,
    SERVER_ERROR = 11,
    NOT_IMPLEMENTED = 12,
    SERVICE_UNAVAILABLE = 13,
    PERMISSION_DENIED = 14
};

struct RequestInfo {  
  RequestInfo() : contentLength(-1) { }
  int contentLength;
  String contentType;
  String host;
  String url;
  String path;
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

struct ResponseType {
    String message;
    int statusCode;
    enum StatusCode responseType;
};

typedef void (*RouteHandler)(RequestInfo &request, Client &client);

struct Route {
    String route;
    RouteHandler handler;
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
        static const ResponseType ResponseTypes[];
        static RequestInfo parseRequest(Client &client);
        static boolean handleRoutes(Route *routes, int routes_length, RequestInfo &request, Client &client);
        static void respondWith(StatusCode statusCode, String contentType, String headers, const char * body, Client &client);
};

#endif