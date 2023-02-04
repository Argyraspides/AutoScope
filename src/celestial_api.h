#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

#include "coords.h"
#include "query_strings.h"

// Planet codes (and moon) for NASA's Horizons API.
#define MERCURY "1"
#define VENUS "2"
#define EARTH "3"
#define MARS "4"
#define JUPITER "5"
#define SATURN "6"
#define URANUS "7"
#define NEPTUNE "8"
#define PLUTO "9"
#define MOON "10"

enum body_type
{
    planet,
    extra_solar
};

std::vector<std::string> horizons_planet_codes =
    {
        "199", // Mercury
        "299", // Venus
        "499", // Mars
        "599", // Jupiter
        "699", // Saturn
        "799", // Uranus
        "899", // Neptune
        "999", // Pluto
};

std::vector<std::string> ned_body_codes =
    {
        "m31" // Andromeda Galaxy
};

std::map<std::string, int> horizons_map;
std::map<std::string, int> ned_map;

void track();
void initializeMaps();
void focusPlanet(const std::string &planet);
void moveScope(const horiz_coord &horiz_coord);
void focusBody(const std::string &body, const body_type &body_type);

float getGMST();

equat_coord getPlanetPosition(const std::string &planet);
equat_coord getExtrasolarBodyPosition(const std::string &extrasolar_body);

std::tm getCurrentUTC(const int &offset);
