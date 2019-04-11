/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ini.hpp
 * Author: cassej
 *
 * Created on 15 декабря 2016 г., 18:52
 */

#ifndef INI_HPP
#define INI_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

inline string trim(string& str)
{
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
}

inline string trim(string& str, string charlist)
{
    str.erase(0, str.find_first_not_of(charlist));       //prefixing spaces
    str.erase(str.find_last_not_of(charlist)+1);         //surfixing spaces
    return str;
}

unordered_map<string, unordered_map<string, string>> parse_ini_file(string filename);

#endif /* INI_HPP */

