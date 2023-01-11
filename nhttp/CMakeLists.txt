cmake_minimum_required(VERSION 3.20)

project(nhttp LANGUAGES C CXX VERSION 1.0.0)
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
add_compile_definitions(LIBUS_USE_OPENSSL LIBUS_USE_ASIO)
file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp" 
    "src/*.c" 
)
file(GLOB HEADER_FILES 
    "include/*.h" 
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif()
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")
include_directories("include")
include_directories("src")
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost 1.77.0 REQUIRED COMPONENTS 
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
# include_directories(${Boost_INCLUDE_DIRS})
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC
    ${Boost_INCLUDE_DIRS}
)
find_package(OpenSSL 1.1.1 REQUIRED)
find_package(ZLIB REQUIRED)
target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC
    ${Boost_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${ZLIB_LIBRARIES}
)
INSTALL(
    TARGETS ${PROJECT_NAME} 
    EXPORT nhttp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION "include/nhttp"
)
install(EXPORT nhttp
    FILE nhttp.cmake
    NAMESPACE nhttp::
    DESTINATION lib/cmake/nhttp
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "nhttpConfigVersion.cmake"
    VERSION ${nhttp_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES 
    "nhttpConfig.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/nhttpConfigVersion.cmake"
    DESTINATION lib/cmake/nhttp
)

