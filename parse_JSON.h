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


class Date// TODO: put in namespace
    {   
        public:
        int year_;
        int month_;
        int day_;

        // constructor, destructor
        Date(int year, int month, int day);
        Date(std::string date_string);
        Date();
        ~Date();
        
        // getters
        // TODO:


        // setters
        //TODO:

        // overload < operator  
        bool operator<(const Date other_date) const;

        // overload operator >
        bool operator>(const Date other_date) const;
    };

class Device
{
private:
    // Device Attributes
    int id_;
    std::string location_;
    std::string type_;
    std::string device_health_;
    Date last_used_;
    std::string price_;
    std::string color_; //TODO:  struct HSV_Color
    

public:
    Device(int id, std::string location, std::string type, std::string device_health, std::string last_used, std::string price, std::string color);
    ~Device();

    // Setter functions 
    void set_ID(int id);
    void set_location(std::string location);
    void set_type(std::string type);
    void set_device_health(std::string device_health);
    void set_last_used(std::string last_used);
    void set_price(std::string price);
    void set_color(std::string color);

    // Getter functions
    int get_id() const;
    std::string get_location() const;
    std::string get_type() const;
    std::string get_device_health() const;
    Date get_last_used() const;
    std::string get_price() const;
    std::string get_color() const;

    // Other class methods
    std::string RGB2HSV(std::string rgb); //TODO: do not have to be a class method
    int hex2dec(std::string hex);         //TODO: do not have to be a class method

};

// Namespace to complete the task of parsing a JSON file with multiple devices and its attributes to Yaml
// and performing operations on these Devices and their attributes
class JsonParser //TODO: make that a namespace
{   
public:// TODO: i just use a class because i could not figure out some errors related to the namespace therefore made it a class and everything public

    rapidjson::Document json_document_;
    
    JsonParser(std::string file_name);
    
    ~JsonParser();
    
    // Container to store all Devices of the JSON file 
    //std::vector<Device> Devices; // TODO: how can i use that as return type in inspect_devices

    // Function to be used in std::sort() to compare two "Device" objects according to their "last_used" date
    static bool compare_Date(const Device& first, const Device& second);

    static bool compare_Devices_by_Date(const Device& first, const Device& second);

    // Function to loop over all Devices of the JSON file, calling functions on them
    std::vector<Device> inspect_Devices(); 
};
