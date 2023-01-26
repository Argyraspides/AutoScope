#include "celestial_api.h"

int main()
{
    // Idk what this does, but apparently it is important.
    curlpp::Cleanup myCleanup;

    // Required by curlpp to parse returned API data into a string format.
    std::ostringstream os;

    QUERY_STRING query_string;
    query_string.constructQueryString();

    os << curlpp::options::Url(std::string(query_string.link));
    std::string CELESTIAL_DATA = os.str();
    std::cout << CELESTIAL_DATA;
}