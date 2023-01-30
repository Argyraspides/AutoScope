#include "celestial_api.h"

int main()
{
    // Idk what this does, but apparently it is important.
    curlpp::Cleanup myCleanup;

    // Required by curlpp to parse returned API data into a string format.
    std::ostringstream os;

    QUERY_STRING query_string;
    query_string.horizons.step_size = 20; 
    query_string.horizons.constructQueryString();

    os << curlpp::options::Url(std::string(query_string.horizons.link));

    std::string api_response = os.str();
    std::string start_delimiter = "$$SOE";
    std::string end_delimiter = "$$EOE";

    unsigned first = api_response.find(start_delimiter) + start_delimiter.length() + 1;
    unsigned last = api_response.find(end_delimiter) - end_delimiter.length() - 1;

    std::string celestial_data = api_response.substr(first, last - first);

    std::cout << celestial_data << std::endl;
}

