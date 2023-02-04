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

// Converts from equatorial to horizontal coordinates, accounting for observers elevation above sea level.
horiz_coord equatToHoriz(
    equat_coord &eq_coo,
    gps_coord &gps_coo,
    float &GMST,
    const float &elevation)
{
    // GMST given in degrees
    float local_sidereal_time = GMST + gps_coo.longitude;
    local_sidereal_time = fmod(local_sidereal_time, 360);

    float hour_angle = local_sidereal_time - eq_coo.right_ascension_degrees;

    if (hour_angle < 0)
        hour_angle += 360;

    // temporarily convert our coordinate values to radians.
    float conv = PI / 180.0f;
    eq_coo.declination *= conv;
    gps_coo.latitude *= conv;
    hour_angle *= conv;

    float altitude =
        asin(
            sin(eq_coo.declination) * sin(gps_coo.latitude) +
            cos(eq_coo.declination) * cos(gps_coo.latitude) * cos(hour_angle));
    // Convert to degrees
    altitude *= (180.0f / PI);

    float azimuth = atan2(
                        sin(hour_angle),
                        cos(hour_angle) * sin(gps_coo.latitude ) - tan(eq_coo.declination) * cos(gps_coo.latitude));

    // We get an output of -180 to 180 with atan2, to get 0 to 360 as azimuth normally is, simply add 180 after
    // converting to degrees.
    azimuth *= (180.0f / PI);
    azimuth += 180;

    horiz_coord horizontal_coordinates;
    horizontal_coordinates.altitude = altitude;
    horizontal_coordinates.azimuth = azimuth;

    return horizontal_coordinates;
}

#endif
