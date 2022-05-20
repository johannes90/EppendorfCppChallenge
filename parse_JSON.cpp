#include "parse_JSON.h"

// Definitions for JsonParser class
JsonParser::JsonParser(std::string file_name)
{
    std::ifstream JSON_file(file_name);
    std::stringstream contents;
    contents << JSON_file.rdbuf();
    //rapidjson::Document doc;
    json_document_.Parse(contents.str().c_str());
}

JsonParser::~JsonParser(){}

// Function to be used in std::sort() to compare two "Device" objects according to their "last_used" date
bool JsonParser::compare_Date(const Device& first, const Device& second)
{
    // return true -> first less than second -> ordered before (date first junger than date second)
    int year_first   = first.get_last_used().year;
    int year_second  = second.get_last_used().year;
    int day_first    = first.get_last_used().month;
    int day_second   = second.get_last_used().month;
    int month_first  = first.get_last_used().day;
    int month_second = second.get_last_used().day;

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
                //same date -> order not relevant therefore just pick "first"
                return true;
            }
        }
    }
}

// Function to loop over all Devices of the JSON file, calling functions on them
std::vector<Device> JsonParser::inspect_Devices()
{
    std::vector<Device> JSON_data_vector;

    assert(json_document_.IsArray());

    // to extract the data from python object inside the loop
    int         itr_ID;
    std::string itr_location;      
    std::string itr_type;          
    std::string itr_device_health;
    std::string itr_last_used;     
    std::string itr_price;         
    std::string itr_color;         
    
    // loop over Json document to construct Device object
    for (auto itr = json_document_.Begin(); itr != json_document_.End(); ++itr)
    {
        itr_ID = itr->FindMember("id")->value.GetInt();

        // Filter the list to drop devices with broken device
        std::string device_health = itr->FindMember("device_health")->value.GetString();

        if(device_health == "broken") 
        {   
            std::cout << "ID " << itr_ID << " got droped because of broken device health" << std::endl;
            continue;
        }

        // Extract the data from JSON object
        itr_location      = itr->FindMember("location")->value.GetString();
        itr_type          = itr->FindMember("type")->value.GetString();
        itr_device_health = itr->FindMember("device_health")->value.GetString();
        itr_last_used     = itr->FindMember("last_used")->value.GetString();
        itr_price         = itr->FindMember("price")->value.GetString(); 
        itr_color         = itr->FindMember("color")->value.GetString();
        
        // Construct Device object 
        Device itr_data(itr_ID, itr_location, itr_type, itr_device_health, itr_last_used, itr_price, itr_color);

        JSON_data_vector.push_back(itr_data);
    }

    return JSON_data_vector;
}


// Class constructor
Device::Device(int id, std::string location, std::string type, std::string device_health, std::string last_used, std::string price, std::string color)
{
    set_ID(id);
    set_location(location);
    set_type(type);
    set_device_health(device_health);
    set_last_used(last_used);
    set_price(price);
    set_color(color);
}

// Class (default) Destructor
Device::~Device(){}

// Mutator functions ("setters")
void Device::set_ID(int id){ Device::id_ = id; }

void Device::set_location(std::string location){ Device::location_ = location; }

void Device::set_type(std::string type){ Device::type_ = type; }

void Device::set_device_health(std::string device_health){ Device::device_health_ = device_health; }

void Device::set_last_used(std::string last_used)
{
    // extract date from string
    Device::last_used_= string2Date(last_used);
}

void Device::set_price(std::string price)
{   
    std::string euro_sign = "€";
    Device::price_  = price + euro_sign;
}

void Device::set_color(std::string color)
{
    std::string color_HSV = Device::RGB2HSV(color);
    Device::color_ = color_HSV;
}

// Acessor functions ("getters")
int Device::get_id() const { return Device::id_; }
std::string  Device::get_location() const { return Device::location_; }
std::string Device::get_type() const {return Device::type_;}
std::string  Device::get_device_health() const { return Device::device_health_; }
Date Device::get_last_used() const { return Device::last_used_; }
std::string Device::get_price() const { return Device::price_; }
std::string Device::get_color() const { return Device::price_; } //TODO:  struct HSV_Color


// Other class methods
Date Device::string2Date(std::string date_string)
{
    Date date;

    date.year = std::stoi(date_string.substr(6,4));
    date.month = std::stoi(date_string.substr(3,2));
    date.day = std::stoi(date_string.substr(0,2));
    
    return date;
}

std::string Device::RGB2HSV(std::string rgb)
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

int Device::hex2dec(std::string hex) {

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

/*

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