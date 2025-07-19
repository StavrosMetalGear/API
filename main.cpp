#include "CompanyDB.h"
#include <iostream>
#include "crow_all.h"

int main() {
    const std::string dbfile = "company.db";
    const std::string jsonfile = "employees.json";

    CompanyDB::createDatabase(dbfile);
    CompanyDB::createTable(dbfile);

    Employee e1 = { 0, "Alice", 30, "New York", 50000 };
    Employee e2 = { 0, "Bob", 45, "Los Angeles", 60000 };

    CompanyDB::insertEmployee(dbfile, e1);
    CompanyDB::insertEmployee(dbfile, e2);

    std::cout << "\nExporting employees to JSON..." << std::endl;
    CompanyDB::exportToJson(dbfile, jsonfile);

    std::cout << "\nDeleting all employees (for demonstration)..." << std::endl;
    CompanyDB::deleteEmployee(dbfile, 1);
    CompanyDB::deleteEmployee(dbfile, 2);

    std::cout << "\nRe-importing employees from JSON..." << std::endl;
    CompanyDB::importFromJson(dbfile, jsonfile);

    std::cout << "\nFinal list of employees:" << std::endl;
    auto employees = CompanyDB::showData(dbfile);
    for (const auto& e : employees) {
        std::cout << e.id << " | " << e.name << " | " << e.age
            << " | " << e.address << " | " << e.salary << std::endl;
    }

    return 0;
}


