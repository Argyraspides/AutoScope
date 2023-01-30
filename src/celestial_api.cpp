#include "celestial_api.h"

int main()
{
    // Required by curlpp to parse returned API data into a string format.
    std::ostringstream horizon_response;
    std::string usno_response;

    QUERY_STRING query_string;
    query_string.horizons.constructQueryString();
    query_string.usno.constructQueryString();

    horizon_response << curlpp::options::Url(query_string.horizons.link);

    curlpp::Easy request;
    request.setOpt(curlpp::options::Url(query_string.usno.link));
    request.setOpt(curlpp::options::HttpHeader({"Accept: application/json"}));

    request.setOpt(curlpp::options::WriteFunction([&usno_response](char *data, size_t size, size_t nmemb)
    {
        usno_response.append(data, size * nmemb);
        return size * nmemb;
    }));

    request.perform();

    Json::Value jsonResponse;
    Json::Reader reader;

    reader.parse(usno_response, jsonResponse);

    std::string api_response = horizon_response.str();
    std::string start_delimiter = "$$SOE";
    std::string end_delimiter = "$$EOE";

    unsigned first = api_response.find(start_delimiter) + start_delimiter.length() + 1;
    unsigned last = api_response.find(end_delimiter) - end_delimiter.length() - 1;

    std::string celestial_data = api_response.substr(first, last - first);
}