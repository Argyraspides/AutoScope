#include "celestial_api.h"
#define log(x) std::cout << x << "\n"

// Frequency of tracking an object, given in milliseconds.
const int tracking_frequency = 20000;

int main()
{

    bool running = true;
    while (running)
    {
    restart:
        std::string planet;
        std::cout << "ENTER TARGET PLANET. CODES:\n"
                     "1 - MERCURY\n"
                     "2 - VENUS\n"
                     "4 - MARS\n"
                     "5 - JUPITER\n"
                     "6 - SATURN\n"
                     "7 - URANUS\n"
                     "8 - NEPTUNE\n"
                     "10 - MOON\n";
        std::cin >> planet;

        bool valid_planet = false;
        for (int i = 0; i < 11; i++)
        {
            if (planet == std::to_string(i))
            {
                valid_planet = true;
                break;
            }
        }

        if (valid_planet)
        {
            log("STARTING ...");

            focusPlanet(planet);

            while (true)
            {
                // Continue tracking the planet
                log("FOCUSSING ... ENTER 'C' TO EXIT AND SELECT ANOTHER PLANET, OR 'X' TO FOCUS THE PLANET AGAIN");
                char inp;
                std::cin >> inp;
                if (inp == 'C' || inp == 'c')
                {
                    break;
                }
                else if (inp == 'X' || inp == 'x')
                {
                    focusPlanet(planet);
                }
            }
            std::cin.clear();
        }
        else
        {
            log("ERROR: INVALID INPUT");
            goto restart;
        }
    }
}

void moveScope(const horiz_coord &horiz_coord)
{
}

void focusPlanet(const std::string &planet)
{

    // THIS SHOULD GO INTO THE "FOCUS PLANET" MODULE.
    equat_coord planet_position = getPlanetPosition(planet);
    // Get the current Greenwich Mean Sidereal Time.
    float gmst = getGMST();
    // gps_coord gps_coord = getGPSCoord();
    gps_coord gps_coord;
    // Temporary coordinates for Kathmandu, Nepal.
    gps_coord.latitude = 27.7172;
    gps_coord.longitude = 85.3240;
    // Get horizontal coordinates
    horiz_coord horiz_coord = equatToHoriz(planet_position, gps_coord, gmst, 0.0f);
    // Move the telescope towards the planet.
    moveScope(horiz_coord);
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

    // Get current time, +0 seconds ahead and +60 seconds ahead.
    std::tm start_time = getCurrentUTC(0);
    std::tm end_time = getCurrentUTC(60);

    // "%20" represents a space character for our API query string
    std::ostringstream start_time_oss;
    start_time_oss << std::put_time(&start_time, "%Y-%m-%d%20%H:%M:%S");
    std::ostringstream end_time_oss;
    end_time_oss << std::put_time(&end_time, "%Y-%m-%d%20%H:%M:%S");

    log("CURRENT TIME: " + start_time_oss.str().substr(0, 10) + ", " + start_time_oss.str().substr(13, 21) + " UT");

    // Set query string parameters and construct the string
    query_string.start_time = start_time_oss.str();
    query_string.stop_time = end_time_oss.str();
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

std::tm getCurrentUTC(const int &offset)
{
    auto now = std::chrono::system_clock::now();
    // Convert to time_t object
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    time += offset;
    // Convert to tm struct, in UTC time.
    std::tm tm = *std::gmtime(&time);
    return tm;
}

float getGMST()
{
    log("\nOBTAINING GMST ...");
    usno_query_string query_string;
    std::string usno_response;

    log("GETTING CURRENT TIME ...");
    std::tm tm = getCurrentUTC(0);

    // String streams for our date and time
    std::ostringstream oss_date, oss_time;

    // Set format "YYYY-MM-DD HH:MM:SS" for start and end times.
    oss_date << std::put_time(&tm, "%Y-%m-%d");
    oss_time << std::put_time(&tm, "%H:%M:%S");
    // Into the query_string as an std::string
    query_string.startDate = oss_date.str();
    query_string.startTime = oss_time.str();

    query_string.constructQueryString();

    log("\nQUERY STRING: " + query_string.link + "\n");

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

// For timing
//  auto start = std::chrono::high_resolution_clock::now();
//  auto stop = std::chrono::high_resolution_clock::now();
//  auto duration = std::chrono::duration_cast<microseconds>(stop - start);
//  std::cout << duration.count() << "\n";