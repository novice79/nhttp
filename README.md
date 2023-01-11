# nhttp for novice http

Modified from [uWebSockets](https://github.com/uNetworking/uWebSockets)

1. change it to cmake build  
2. strip other build options except openssl+asio
3. add serve static files in specified dir(s) api

dependence:  

    zlib
    openssl
    boost

# Usage

```c++
    uWS::SSLApp({
	  .key_file_name = "misc/key.pem",
	  .cert_file_name = "misc/cert.pem"
	});
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
    // https://localhost:3000/  -- serve files(default index.html) in www dir within exe path
    .serve_dir("/", Util::exe_path(argv[0]) / "www")
    // https://localhost:3000/store/ -- serve files(default index.html) in store dir within exe path
    .serve_dir("/store/", Util::exe_path(argv[0]) / "store")
    .listen(3000, [](auto *token) {
        if (token) {
            std::cout << "Serving static files"  << " over HTTP a " << 3000 << std::endl;
        }
    }).run();

```