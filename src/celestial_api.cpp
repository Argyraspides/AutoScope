#include "celestial_api.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#define log(x) std::cout << x << std::endl
using namespace std::chrono;

void track();
void focusPlanet();
equat_coord getPlanetPosition(const std::string &planet);

int main()
{
    equat_coord c = getPlanetPosition(SATURN);
}

void track()
{
}

void focusPlanet(const std::string &planet)
{
}

equat_coord getPlanetPosition(const std::string &planet)
{
    log("OBTAINING PLANET POSITION");
    // Represents the final responses of the API. JPL Horizons returns in .txt format.
    std::ostringstream horizon_response;
    equat_coord final_coordinates;

    // Holds the and constructs the final query string for each API (USNO, NASA JPL)
    horizons_query_string query_string;

    log("GETTING CURRENT TIME ...");
    // Obtain the current time as "YYYY-MM-DD HH:MM:SS" for the API:
    std::time_t time = std::time(nullptr);
    std::tm tm = *std::localtime(&time);
    // API Demands an end time that has to be greater than the start time, we will just add 60 seconds.
    time += 60;
    std::tm tm_end = *std::localtime(&time);

    std::ostringstream oss, oss_end;
    // Set format "YYYY-MM-DD HH:MM:SS" for start and end times.
    // the space character is represented by "%20" for the URL later.
    oss << std::put_time(&tm, "%Y-%m-%d%20%H:%M:%S");
    log("CURRENT TIME: " + oss.str().substr(0, 10) + ", " + oss.str().substr(13, 21) + " UT");
    oss_end << std::put_time(&tm_end, "%Y-%m-%d%20%H-%M-%S");

    // Set query string parameters and construct the string
    query_string.start_time = oss.str();
    query_string.stop_time = oss_end.str();
    query_string.planet = planet;
    query_string.constructQueryString();

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
    float sign = (std::stof(celestial_data.substr(38, 38))) < 0 ? -1 : 1;

    float degrees = final_coordinates.declination = std::stof(celestial_data.substr(39, 40));
    float minutes = final_coordinates.declination = std::stof(celestial_data.substr(41, 42));
    float seconds = final_coordinates.declination = std::stof(celestial_data.substr(44, 47));
    final_coordinates.declination = sign * (degrees + minutes / 60.0f + seconds / 3600.0f);

    // Right ascension given as HH:MM:SS. Updates the separate degree value variable.
    final_coordinates.update();

    log("SUCCESSFULLY OBTAINED PLANETARY DATA IN EQUATORIAL COORDINATES");
    log("PLANET: " + planet + ", COORDINATES:\nR.A. " +
        std::to_string(final_coordinates.right_ascension[0]) + "H:" +
        std::to_string(final_coordinates.right_ascension[1]) + "M:" +
        std::to_string(final_coordinates.right_ascension[2]) + "S:" + "\nDEC: " +
        std::to_string(final_coordinates.declination));

    return final_coordinates;
}

float getGMST()
{

    usno_query_string query_string;

    std::string usno_response;

    // USNO Returns as a JSON, so we configure curlpp to take json replies
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

    Json::Value json_response;
    Json::Reader reader;
    // Convert to JSONPP json object.
    reader.parse(usno_response, json_response);
}

// For timing
//  auto start = high_resolution_clock::now();
//  auto stop = high_resolution_clock::now();
//  auto duration = duration_cast<microseconds>(stop - start);
//  std::cout << duration.count() << "\n";