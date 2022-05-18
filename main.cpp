#include "./parse_JSON.cpp"

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


/*
//#include rapidyaml
// TODO: get the rapidyaml library to work 
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

// adds "€" sign to price field
void set_price(JSON_data_structure &data, std::string price)
{   
    std::string euro_sign = "€";
    data.price = price + euro_sign;

}

int hex2dec(std::string hex) {

   int len = hex.length();
   int base = 1;
   int dec = 0;

   // go from LSB(right) to MSB(left) and "revert" the hex number representation
   for (int i=len-1; i>=0; i--)
   {    

      // convert lower case to upper case
      if (hex[i]>='a' && hex[i]<='f')
      {
          hex[i] -= 32;
      }

        // Hex->Dec: [0-9] = [0-9], [A,F] = [10,15]
      if (hex[i]>='0' && hex[i]<='9') 
      {
         dec += (hex[i] - 48)*base;
         base = base * 16; 
      }
      else if (hex[i]>='A' && hex[i]<='F') 
      {
         dec += (hex[i] - 55)*base; //'A' = 65 
         base = base*16;
      }
   }
   return dec;
}
std::string RGB2HSV(std::string rgb)
{
    // convert from RGB to HSV value 
    // in "#XXXXXX"
    // out "HhhhSssVvv" 
    // used algorithm from https://www.rapidtables.com/convert/color/rgb-to-hsv.html
    
    //extract rgb values from hex number
    std::string R_hex = rgb.substr(1,2);
    std::string G_hex = rgb.substr(3,2);
    std::string B_hex = rgb.substr(5,2);
    
    int R_dec = hex2dec(R_hex);
    int G_dec = hex2dec(G_hex);
    int B_dec = hex2dec(B_hex);

    //R' = R/255
    float R_dash = static_cast<float>(R_dec) / 255.0f;
    float G_dash = static_cast<float>(G_dec) / 255.0f;
    float B_dash = static_cast<float>(B_dec) / 255.0f;

    // Cmax = max(R', G', B')
    float Cmax = std::max({R_dash, G_dash, B_dash});

    // Cmin = min(R', G', B')
    float Cmin = std::min({R_dash, G_dash, B_dash});

    //delta_C = Cmax - Cmin
    float delta_C =  Cmax - Cmin;

    // Hue calc
    float Hue{0.0f};
    if(Cmax == R_dash)
    {
        Hue = 60.f * ( static_cast<int>((G_dash - B_dash) /delta_C) % 6  );
    }
    else if(Cmax == G_dash)
    {
        Hue = 60.f * (((B_dash - R_dash) /delta_C) + 2  );
    }
    else if(Cmax == B_dash)
    {
        Hue = 60.f * (((R_dash - G_dash) /delta_C) + 4  );
    }

    //Sat calc
    float Sat{0.0f};
    if (Cmax != 0)
    {
        Sat = delta_C / Cmax;
    }

    // Val calc
    float Val = Cmax;

    int test = static_cast<int>(1.49f + 0.5f);
    //convert Sat, Val to percantage value and save a a string ('H'hhh'S'ss'V'vv)
    std::string HSV = "H" + std::to_string(static_cast<int>(Hue + 0.5 )) 
    + "S" + std::to_string(static_cast<int>(Sat * 100 + 0.5f))
    + "V" + std::to_string(static_cast<int>(Val *100 + 0.5)); 

    return HSV;
}

void set_color(JSON_data_structure &data, std::string color)
{
    std::string color_HSV = RGB2HSV(color);
    data.color = color_HSV;
}

bool compare_Date(const JSON_data_structure& first, const JSON_data_structure& second)
    {
        // return true -> first less than second -> ordered before 
        // if date a is before date b -> return true 
        int year_first   = std::stoi(first.last_used.substr(6,4));
        int year_second  = std::stoi(second.last_used.substr(6,4));
        int day_first  = std::stoi(first.last_used.substr(3,2));
        int day_second = std::stoi(second.last_used.substr(3,2));
        int month_first    = std::stoi(first.last_used.substr(0,2));
        int month_second   = std::stoi(second.last_used.substr(0,2));

        if(year_first < year_second)
        {
            return true;
        }
        else if(year_first > year_second)
        {
            return false;
        }
        else//same year
        {
            if(month_first < month_second)
            {
                return true;
            }
            else if(month_first > month_second)
            {
                return false;
            }
            else//same month
            {
                if(day_first < day_second)
                {
                    return true;
                }
                else if(day_first > day_second)
                {
                    return false;
                }
                else
                {
                    return true;//same date -> order not relevant
                }
            }
        }
    }

std::vector<JSON_data_structure> inspect_JSON(rapidjson::Document &doc)
{
    std::vector<JSON_data_structure> JSON_data_vector;

    // loop over doc object
    int cnt  = 0;
    assert(doc.IsArray());

    std::vector<int> broken_IDs;
    std::string price;

    for (auto itr = doc.Begin(); itr != doc.End(); ++itr)
    {
        JSON_data_structure itr_data;

        int itr_ID = itr->FindMember("id")->value.GetInt();

        //1: Filter the list to drop devices with broken device
        std::string device_health = itr->FindMember("device_health")->value.GetString();

        if(device_health == "broken") 
        {   
            broken_IDs.push_back(itr_ID);
            std::cout << "id " << itr_ID << " got droped because of broken device health" << std::endl;
            continue;
        }

        // Fill data structure
        itr_data.ID = itr_ID;
        
        // location
        itr_data.location = itr->FindMember("location")->value.GetString();

        // type
        itr_data.type = itr->FindMember("type")->value.GetString();

        // device health
        itr_data.device_health = itr->FindMember("device_health")->value.GetString();

        // last used
        itr_data.last_used = itr->FindMember("last_used")->value.GetString();

        // price
        price = itr->FindMember("price")->value.GetString(); 
        set_price(itr_data, price);

        // color 
        std::string color = itr->FindMember("color")->value.GetString();
        set_color(itr_data, color);

        JSON_data_vector.push_back(itr_data);
        cnt ++;
    }
    return JSON_data_vector;
}
*/

int main() {
    // 1. Parse JSON file
    std::ifstream JSON_file("data_test.json");
    std::stringstream contents;
    contents << JSON_file.rdbuf();
    rapidjson::Document doc;
    doc.Parse(contents.str().c_str());
    
    // 2. Ispect data
    // inspect and alter json data and save in a vector
    // strings are set in header: "id" "device_health" "broken" 
    std::vector<JSON_data_structure> JSON_data_vector = inspect_JSON(doc);

    std::cout << "data vector gets sorted.. " << std::endl; 
    // Sort after TODO: put in first loop to save iterations/time -> binary insertion on sorted vector
    std::sort(std::begin(JSON_data_vector), std::end(JSON_data_vector), compare_Date);
    

    // TODO: save data as yaml file -> could not get the library to work 
    return 0;
}