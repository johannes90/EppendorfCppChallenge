#include "./parse_JSON.h"

#include "./rapidjson/document.h" // header-only implementation of rapidjson
#include <iostream>
#include <string>   //
#include <sstream>  // to store the contents from the file
#include <fstream>  // to open/read the file
#include <vector>
#include <map> 
#include <algorithm>

#include <cstdlib>
#include "./rapidjson/istreamwrapper.h"
#include "./rapidjson/writer.h"
#include "./rapidjson/stringbuffer.h"
#include "./rapidjson/ostreamwrapper.h"


// for opening files
#include <stdio.h> 
#include <cstring>

//library to save as yaml
// TODO: get the rapidyaml library to work 
#define RYML_SINGLE_HDR_DEFINE_NOW
#include "./rapidyaml/ryml_all.hpp"


int main() {
    std::string file = "data_test.json";
    JsonParser jsonparser(file);
    
    std::vector<Device> JSON_data_vector = jsonparser.inspect_Devices();

    std::sort(std::begin(JSON_data_vector), std::end(JSON_data_vector), JsonParser::compare_Date);

    // TODO: because saving as yaml does not yet work the following is a workaround to show how the rest works:
    // therefore it is not beautifull and all hardcoded.. 
    std::cout << "First few lines of original JSON file content:" << std::endl; 
    std::string line;
    std::ifstream filestream(file);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            
            std::cout << line << std::endl;
            //std::istringstream linestream(line);
        }
    }

    // Altered file
    std::cout << std::endl << "First few lines of altered file content:" << std::endl; 
    std::string date;
    std::string YYYY;
    std::string MM;
    std::string DD;
    for(Device device: JSON_data_vector)
    {
        
        date = std::to_string(device.get_last_used().month) + "/" + std::to_string(device.get_last_used().day) + "/" + std::to_string(device.get_last_used().year);

        std::cout << "ID: " << device.get_id() 
        << ", location: " << device.get_location() 
        << ", type: " << device.get_type()
        << ", device_health: " << device.get_device_health()
        << ", last_used: " << date
        << ", price: " << device.get_price()
        << ", color: " << device.get_color() 
        << std::endl;
    }

      
  


    // TODO: save data as yaml file
    /*
    std::vector<char> input({ 'a', 'b', 'c' });
    std::string s(input.begin(), input.end()); //vector -> string
    int n = s.length();
 
    char char_array[n + 1];
    std::strcpy(char_array, s.c_str());

    char yml_buf[] = "{foo: 1, bar: [2, 3], john: doe}";

    ryml::Tree tree = ryml::parse_in_place(ryml::substr(yml_buf));
    //ryml::Tree tree = yml::parse_in_arena(ryml::to_csubstr());
    
    FILE * pFile;
    pFile = fopen ("myfile.yaml" , "w");
    
    ryml::emit(tree, pFile);
    

    fclose (pFile);
    */
    // ryml::Tree tree = ryml::parse(JSON_data_vector);
    // ryml::emit(JSON_data_vector, "yamldata.yaml");
    
    return 0;
}