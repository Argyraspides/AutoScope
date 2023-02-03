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

void track();
void focusPlanet(const std::string &planet);
void moveScope(const horiz_coord &horiz_coord);

float getGMST();

equat_coord getPlanetPosition(const std::string &planet);
std::tm getCurrentUTC (const int &offset);
