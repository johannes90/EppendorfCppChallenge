#pragma once

#include "./rapidjson/document.h" // header-only implementation of rapidjson
#include <iostream>
#include <string>   //
#include <sstream>  // to store the contents from the file
#include <fstream>  // to open/read the file
#include <vector>
#include <map> 
# include <algorithm>

#include <cstdlib>
#include "./rapidjson/istreamwrapper.h"
#include "./rapidjson/writer.h"
#include "./rapidjson/stringbuffer.h"
#include "./rapidjson/ostreamwrapper.h"


// functions to parse and inspect/alter the JSON data

//#include rapidyaml
// TODO: get the rapidyaml library to work 


/*
    Type declarations
*/
struct JSON_data_structure  
{
    int ID;
    std::string location;
    std::string type;
    std::string device_health;
    std::string last_used;
    std::string price;
    std::string color;
};


/*
Function declarations 
*/
// adds "€" sign to price field
void set_price(JSON_data_structure &data, std::string price);

int hex2dec(std::string hex);

std::string RGB2HSV(std::string rgb);

void set_color(JSON_data_structure &data, std::string color);


bool compare_Date(const JSON_data_structure& first, const JSON_data_structure& second);

std::vector<JSON_data_structure> inspect_JSON(rapidjson::Document &doc);
