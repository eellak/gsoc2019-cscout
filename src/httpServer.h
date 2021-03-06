#ifndef HTTPSERV_H_
#define HTTPSERV_H_
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <string>
#include <functional>
#include <sys/wait.h>

class GraphDisplay;
using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
 
class HttpServer;

// Struct to hold functions to respond  
typedef struct{
    utility::string_t value;
    function <json::value(void*)> handleFunction;
    void* attributes;
} Handler;

// Struct to hold functions to respond  
typedef struct{
    utility::string_t value;
    function <ostringstream *(function<void (GraphDisplay *)>)> handleFunction;
    function<void (GraphDisplay *)>  attributes;
} Graph_Handler;



class HttpServer{
private : 
    json::value params;
    ofstream *log_file;
    
public:
    static bool must_exit;
    // listener sturct 
    http_listener listener;
    
    HttpServer() {}
    // Constructor for HttpServer on a url
    HttpServer(utility::string_t url, ofstream * log);
    // HTTP GET handler
    void handle_get(http_request request);
    // HTTP Put handler
    void handle_put(http_request request);
    // HTTP Options handler
    void handle_options(http_request request);
    // adds a function to a map based on the path of the request
    void addHandler(utility::string_t value,function <json::value(void*)> handleFunction,void* attributes);
    // adds a graph function to a map based on the path of the request
    void addGraphHandler(utility::string_t value, function<ostringstream *(function<void (GraphDisplay *)>)> handleFunction, function<void (GraphDisplay *)> attributes);
    // adds a function to a map based on the path of the request for Http Put
    void addPutHandler(utility::string_t value,function <json::value(void *)> handleFunction,void* attributes);
    // starts the server to listen on the url
    void serve();
    // Returns integer request parameter from uri
    int getIntParam(string name);
    // Returns string parameter from uri 
    string getStrParam(string name);
    // Returns const char * parameter from uri
    const char * getCharPParam(string name);
    // Returns address parameter from uri
    void * getAddrParam(string name);
    // Returns true if parameter exists in uri
    bool getBoolParam(string name);
    // log server to a file
    void log(string msg);
    // JSON Put data 
    json::value putData;
};
#endif
