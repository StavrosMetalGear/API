#include "CompanyDB.h"
#include <iostream>

int main() {
    const std::string dbfile = "company.db";

    CompanyDB::createDatabase(dbfile);
    CompanyDB::createTable(dbfile);

    Employee e1 = { 0, "Alice", 30, "New York", 50000.0 };
    Employee e2 = { 0, "Bob", 45, "Los Angeles", 60000.0 };

    CompanyDB::insertEmployee(dbfile, e1);
    CompanyDB::insertEmployee(dbfile, e2);

    std::cout << "All employees:" << std::endl;
    auto employees = CompanyDB::showData(dbfile);
    for (const auto& e : employees) {
        std::cout << e.id << " | " << e.name << " | " << e.age
            << " | " << e.address << " | " << e.salary << std::endl;
    }

    std::cout << "\nDeleting employee with ID 1..." << std::endl;
    CompanyDB::deleteEmployee(dbfile, 1);

    std::cout << "\nSearch for Bob:" << std::endl;
    auto found = CompanyDB::findEmployeesByName(dbfile, "Bob");
    for (const auto& e : found) {
        std::cout << e.id << " | " << e.name << " | " << e.age
            << " | " << e.address << " | " << e.salary << std::endl;
    }

    return 0;
}


