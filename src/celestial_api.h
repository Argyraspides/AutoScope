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
                link += (parameters[i][0] += parameters[i][1]);
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
                link += (parameters[i][0] += parameters[i][1]);
            }
        }
    };
    usno usno;
};

struct coord
{
public:
    struct equat_coord
    {
        // Given as Hours:Minutes:Seconds
        std::vector<float> right_ascension = {0.0f, 0.0f, 0.0f};
        float declination = 0.0f;

        float right_ascension_degrees =
            (right_ascension[0] * 15.0f) + (right_ascension[1] / 4.0f) + (right_ascension[2] / 240.0f);
    };
    equat_coord equatorial;

    struct horiz_coord
    {
        // Given as Hours:Minutes:Seconds
        std::vector<float> azimuth = {0.0f, 0.0f, 0.0f};
        float altitude = 0.0f;

        float azimuth_degrees = (azimuth[0] * 15.0f) + (azimuth[1] / 4.0f) + (azimuth[2] / 240.0f);
    };
    horiz_coord horizontal;

    struct gps_coord
    {
        float latitude = 0.0f, longitude = 0.0f;
    }
};

struct MATH
{

    horiz_coord equatToHoriz(
        const equat_coord &eq_coo,
        const gps_coord &gps_coo,
        const float &GMST const float &altitude)
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
            1.0f / (cos(altitude) * cos(gps_coo.latitude) * cos(local_hour_angle) +
                    sin(gps_coo.latitude) * sin(eq_coo.declination));

        altitude *= correction_factor;
        azimuth *= correction_factor;

        horiz_coord horizontal_coordinates;
        horizontal_coordinates.altitude = altitude;
        horizontal_coordinates.azimuth_degrees = azimuth;

        return horizontal_coordinates;
    }
};

struct EARTH
{
};