#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <string>
#include <functional>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
 

typedef struct{
    utility::string_t value;
    function <json::value(FILE*,void*)> handleFunction;
    void* attributes;
} Handler;


class HttpServer{
private:
    map<utility::string_t, Handler> mapHandlers;
public:
    HttpServer(){}
    HttpServer(utility::string_t url);
    void handle_get(http_request request);
    void handle_post(http_request request);
    void handle_delete(http_request request);
    void handle_put(http_request request);
    void addHandler(utility::string_t value,function <json::value(FILE*,void*)> handleFunction,void* attributes);
    http_listener listener;
};