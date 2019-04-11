#include "ini.hpp"

unordered_map<string, unordered_map<string, string>> parse_ini_file(string filename)
{
    fstream ini_file;
    ini_file.open(filename.c_str(), fstream::in);
    string ini_line;
    string section = "";
    string key = "";
    string value = "";
    unordered_map<string, unordered_map<string, string>> ini_data;
    
    if (ini_file.is_open()) {
        cerr << "Reading config..." << endl;
        while(!ini_file.eof())
        {
            getline(ini_file, ini_line);
            ini_line = trim(ini_line);
            if (ini_line.length() > 0 && ini_line[0] != ';') {
                
                if (ini_line[0] == '[') {
                    section = trim(ini_line, "[]");
                } else {
                    int pos = ini_line.find_first_of("=");
                    key = ini_line.substr(0, pos);
                    value = ini_line.substr(pos+1, ini_line.size());
                    ini_data[section][key] = value;
                    cerr << "Section: " << section << " Key: " << key << " Value: " << value << endl;
                }   
            }
        }
    } else {
        cerr << "Error reading config... " << filename.c_str() << endl;
    }
    
    ini_file.close();
    
    return ini_data;
}