#ifndef coords_h
#define coords_h
#include <vector>
#include <cmath>
#define PI 3.14159265359

// Equatorial Coordinates
struct equat_coord
{
    // Given as Hours:Minutes:Seconds
    std::vector<float> right_ascension = {0.0f, 0.0f, 0.0f};
    float declination = 0.0f;
    float right_ascension_degrees = 0;

    // Updates the right ascension in degrees
    void update()
    {
        right_ascension_degrees =
            (right_ascension[0] + right_ascension[1] / 60.0f + right_ascension[2] / 3600.0f) * 15;
    }
};

// Horizontal coordinates
struct horiz_coord
{
    float altitude = 0.0f;
    float azimuth = 0;
};

// GPS coordinates
struct gps_coord
{
    float latitude = 0.0f, longitude = 0.0f;
};

// Converts from equatorial to horizontal coordinates.
horiz_coord equatToHoriz(
    const equat_coord &eq_coo,
    const gps_coord &gps_coo,
    const float &GMST,
    const float &elevation)
{
    // GMST given in degrees
    float local_sidereal_time = GMST + gps_coo.longitude;
    float hour_angle = local_sidereal_time - eq_coo.right_ascension_degrees;

    float altitude =
        asin(
            sin(eq_coo.declination * PI / 180.0f) * sin(gps_coo.latitude * PI / 180.0f) +
            cos(eq_coo.declination * PI / 180.0f) * cos(gps_coo.latitude * PI / 180.0f) * cos(hour_angle * PI / 180.0f));

    float azimuth =
        atan2(sin(hour_angle * PI / 180.0f),
              cos(eq_coo.declination * PI / 180.0f) * cos(gps_coo.latitude * PI / 180.0f) - sin(eq_coo.declination * PI / 180.0f) * sin(gps_coo.latitude * PI / 180.0f) * cos(hour_angle * PI / 180.0f));

    azimuth = fmod((azimuth + 2 * PI), (2 * PI));

    // Our correction factor for elevation above sea level
    float local_hour_angle = GMST + gps_coo.longitude - eq_coo.right_ascension_degrees;
    float correction_factor =
        1.0f / (cos(elevation) * cos(gps_coo.latitude * PI / 180.0f) * cos(local_hour_angle * PI / 180.0f) +
                sin(gps_coo.latitude * PI / 180.0f) * sin(eq_coo.declination * PI / 180.0f));

    altitude *= correction_factor;
    azimuth *= correction_factor;

    horiz_coord horizontal_coordinates;
    horizontal_coordinates.altitude = altitude;
    horizontal_coordinates.azimuth = azimuth;

    return horizontal_coordinates;
}

#endif
