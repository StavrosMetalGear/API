#include "crow_all.h"

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

    app.port(18080).multithreaded().run();
}

