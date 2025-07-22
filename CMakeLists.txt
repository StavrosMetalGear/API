cmake_minimum_required(VERSION 3.16)
project(CompanyAPI)

set(CMAKE_CXX_STANDARD 17)

# Add your source files here
add_executable(CompanyAPI
    main.cpp
    CompanyDB.cpp
    sqlite3.c
)

# Add include directories
target_include_directories(CompanyAPI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Use vcpkg dependencies
find_package(Threads REQUIRED)

