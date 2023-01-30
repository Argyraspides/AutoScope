#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

struct QUERY_STRING
{
public:
    struct horizons
    {
        int step_size = 20;
        // Horizons API "base" link
        std::string link = "https://ssd.jpl.nasa.gov/api/horizons.api?";
        std::string format = "format=text";   // Get data as .txt
        std::string command = "COMMAND='5'";  // Specifies planet.
        std::string center = "CENTER='@sun'"; // Gives center of the
        std::string reference_plane = "REF_PLANE='ECLIPTIC'";
        std::string coord_type = "COORD_TYPE='CYLINDRICAL'";
        std::string start_time = "START_TIME='2023-01-01";
        std::string stop_time = "STOP_TIME='2023-01-02'";
        std::string quantities = "QUANTITIES='1," + std::to_string(step_size) + "'";

        std::vector<std::string> query_parameters{
            format,
            command,
            center,
            reference_plane,
            coord_type,
            start_time,
            stop_time,
            quantities};

        void constructQueryString()
        {
            for (int i = 0; i < query_parameters.size() - 1; i++)
            {
                link += query_parameters[i];
                link += "&";
            }

            link += query_parameters[query_parameters.size() - 1];
        }
    };
    horizons horizons;

    struct usno {

        std::string keyWords[] = {"coords=", "reps=", "intv_mag=", "intv_unit=", "time="};
        std::string startDate = "2020-03-04";
        std::string coords = "41.89, 12.48";
        std::string increments = "90";
        std::string incrementSize = "5";
        std::string incrementUnits = "minutes";
        std::string startTime = "21:00:00";

        std::string link = 
        "https://aa.usno.navy.mil/api/siderealtime?date="
        "2020-03-04&"
        "coords=41.89, 12.48&"
        "reps=90 &"
        "intv_mag=5&"
        "intv_unit=minutes &"
        "time=21:00:00";


    };
    usno usno;

};

struct EARTH
{
};