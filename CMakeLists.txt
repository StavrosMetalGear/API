cmake_minimum_required(VERSION 3.15)
project(CompanyAPI)

set(CMAKE_CXX_STANDARD 17)

# Enable vcpkg integration
if(DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Using vcpkg toolchain from ${CMAKE_TOOLCHAIN_FILE}")
endif()

# Add your source files
add_executable(CompanyAPI
    main.cpp
    CompanyDb.cpp
    sqlite3.c
)

# Link Crow dependencies
find_package(Boost REQUIRED)
target_include_directories(CompanyAPI PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(CompanyAPI PRIVATE Boost::boost)

# Include crow_all.h, json.hpp, etc.
target_include_directories(CompanyAPI PRIVATE ${CMAKE_SOURCE_DIR})


