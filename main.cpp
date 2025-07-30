#include "crow_all.h"
#include <sqlite3.h>
#include "json.hpp"
using json = nlohmann::json;

int main() {
    crow::SimpleApp app;

    // Test route: /hello
    CROW_ROUTE(app, "/hello")([]() {
        return "Hello, world!";
        });

    // Test route: /employees (static)
    CROW_ROUTE(app, "/employees")([]() {
        crow::json::wvalue response;
        response["status"] = "success";
        response["message"] = "This will return employee list soon.";
        return response;
        });
    CROW_ROUTE(app, "/add").methods("POST"_method)
        ([](const crow::request& req) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int rc;

        // Parse JSON body
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        std::string name = body["name"].s();
        int age = body["age"].i();
        std::string address = body["address"].s();
        double salary = body["salary"].d();

        rc = sqlite3_open("company.db", &db);
        if (rc != SQLITE_OK) {
            return crow::response(500, "Failed to open database");
        }

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
    CROW_ROUTE(app, "/employee/<int>")
        .methods("GET"_method)
        ([](int id) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int rc = sqlite3_open("company.db", &db);
        if (rc != SQLITE_OK) {
            return crow::response(500, "Failed to open database");
        }

        const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY WHERE ID = ?;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare SQL query");
        }

        sqlite3_bind_int(stmt, 1, id);

        json employee;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            employee["id"] = sqlite3_column_int(stmt, 0);
            employee["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            employee["age"] = sqlite3_column_int(stmt, 2);
            employee["address"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            employee["salary"] = sqlite3_column_double(stmt, 4);
        }
        else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(404, "Employee not found");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(employee.dump());
            });
    CROW_ROUTE(app, "/employee/<int>")
        ([](int id) {
        sqlite3* db;
        sqlite3_stmt* stmt;
        int rc = sqlite3_open("company.db", &db);
        if (rc != SQLITE_OK) {
            return crow::response(500, "Failed to open database");
        }

        const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY WHERE ID = ?;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return crow::response(500, "Failed to prepare SQL query");
        }

        sqlite3_bind_int(stmt, 1, id);
        json row;

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            row["id"] = sqlite3_column_int(stmt, 0);
            row["name"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            row["age"] = sqlite3_column_int(stmt, 2);
            row["address"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            row["salary"] = sqlite3_column_double(stmt, 4);
        }
        else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return crow::response(404, "Employee not found");
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return crow::response(row.dump());
            });


    app.port(18080).multithreaded().run();
}

