#include<iostream>
#include<stdio.h>
#include<sqlite3.h>
#include<vector>
#include<string>
using namespace std;
static int createDataBase(const char* s);
static int createTable(const char* s);
static int callBack(void* NotUsed, int argc, char** argv, char** azColName);
static int deleteData(const char* s);
static int insertData(const char* s);
static int queryOneItem(const char* s);
static int showData(const char* s);

int main() {
	return 0;
}
static int createDataBase(const char* s) {
	sqlite3* db;
	int rc = sqlite3_open(s, &db);
	char* zErrMsg = 0;
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return rc;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
static int createTable(const char* s) {
	sqlite3* db;
	char* zErrMsg = 0;
	int rc = sqlite3_open(s, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return rc;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}
	const char* sql = "CREATE TABLE IF NOT EXISTS COMPANY("  \
		"ID INT PRIMARY KEY     NOT NULL," \
		"NAME           TEXT    NOT NULL," \
		"AGE            INT(2)     NOT NULL," \
		"ADDRESS        CHAR(50)," \
		"SALARY         REAL );";
	rc = sqlite3_exec(db, sql, callBack, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
static int callBack(void* NotUsed, int argc, char** argv, char** azColName) {
	for (int i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
static int deleteData(const char* s) {
	sqlite3* db;
	char* zErrMsg = 0;
	int rc = sqlite3_open(s, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return rc;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}
	const char* sql = "DELETE FROM COMPANY WHERE ID=2;";
	rc = sqlite3_exec(db, sql, callBack, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Record deleted successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
static int insertData(const char* s) {
    cout << "The insertData function is called" << endl;

    vector<int> idPrimaryKey;
    vector<string> compNames;
    vector<int> employeAges;
    vector<string> employeAddresses;
    vector<int> employeSalaries;

    int i = 1;
    while (i == 1) {
        cout << "Please insert the name of the employee: ";
        string name;
        cin.ignore(); // Make sure to clear any leftover newline
        getline(cin, name);

        cout << "Please insert the age of the employee: ";
        int age;
        cin >> age;
        cin.ignore();

        cout << "Please insert the address of the employee: ";
        string address;
        getline(cin, address);

        cout << "Please insert the salary of the employee: ";
        int salary;
        cin >> salary;
        cin.ignore();

        idPrimaryKey.push_back(idPrimaryKey.size() + 1);
        compNames.push_back(name);
        employeAges.push_back(age);
        employeAddresses.push_back(address);
        employeSalaries.push_back(salary);

        cout << "Do you want to insert another employee? (1 = Yes, 0 = No): ";
        cin >> i;
    }

    sqlite3* db;
    sqlite3_stmt* pStmt;
    int rc = sqlite3_open(s, &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }
    else {
        cout << "Opened database successfully" << endl;
    }

    const char* sql = "INSERT INTO COMPANY (ID, NAME, AGE, ADDRESS, SALARY) VALUES (?, ?, ?, ?, ?);";

    for (size_t j = 0; j < compNames.size(); j++) {
        rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Prepare error: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return 1;
        }

        sqlite3_bind_int(pStmt, 1, idPrimaryKey[j]);
        sqlite3_bind_text(pStmt, 2, compNames[j].c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(pStmt, 3, employeAges[j]);
        sqlite3_bind_text(pStmt, 4, employeAddresses[j].c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(pStmt, 5, employeSalaries[j]);

        rc = sqlite3_step(pStmt);
        if (rc != SQLITE_DONE) {
            cerr << "Execution error: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(pStmt);
            sqlite3_close(db);
            return 1;
        }

        cout << "Record inserted successfully: " << compNames[j] << endl;

        sqlite3_finalize(pStmt);
    }

    sqlite3_close(db);
    return 0;
}
static int queryOneItem(const char* s) {
    cout << "Enter the name to search: ";
    string searchName;
    cin.ignore();
    getline(cin, searchName);

    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc = sqlite3_open(s, &db);
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return rc;
    }

    const char* sql = "SELECT ID, NAME, AGE, ADDRESS, SALARY FROM COMPANY WHERE NAME = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_text(stmt, 1, searchName.c_str(), -1, SQLITE_TRANSIENT);

    cout << "ID | NAME | AGE | ADDRESS | SALARY" << endl;
    cout << "-------------------------------------" << endl;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);
        const unsigned char* address = sqlite3_column_text(stmt, 3);
        double salary = sqlite3_column_double(stmt, 4);

        cout << id << " | "
            << (name ? reinterpret_cast<const char*>(name) : "NULL") << " | "
            << age << " | "
            << (address ? reinterpret_cast<const char*>(address) : "NULL") << " | "
            << salary << endl;
    }

    if (rc != SQLITE_DONE) {
        cerr << "Step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
static int showData(const char* s) {}

