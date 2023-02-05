#include "celestial_api.h"
#define print(x) std::cout << x << "\n"

// Frequency of tracking an object, given in milliseconds.
const int tracking_frequency = 20000;

int main()
{
    // Initialize the database for planetary codes and extrasolar object codes
    initializeMaps();
    bool running = true;
    while (running)
    {
    restart:
        std::string body;
        body_type body_type;
        print("ENTER TARGET BODY");
        std::cin >> body;

        bool valid_body = false;

        if (horizons_map.count(body) > 0)
        {
            valid_body = true;
            body_type = body_type::planet;
        }
        else if (ned_map.count(body) > 0)
        {
            valid_body = true;
            body_type = body_type::extra_solar;
        }

        if (valid_body)
        {
            print("STARTING ...");

            focusBody(body, body_type);

            while (true)
            {
                // Continue tracking the planet
                print("FOCUSSING ... ENTER 'C' TO EXIT AND SELECT ANOTHER BODY, OR 'X' TO FOCUS THE PLANET AGAIN");
                char inp;
                std::cin >> inp;
                if (inp == 'C' || inp == 'c')
                {
                    break;
                }
                else if (inp == 'X' || inp == 'x')
                {
                    focusBody(body, body_type);
                }
            }
            std::cin.clear();
        }
        else
        {
            print("ERROR: INVALID INPUT");
            goto restart;
        }
    }
}

// Sends control to motors to orient the telescope
void moveScope(const horiz_coord &horiz_coord)
{
}

// Orients the telescope to point at the desired planet
void focusBody(const std::string &body, const body_type &body_type)
{
    equat_coord body_position;
    if (body_type == body_type::planet)
        body_position = getPlanetPosition(body);
    else if (body_type == body_type::extra_solar)
        body_position = getExtrasolarBodyPosition(body);

    // Get the current Greenwich Mean Sidereal Time.
    float gmst = getGMST();
    // gps_coord gps_coord = getGPSCoord();
    gps_coord gps_coord;
    // Temporary coordinates for Auckland, NZ
    gps_coord.latitude = -36.8509;
    gps_coord.longitude = 174.7645;
    // Get horizontal coordinates
    horiz_coord horiz_coord = equatToHoriz(body_position, gps_coord, gmst, 0.0f);

    print("Equatorial Coordinates: RA: " + std::to_string(body_position.right_ascension_degrees) + " Dec: " + std::to_string(body_position.declination));
    print("Horizontal Coordinates: Alt: " + std::to_string(horiz_coord.altitude) + " Az: " + std::to_string(horiz_coord.azimuth));
    print("GPS Position: " + std::to_string(gps_coord.latitude) + " Lat, " + std::to_string(gps_coord.longitude) + " Long");

    // Move the telescope towards the planet.
    moveScope(horiz_coord);
}

// Tracks the current planet across its diurnal path in the sky
void track()
{
}

// Obvious enough. Returns in equatorial coordinates (Right Ascension and Declination)
equat_coord getPlanetPosition(const std::string &planet)
{
    // Represents the final responses of the API. JPL Horizons returns in .txt format.
    std::ostringstream horizon_response;
    equat_coord final_coordinates;

    // Holds the and constructs the final query string for each API (USNO, NASA JPL)
    horizons_query_string query_string;

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

    // Set query string parameters and construct the string
    query_string.start_time = start_time_oss.str();
    query_string.stop_time = end_time_oss.str();
    query_string.planet = planet;
    query_string.constructQueryString();

    // Call NASA's JPL Horizons API for planetary positional data, put into horizons_response.
    horizon_response << curlpp::options::Url(query_string.link);

    // Retrieve only part of the table that contains the actual data.
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

    return final_coordinates;
}

// Returns equatorial coordinates about a target body outside of the solar system. Returns in equatorial coordinates (Right Ascension and Declination)
// Utilizes NASA's NED API.
equat_coord getExtrasolarBodyPosition(const std::string &extrasolar_body)
{

    ned_query_string query_string;
    equat_coord final_coordinates;

    std::ostringstream ned_response;

    query_string.body_name = extrasolar_body;
    query_string.constructQueryString();
    ned_response << curlpp::options::Url(query_string.link);

    Json::Value json_response;
    Json::Reader reader;
    // Parse API response to json object.
    reader.parse(ned_response.str(), json_response);

    // Extract right ascension and declination from API response.
    std::string right_ascension = json_response["Preferred"]["Position"]["RA"].asString();
    std::string declination = json_response["Preferred"]["Position"]["Dec"].asString();

    // The API returns results already converting R.A. into degrees.
    final_coordinates.declination = std::stof(declination);
    final_coordinates.right_ascension_degrees = std::stof(right_ascension);

    return final_coordinates;
}

// Obtains the current UTC + "offset" seconds ahead. E.g. offset = 60 gives back UTC time one minute ahead of now.
std::tm getCurrentUTC(const int &offset)
{
    auto now = std::chrono::system_clock::now();
    // Convert to time_t object
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    time += offset;
    return *std::gmtime(&time);
}

// Obtains the current Greenwich Mean Sidereal Time from the USNO API. Accounts for Earth's precession.
float getGMST()
{
    usno_query_string query_string;
    std::string usno_response;

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
    // Parse API response to json object.
    reader.parse(usno_response, json_response);

    // Extract GMST from response.
    std::string gmst = json_response["properties"]["data"][0]["gmst"].asString();

    // Return GMST in degrees.
    float hours = std::stof(gmst.substr(0, 2));
    float minutes = std::stof(gmst.substr(3, 4));
    float seconds = std::stof(gmst.substr(6, 10));
    print("Current GMST: " + gmst);

    return (hours + minutes / 60.0f + seconds / 3600) * 15.0f;
}

void initializeMaps()
{
    for (int i = 0; i < horizons_planet_codes.size(); i++)
    {
        horizons_map[horizons_planet_codes[i]] = i;
    }

    for (int i = 0; i < ned_body_codes.size(); i++)
    {
        ned_map[ned_body_codes[i]] = i;
    }
}
// For timing
//  auto start = std::chrono::high_resolution_clock::now();
//  auto stop = std::chrono::high_resolution_clock::now();
//  auto duration = std::chrono::duration_cast<microseconds>(stop - start);
//  std::cout << duration.count() << "\n";