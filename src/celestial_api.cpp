#include "celestial_api.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#define log(x) std::cout << x << std::endl
using namespace std::chrono;

void track();
void focusPlanet();
equat_coord getPlanetPosition(const std::string &planet);
float getGMST();

int main()
{
    equat_coord c = getPlanetPosition(SATURN);
    float x = getGMST();
}

void track()
{
}

void focusPlanet(const std::string &planet)
{
}

equat_coord getPlanetPosition(const std::string &planet)
{

    log("\nOBTAINING PLANET POSITION");
    // Represents the final responses of the API. JPL Horizons returns in .txt format.
    std::ostringstream horizon_response;
    equat_coord final_coordinates;

    // Holds the and constructs the final query string for each API (USNO, NASA JPL)
    horizons_query_string query_string;

    log("GETTING CURRENT TIME ...");
    // Obtain the current time as "YYYY-MM-DD HH:MM:SS" for the API:
    // Get the local time
    auto now = std::chrono::system_clock::now();
    // Convert to time_t object
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    // Convert to tm struct, in UTC time.
    std::tm tm = *std::gmtime(&time);
    // API Demands an end time that has to be greater than the start time, we will just add 60 seconds.
    time += 60;
    std::tm tm_end = *std::gmtime(&time);

    std::ostringstream oss, oss_end;
    // Set format "YYYY-MM-DD HH:MM:SS" for start and end times.
    // the space character is represented by "%20" for the URL later.
    oss << std::put_time(&tm, "%Y-%m-%d%20%H:%M:%S");
    log("CURRENT TIME: " + oss.str().substr(0, 10) + ", " + oss.str().substr(13, 21) + " UT");
    oss_end << std::put_time(&tm_end, "%Y-%m-%d%20%H:%M:%S");

    // Set query string parameters and construct the string
    query_string.start_time = oss.str();
    query_string.stop_time = oss_end.str();
    query_string.planet = planet;
    query_string.constructQueryString();
    log("\nQUERY STRING: " + query_string.link + "\n");

    // Call NASA's JPL Horizons API for planetary positional data, put into horizons_response.
    log("CALLING HORIZONS API ...");
    horizon_response << curlpp::options::Url(query_string.link);
    log("SUCCESSFULLY CALLED API");

    // Retrieve only part of the table that contains the actual data.
    log("PARSING DATA ...");
    std::string api_response = horizon_response.str();
    std::string start_delimiter = "$$SOE";
    std::string end_delimiter = "$$EOE";

    unsigned first = api_response.find(start_delimiter) + start_delimiter.length() + 1;
    unsigned last = api_response.find(end_delimiter) - end_delimiter.length() - 1;

    std::string celestial_data = api_response.substr(first, last - first);

    // R.A. Hour values are the 26th and 27th characters in each line.
    //      Minute values are the 29th and 30th
    //      Seconds are the 32nd to the 36th.

    final_coordinates.right_ascension[0] = std::stof(celestial_data.substr(26, 27));
    final_coordinates.right_ascension[1] = std::stof(celestial_data.substr(29, 30));
    final_coordinates.right_ascension[2] = std::stof(celestial_data.substr(32, 36));

    // Given as minutes by Horizons API.

    float degrees = final_coordinates.declination = std::stof(celestial_data.substr(38, 40));
    float sign = (degrees < 0) ? -1 : 1;
    float minutes = final_coordinates.declination = std::stof(celestial_data.substr(41, 42));
    float seconds = final_coordinates.declination = std::stof(celestial_data.substr(44, 47));
    final_coordinates.declination = sign * (abs(degrees) + minutes / 60.0f + seconds / 3600.0f);

    // Right ascension given as HH:MM:SS. Updates the separate degree value variable.
    final_coordinates.update();

    log("SUCCESSFULLY OBTAINED PLANETARY DATA IN EQUATORIAL COORDINATES");
    log("PLANET: " + planet + ", COORDINATES:\nR.A. " +
        std::to_string(final_coordinates.right_ascension[0]).substr(0, 5) + "H:" +
        std::to_string(final_coordinates.right_ascension[1]).substr(0, 5) + "M:" +
        std::to_string(final_coordinates.right_ascension[2]).substr(0, 5) + "S:" + "\nDEC: " +
        std::to_string(final_coordinates.declination));

    return final_coordinates;
}

float getGMST()
{
    log("\nOBTAINING GMST ...");
    usno_query_string query_string;
    std::string usno_response;

    log("GETTING CURRENT TIME ...");
    // Obtain the current time as "YYYY-MM-DD HH:MM:SS" for the API:
    // Get the local time
    auto now = std::chrono::system_clock::now();
    // Convert to time_t object
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    // Convert to tm struct, in UTC time.
    std::tm tm = *std::gmtime(&time);

    // String streams for our date and time
    std::ostringstream oss_date, oss_time;

    // Set format "YYYY-MM-DD HH:MM:SS" for start and end times.
    oss_date << std::put_time(&tm, "%Y-%m-%d");
    oss_time << std::put_time(&tm, "%H:%M:%S");
    // Into the query_string as an std::string
    query_string.startDate = oss_date.str();
    query_string.startTime = oss_time.str();

    query_string.constructQueryString();

    // USNO Returns as a JSON, so we configure curlpp to take json replies
    log("REQUESTING USNO API ...");
    curlpp::Easy request;
    request.setOpt(curlpp::options::Url(query_string.link));
    request.setOpt(curlpp::options::HttpHeader({"Accept: application/json"}));
    // Get the reply as a string so we can parse it as a JSON object with JSONCPP.
    request.setOpt(curlpp::options::WriteFunction([&usno_response](char *data, size_t size, size_t nmemb)
                                                  {
        usno_response.append(data, size * nmemb);
        return size * nmemb; }));

    // API call.
    request.perform();
    log("SUCCESSFULLY RETRIEVED API\nPARSING DATA");

    Json::Value json_response;
    Json::Reader reader;
    // Parse API response to json object.
    reader.parse(usno_response, json_response);

    // Extract GMST from response.
    std::string gmst = json_response["properties"]["data"][0]["gmst"].asString();

    // Return GMST in degrees.
    float hours = std::stof(gmst.substr(0, 1));
    float minutes = std::stof(gmst.substr(3, 4));
    float seconds = std::stof(gmst.substr(6, 10));
    log("GMST RETRIEVAL COMPLETE");
    log("CURRENT GMST: " + gmst);

    return (15.0f * hours + minutes / 4.0f + seconds / 240.0f);
}

std::string getCurrentDateAndTime()
{
}

// For timing
//  auto start = high_resolution_clock::now();
//  auto stop = high_resolution_clock::now();
//  auto duration = duration_cast<microseconds>(stop - start);
//  std::cout << duration.count() << "\n";