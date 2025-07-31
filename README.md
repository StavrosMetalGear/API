#  CompanyAPI – RESTful C++ Web API with Crow, SQLite & CMake

This project is a simple and powerful **REST API** built in modern **C++** using:

-  [Crow](https://github.com/CrowCpp/Crow) – Fast HTTP server for C++
-  SQLite – Lightweight embedded database
-  [vcpkg](https://github.com/microsoft/vcpkg) – C++ package manager
-  CMake – Cross-platform build system

---

##  Features

- `GET /employees` – List all employees
- `GET /employee/<id>` – Get one employee
- `POST /add` – Add an employee
- `DELETE /employee/<id>` – Delete an employee

All routes return **JSON** responses.

---

##  Project Structure

├── main.cpp # Main web API with routes
├── company.db # SQLite database file
├── CMakeLists.txt # Build instructions
├── crow_all.h # Crow single-header
├── build/ # CMake build output
---

## ⚙ Build Instructions

### 1. Clone & Install vcpkg

```bash
git clone https://github.com/microsoft/vcpkg
cd vcpkg
./bootstrap-vcpkg.bat
vcpkg install crow sqlite3 nlohmann-json

cd path/to/your/project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build

./build/CompanyAPI.exe
Visit it at: http://localhost:18080

curl http://localhost:18080/employees
curl -X POST http://localhost:18080/add -H "Content-Type: application/json" -d "{\"name\":\"Alice\",\"age\":30,\"address\":\"London\",\"salary\":50000}"
curl http://localhost:18080/employee/1
curl -X DELETE http://localhost:18080/employee/1

