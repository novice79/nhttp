#include <nhttp/App.h>
#include <boost/json.hpp>
/* Note that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support */
using namespace boost::json;
int main() {
	value jv = {
    { "pi", 3.141 },
    { "happy", true },
    { "name", "Boost" },
    { "nothing", nullptr },
    { "answer", {
        { "everything", 42 } } },
    {"list", {1, 0, 2}},
    {"object", {
        { "currency", "USD" },
        { "value", 42.99 }
            } }
    };
	std::string s = serialize( jv ); 
	std::cout<< s << std::endl;
	/* Overly simple hello world app */
	uWS::SSLApp({
	  .key_file_name = "../misc/key.pem",
	  .cert_file_name = "../misc/cert.pem"
	}).get("/*", [](auto *res, auto */*req*/) {
	    res->end("Hello world from novice!");
	}).listen(3000, [](auto *listen_socket) {
	    if (listen_socket) {
			std::cout << "Listening on port " << 3000 << std::endl;
	    }
	}).run();

	std::cout << "Failed to listen on port 3000" << std::endl;
}
