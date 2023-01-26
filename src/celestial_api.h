#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

struct QUERY_STRING {
public:

    int step_size = 20;
    // Horizons API "base" link
    std::string link = "https://ssd.jpl.nasa.gov/api/horizons.api?";
    std::string format = "format=text"; // Get data as .txt
    std::string command = "COMMAND='5'"; // Specifies planet.
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
        quantities
    };

    void constructQueryString()
    {
        for(int i = 0; i < query_parameters.size() - 1; i++)
        {
            link += query_parameters[i];
            link += "&";
        }

        link += query_parameters[query_parameters.size() - 1];
    }

};

struct EARTH {


};