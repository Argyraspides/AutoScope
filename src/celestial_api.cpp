#include "celestial_api.h"
#include <chrono>
using namespace std::chrono;

int main()
{
    // Represents the final responses of the API's. JPL Horizons returns in .txt format.
    std::ostringstream horizon_response;
    // US Naval Observatory API response to obtain GMST (Greenwhich Mean Sidereal Time)
    std::string usno_response;

    // Holds the and constructs the final query string for each API (USNO, NASA JPL)
    QUERY_STRING query_string;
    query_string.horizons.constructQueryString();
    query_string.usno.constructQueryString();

    // Call NASA's JPL Horizons API for planetary positional data, put into horizons_response.
    horizon_response << curlpp::options::Url(query_string.horizons.link);

    // USNO Returns as a JSON, so we configure curlpp to take json replies
    curlpp::Easy request;
    request.setOpt(curlpp::options::Url(query_string.usno.link));
    request.setOpt(curlpp::options::HttpHeader({"Accept: application/json"}));
    // Get the reply as a string so we can parse it as a JSON object with JSONCPP.
    request.setOpt(curlpp::options::WriteFunction([&usno_response](char *data, size_t size, size_t nmemb)
                                                  {
        usno_response.append(data, size * nmemb);
        return size * nmemb; }));

    auto start = high_resolution_clock::now();

    // API call.
    request.perform();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() << "\n";

    Json::Value jsonResponse;
    Json::Reader reader;
    // Convert to JSONPP json object.
    reader.parse(usno_response, jsonResponse);

    std::string api_response = horizon_response.str();
    std::string start_delimiter = "$$SOE";
    std::string end_delimiter = "$$EOE";

    unsigned first = api_response.find(start_delimiter) + start_delimiter.length() + 1;
    unsigned last = api_response.find(end_delimiter) - end_delimiter.length() - 1;

    std::string celestial_data = api_response.substr(first, last - first);

    std::cout << query_string.horizons.link << "\n";
    std::cout << query_string.usno.link << "\n";
}


