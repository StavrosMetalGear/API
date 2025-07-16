#pragma once
#include <string>
#include <vector>

// Model
struct Employee {
    int id;
    std::string name;
    int age;
    std::string address;
    double salary;
};

namespace CompanyDB {

    // Database operations
    bool createDatabase(const std::string& filename);
    bool createTable(const std::string& filename);

    // Data operations
    bool insertEmployee(const std::string& filename, const Employee& e);
    std::vector<Employee> showData(const std::string& filename);
    bool deleteEmployee(const std::string& filename, int id);
    std::vector<Employee> findEmployeesByName(const std::string& filename, const std::string& name);
    bool exportToJson(const std::string& dbfile, const std::string& jsonFile);
    bool importFromJson(const std::string& dbfile, const std::string& jsonFile);

}
#pragma once
