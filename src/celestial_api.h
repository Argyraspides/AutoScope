#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <cmath>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

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
    std::string start_time = "2023-01-01";
    std::string stop_time = "2023-01-02";
    std::string quantities = "1," + std::to_string(step_size); // Number of measurements to be returned from between start
    // and end time. E.g. If start->stop was 24 hours, quantities was "1,24", then 24 measurements would be given back,
    // i.e. the positional data once per hour.

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

struct usno_query_string
{
    std::vector<std::string> keyWords = {"date=", "coords=", "reps=", "intv_mag=", "intv_unit=", "time="};
    std::string startDate = "2020-03-04";
    std::string coords = "41.89,12.48";
    std::string increments = "90";
    std::string incrementSize = "5";
    std::string incrementUnits = "minutes";
    std::string startTime = "21:00:00";

    std::string link =
        "https://aa.usno.navy.mil/api/siderealtime?";

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

struct equat_coord
{
    // Given as Hours:Minutes:Seconds
    std::vector<float> right_ascension = {0.0f, 0.0f, 0.0f};
    float declination = 0.0f;
    float right_ascension_degrees = 0;

    void update()
    {
        right_ascension_degrees =
            (right_ascension[0] + right_ascension[1] / 60.0f + right_ascension[2] / 3600.0f) * 15;
    }
};

struct horiz_coord
{
    float altitude = 0.0f;
    float azimuth = 0;
};

struct gps_coord
{
    float latitude = 0.0f, longitude = 0.0f;
};

horiz_coord equatToHoriz(
    const equat_coord &eq_coo,
    const gps_coord &gps_coo,
    const float &GMST,
    const float &elevation)
{
    // GMST given in degrees
    float local_sidereal_time = GMST + gps_coo.longitude;
    float hour_angle = local_sidereal_time - eq_coo.right_ascension_degrees;

    // Our altitude before correcting for elevation above sea level.
    float altitude =
        asin(
            sin(eq_coo.declination) * sin(gps_coo.latitude) +
            cos(eq_coo.declination) * cos(gps_coo.latitude) * cos(hour_angle));

    // Our altitude before correcting for elevation above sea level.
    float azimuth =
        asin(
            -sin(hour_angle) * cos(eq_coo.declination) / cos(altitude));

    // Our correction factor for elevation above sea level
    float local_hour_angle = GMST + gps_coo.longitude - eq_coo.right_ascension_degrees;
    float correction_factor =
        1.0f / (cos(elevation) * cos(gps_coo.latitude) * cos(local_hour_angle) +
                sin(gps_coo.latitude) * sin(eq_coo.declination));

    altitude *= correction_factor;
    azimuth *= correction_factor;

    horiz_coord horizontal_coordinates;
    horizontal_coordinates.altitude = altitude;
    horizontal_coordinates.azimuth = azimuth;

    return horizontal_coordinates;
}
