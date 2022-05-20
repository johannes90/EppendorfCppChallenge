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

    // 
    JsonParser jsonparser("data_test.json");
    
    std::vector<Device> JSON_data_vector = jsonparser.inspect_Devices();

    std::sort(std::begin(JSON_data_vector), std::end(JSON_data_vector), JsonParser::compare_Date);

    // TODO: save data as yaml file -> 
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

/*
YAML: 
---
- ()


*/