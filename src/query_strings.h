#ifndef query_strings_h
#define query_strings_h
#include <string>
#include <vector>

// The query string containing parameters for NASA's JPL Horizons API for planetary positional data.
struct horizons_query_string
{
    int step_size = 20;
    // Horizons API "base" link
    std::string link = "https://ssd.jpl.nasa.gov/api/horizons.api?";
    std::string format = "text";              // Get data as .txt
    std::string planet = "5";                 // Specifies target planet.
    std::string center = "3";                 // Planet from which to take the center of the coordinate system
    std::string reference_plane = "ECLIPTIC"; // Type of reference plane
    std::string coord_type = "CYLINDRICAL";   // Type of coordinate system to use
    std::string start_time = "2023-01-01";    // Start and stop times for positional data
    std::string stop_time = "2023-01-02";
    std::string quantities = "1," + std::to_string(step_size);

    // Takes all parameters and creates the final query string for the API.
    void constructQueryString()
    {

        std::vector<std::vector<std::string>> parameters{
            {"format=", format + "&"},
            {"COMMAND=", "'" + planet + "'&"},
            {"CENTER=", "'" + center + "'&"},
            {"REF_PLANE=", "'" + reference_plane + "'&"},
            {"COORD_TYPE=", "'" + coord_type + "'&"},
            {"START_TIME=", "'" + start_time + "'&"},
            {"STOP_TIME=", "'" + stop_time + "'&"},
            {"QUANTITIES=", "'" + quantities + "'"},
        };

        for (int i = 0; i < parameters.size(); i++)
        {
            link += (parameters[i][0] += parameters[i][1]);
        }
    }
};

// The query string containing parameters for the US Navl Observatory (USNO) API to recieve the current Greenwich Mean Sidereal Time.
// GMST returned here accounts for Earth's precession.
struct usno_query_string
{

    // Parameters for the USNO API.
    std::string startDate = "2020-03-04";
    std::string coords = "41.89,12.48";
    std::string increments = "1";
    std::string incrementSize = "1";
    std::string incrementUnits = "minutes";
    std::string startTime = "21:00:00";

    // The "base" link for the API
    std::string link =
        "https://aa.usno.navy.mil/api/siderealtime?";

    // Takes all parameters and creates the final query string for the API.
    void constructQueryString()
    {

        std::vector<std::vector<std::string>> parameters{
            {"date=", startDate + "&"},
            {"coords=", coords + "&"},
            {"reps=", increments + "&"},
            {"intv_mag=", incrementSize + "&"},
            {"intv_unit=", incrementUnits + "&"},
            {"time=", startTime}};

        for (int i = 0; i < parameters.size(); i++)
        {
            link += (parameters[i][0] += parameters[i][1]);
        }
    }
};
#endif
