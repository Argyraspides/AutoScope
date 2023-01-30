#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

struct QUERY_STRING
{
public:
    struct horizons
    {
        int step_size = 20;
        // Horizons API "base" link
        std::string link = "https://ssd.jpl.nasa.gov/api/horizons.api?";
        std::string format = "text"; // Get data as .txt
        std::string command = "5";   // Specifies planet.
        std::string center = "@sun"; // Gives center of the
        std::string reference_plane = "ECLIPTIC";
        std::string coord_type = "CYLINDRICAL";
        std::string start_time = "2023-01-01";
        std::string stop_time = "2023-01-02";
        std::string quantities = "1," + std::to_string(step_size);

        std::vector<std::vector<std::string>> parameters{
            {"format=", format + "&"},
            {"COMMAND=", "'" + command + "'&"},
            {"CENTER=", "'" + center + "'&"},
            {"REF_PLANE=", "'" + reference_plane + "'&"},
            {"COORD_TYPE=", "'" + coord_type + "'&"},
            {"START_TIME=", "'" + start_time + "'&"},
            {"STOP_TIME=", "'" + stop_time + "'&"},
            {"QUANTITIES=", "'" + quantities + "'"},
        };

        void constructQueryString()
        {
            for (int i = 0; i < parameters.size(); i++)
            {
                link += (parameters[i][0] + parameters[i][1]);
            }
        }
    };
    horizons horizons;

    struct usno
    {
        std::vector<std::string> keyWords = {"date=", "coords=", "reps=", "intv_mag=", "intv_unit=", "time="};
        std::string startDate = "2020-03-04";
        std::string coords = "41.89,12.48";
        std::string increments = "90";
        std::string incrementSize = "5";
        std::string incrementUnits = "minutes";
        std::string startTime = "21:00:00";

        std::vector<std::vector<std::string>> parameters{
            {"date=", startDate + "&"},
            {"coords=", coords + "&"},
            {"reps=", increments + "&"},
            {"intv_mag=", incrementSize + "&"},
            {"intv_unit=", incrementUnits + "&"},
            {"time=", startTime}};

        std::string link =
            "https://aa.usno.navy.mil/api/siderealtime?";

        void constructQueryString()
        {
            for (int i = 0; i < parameters.size(); i++)
            {
                link += (parameters[i][0] + parameters[i][1]);
            }
        }
    };
    usno usno;
};

struct EARTH
{
};