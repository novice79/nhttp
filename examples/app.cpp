/* This is a simple HTTP(S) web server much like Python's SimpleHTTPServer */

#include <nhttp/App.h>


int main(int argc, char **argv) {

    int port = 3000;
    // auto app = uWS::App();
    // tls http server or not
    auto app = uWS::SSLApp({
	  .key_file_name = "misc/key.pem",
	  .cert_file_name = "misc/cert.pem"
	});
    app
    .get("/hello",[](auto *res, auto *req) {
        res->writeHeader("Content-Type", "text/html");
        res->end("hello world");
    })
    .post("/update",[](auto *res, auto *req) {
        res->end("test post api");
    })
    .put("/put",[](auto *res, auto *req) {
        res->end("test put api");
    })
    .del("/delete",[](auto *res, auto *req) {
        res->end("test delete api");
    })
    .serve_dir("/", Util::exe_path(argv[0]) / "www")
    .serve_dir("/store/", Util::exe_path(argv[0]) / "store")
    .listen(port, [port](auto *token) {
        if (token) {
            std::cout << "Serving static files"  << " over HTTP a " << port << std::endl;
        }
    }).run();
    

    std::cout << "Failed to listen to port " << port << std::endl;
}
