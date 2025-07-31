#include "crow_all.h"
#include <sqlite3.h>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    crow::SimpleApp app;

    // Test route
    CROW_ROUTE(app, "/hello")([]() {
        return "Hello, world!";
        });

    

    // POST /add - Add employee
    CROW_ROUTE(app, "/add").methods("POST"_method)
        ([](const crow::request& req) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int rc;

        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string name = body["name"].s();
        int age = body["age"].i();
        std::string address = body["address"].s();
        double salary = body["salary"].d();

        rc = sqlite3_open("company.db", &db);
        if (rc != SQLITE_OK) return crow::response(500, "Failed to open database");

        const char* sql = "INSERT INTO COMPANY (NAME, AGE, ADDRESS, SALARY) VALUES (?, ?, ?, ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare statement");
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, age);
        sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 4, salary);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Failed to execute insert");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return crow::response(201, "Employee added successfully");
            });

    // GET /employee/<id> - Fetch employee by ID
    CROW_ROUTE(app, "/employee/<int>").methods("GET"_method)
        ([](int id) {
        sqlite3* db;
        sqlite3_stmt* stmt;

        if (sqlite3_open("company.db", &db) != SQLITE_OK) {
            return crow::response(500, "Failed to open database");
        }

        const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY WHERE ID = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare SQL statement");
        }

        sqlite3_bind_int(stmt, 1, id);

        json emp;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            emp["id"] = sqlite3_column_int(stmt, 0);
            emp["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            emp["age"] = sqlite3_column_int(stmt, 2);
            emp["address"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            emp["salary"] = sqlite3_column_double(stmt, 4);
        }
        else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(404, "Employee not found");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(emp.dump());
            });
    // DELETE /employee/<id> - Delete employee by ID
    CROW_ROUTE(app, "/employee/<int>").methods("DELETE"_method)
        ([](int id) {
        sqlite3* db;
        sqlite3_stmt* stmt;

        if (sqlite3_open("company.db", &db) != SQLITE_OK) {
            return crow::response(500, "Failed to open database");
        }

        const char* sql = "DELETE FROM COMPANY WHERE ID = ?;";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare SQL statement");
        }

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(500, "Failed to delete employee");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(200, "Employee deleted successfully");
            });
    CROW_ROUTE(app, "/employees").methods("GET"_method)([] {
        sqlite3* db;
        sqlite3_stmt* stmt;

        int rc = sqlite3_open("company.db", &db);
        if (rc != SQLITE_OK) {
            return crow::response(500, "Failed to open database");
        }

        const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare SQL query");
        }

        nlohmann::json employees = nlohmann::json::array();

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            nlohmann::json emp;
            emp["id"] = sqlite3_column_int(stmt, 0);
            emp["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            emp["age"] = sqlite3_column_int(stmt, 2);
            emp["address"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            emp["salary"] = sqlite3_column_double(stmt, 4);
            employees.push_back(emp);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return crow::response(employees.dump());
        });

    app.port(18080).multithreaded().run();
}
