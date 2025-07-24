cmake_minimum_required(VERSION 3.16)
project(CompanyAPI)

set(CMAKE_CXX_STANDARD 17)

add_executable(CompanyAPI
    main.cpp
    CompanyDB.cpp
)

target_include_directories(CompanyAPI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


