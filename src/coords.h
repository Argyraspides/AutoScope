#ifndef coords_h
#define coords_h
#include <vector>
#include <cmath>

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

#endif
