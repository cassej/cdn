/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   handlers.hpp
 * Author: cassej
 *
 * Created on 30 сентября 2016 г., 14:55
 */

#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include "global.hpp"


using namespace std;

void upload(struct evhttp_request *req, void *arg);
void status(struct evhttp_request *req, void *arg);
string get_file_place(string content);
unordered_map<string, string> get_storage();
bool place_file(string content, string dst, string path);
string check_exists (string path);
string get_extension(string content);


#endif /* HANDLERS_HPP */

