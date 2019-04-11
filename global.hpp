#ifndef GLOBAL_HPP
#define GLOBAL_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <evhttp.h>
#include <event2/event.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include "MurmurHash3.h"
#include <magic.h>


using namespace std;

extern unordered_map<string, string> mimetypes;
extern unordered_map<string, unordered_map<string, string>> config;
void init_extensions(unordered_map<string, string> *mimetypes);

#endif /* GLOBAL_HPP */

