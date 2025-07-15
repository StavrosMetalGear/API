#include "CompanyDB.h"
#include <sqlite3.h>
#include <iostream>

namespace CompanyDB {

    // 1️⃣ Create the database file
    bool createDatabase(const std::string& filename) {
        sqlite3* db;
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_close(db);
        return true;
    }

    // 2️⃣ Create the COMPANY table
    bool createTable(const std::string& filename) {
        sqlite3* db;
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        const char* sql = R"(
        CREATE TABLE IF NOT EXISTS COMPANY (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            NAME TEXT NOT NULL,
            AGE INTEGER,
            ADDRESS TEXT,
            SALARY REAL
        );
    )";

        char* errMsg = nullptr;
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }

        sqlite3_close(db);
        return true;
    }

    // 3️⃣ Insert a new employee
    bool insertEmployee(const std::string& filename, const Employee& e) {
        sqlite3* db;
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        const char* sql = "INSERT INTO COMPANY (NAME, AGE, ADDRESS, SALARY) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_text(stmt, 1, e.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, e.age);
        sqlite3_bind_text(stmt, 3, e.address.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 4, e.salary);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    // 4️⃣ Show all employees
    std::vector<Employee> showData(const std::string& filename) {
        sqlite3* db;
        std::vector<Employee> employees;

        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return employees;
        }

        const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY;";
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return employees;
        }

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            Employee e;
            e.id = sqlite3_column_int(stmt, 0);
            e.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            e.age = sqlite3_column_int(stmt, 2);
            e.address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            e.salary = sqlite3_column_double(stmt, 4);
            employees.push_back(e);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return employees;
    }

    // 5️⃣ Delete an employee by ID
    bool deleteEmployee(const std::string& filename, int id) {
        sqlite3* db;
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        const char* sql = "DELETE FROM COMPANY WHERE ID = ?;";
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_int(stmt, 1, id);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return false;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    // 6️⃣ Search employees by name
    std::vector<Employee> findEmployeesByName(const std::string& filename, const std::string& name) {
        sqlite3* db;
        std::vector<Employee> employees;

        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return employees;
        }

        const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY WHERE NAME = ?;";
        sqlite3_stmt* stmt;

        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return employees;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            Employee e;
            e.id = sqlite3_column_int(stmt, 0);
            e.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            e.age = sqlite3_column_int(stmt, 2);
            e.address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            e.salary = sqlite3_column_double(stmt, 4);
            employees.push_back(e);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return employees;
    }

} // namespace CompanyDB
