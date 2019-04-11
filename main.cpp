/*
 * File:   main.cpp
 * Author: marusyarys
 *
 * Created on 20 Август 2016 г., 19:40
 */

#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include "global.hpp"
#include "handlers.hpp"
#include "ini.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    signal(SIGPIPE, SIG_IGN);
   
    int port = 9100;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    struct event_base *base = event_init();
    struct event *gc_event;
    if (base == NULL) return -1;
    
    struct evhttp *httpd = evhttp_new(base);
    
    config = parse_ini_file("./config.ini");
    init_extensions(&mimetypes);
    
    if (httpd == NULL) return -1;
    try
    {
        if (evhttp_bind_socket(httpd, "0.0.0.0", port) != 0) return -1;

        evhttp_set_cb(httpd, "/upload", upload, NULL);
        evhttp_set_cb(httpd, "/status", status, NULL);
        
        

        event_base_dispatch(base);
    }
    catch (exception const &e)
    {
        cout << e.what() << endl;
    }
    evhttp_free(httpd);
    event_base_free(base);
    return 0;
}