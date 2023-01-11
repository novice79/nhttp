cmake_minimum_required(VERSION 3.20)

project(test)

set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
# use all static lib
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif()
file(GLOB SOURCE_FILES  "*.cpp")
foreach(file ${SOURCE_FILES})
    get_filename_component(name "${file}" NAME_WE)
    add_executable(${name} "${file}")
    list(APPEND targets ${name})
endforeach()

# set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
# set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost REQUIRED COMPONENTS 
    date_time 
    filesystem 
    serialization
    atomic 
    json
    log_setup 
    log 
    program_options
    system 
    locale 
)
# find_package(Boost 1.77.0 REQUIRED COMPONENTS ALL)
# find_package (Boost 1.79.0 REQUIRED COMPONENTS ALL PATHS "$ENV{HOME}/boost" NO_DEFAULT_PATH)
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME}
#     Boost::date_time 
#     Boost::filesystem 
#     Boost::regex 
#     Boost::atomic 
#     Boost::chrono 
#     Boost::log_setup 
#     Boost::thread 
#     Boost::log 
#     Boost::system 
#     Boost::locale
# )
# https://cmake.org/cmake/help/latest/module/FindBoost.html
# message("Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
# message("Boost_LIBRARIES=${Boost_LIBRARIES}")
# set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL 1.1.1 REQUIRED)
message("OPENSSL_CRYPTO_LIBRARY=${OPENSSL_CRYPTO_LIBRARY}")
message("OPENSSL_LIBRARIES=${OPENSSL_LIBRARIES}")


# sudo apt-get install libz-dev
find_package(ZLIB REQUIRED)
message("ZLIB_LIBRARIES=${ZLIB_LIBRARIES}")

# only for windows
# target_link_libraries(${PROJECT_NAME} wsock32)
find_package(nhttp REQUIRED)
get_target_property(nhttp_INCLUDE_DIRS nhttp::nhttp INTERFACE_INCLUDE_DIRECTORIES )
message("nhttp_INCLUDE_DIRS=${nhttp_INCLUDE_DIRS}")
get_target_property(nhttp_LIBRARIES nhttp::nhttp IMPORTED_LOCATION_RELEASE )
message("nhttp_LIBRARIES=${nhttp_LIBRARIES}")
message("nhttp_VERSION=${nhttp_VERSION}")

set(STATIC_LINK " -s -static  -static-libstdc++ -static-libgcc")
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    # set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY LINK_FLAGS " ${STATIC_LINK}")
    string(APPEND CMAKE_EXE_LINKER_FLAGS "${STATIC_LINK}")
endif ()
foreach(t ${targets})
    target_link_libraries(
        ${t}
        ${Boost_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${ZLIB_LIBRARIES}
        # ZLIB::ZLIB
        nhttp::nhttp
        ${CMAKE_DL_LIBS} 
    )
    if(WIN32)
        target_link_libraries(${t} ws2_32 wsock32)
    endif()
    install(TARGETS ${t} RUNTIME DESTINATION bin)
endforeach()

# otool -L  dist/app


