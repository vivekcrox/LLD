#include<iostream>
#include<mutex>
using namespace std;

// Problem 1: Singleton - Logger System
/**
 * Implement a thread-safe Logger that writes to console
 * Requirements: 
 * - Only one instance across application
 * - Thread-safe getInstance()
 * - Method: log(level, message)
 * - Support levels: INFO, WARNING & ERROR
 */
class Logger {

private: 
    static Logger* instance;
    static mutex mtx;

    Logger() {}

public: 
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    static Logger* getInstance() {
        if(instance == nullptr) {
            lock_guard<mutex> lock(mtx);
            if(instance == nullptr) {
                instance = new Logger();
            }
        }
        return instance;
    }

    void log(const string& level, const string& message) {
        lock_guard<mutex> lock(mtx);
        cout << "[" << level << "] " << message << "\n";
    }
};

Logger* Logger::instance = nullptr;
mutex Logger::mtx;

void testSingleton() {

    cout << "Testing Singleton Design Pattern" << "\n";

    Logger* logger1 = Logger::getInstance();
    Logger* logger2 = Logger::getInstance();

    cout << "Same instance ?? " << (logger1 == logger2 ? "Yes" : "No") << "\n \n";

    logger1->log("INFO", "Application started!");
    logger2->log("ERROR", "Something went wrong!");

    cout << "\n\n";
}

// ===========================================================================

// Problem 2: Factory - Database Connection Factory
/**
 * Create a factory that produces different database connections
 * Support: MySQL, PostgreSQL, MongoDB
 * Each connection has connect() and execute() methods
 */

// unique_ptr<Database> --> Smart pointer, automatic memory management
// make_unique<MySQL>()	--> Creates a unique_ptr safely without new

class Database {
public: 
    virtual ~Database() = default;
    virtual void connect() = 0;
    virtual void execute(const string& query) = 0;
};

class MySQL : public Database {
public: 
    void connect() override {
        cout << "Connected to MySQL" << "\n";
    }
    void execute(const string& query) override {
        cout << "MySQL executing: " << query << "\n";
    }
};

class PostgreSQL : public Database {
public: 
    void connect() override {
        cout << "Connected to PostgreSQL" << "\n";
    }
    void execute(const string& query) override {
        cout << "PostgreSQL executing: " << query << "\n";
    }
};

class MongoDB : public Database {
public: 
    void connect() override {
        cout << "Connected to MongoDB" << "\n";
    }
    void execute(const string& query) override {
        cout << "MongoDB executing: " << query << "\n";
    }
};

class DatabaseFactory {
public: 
    static unique_ptr<Database> createDatabase(const string& type) {
        if(type == "MySQL") {
            return make_unique<MySQL>();
        } else if(type == "PostgreSQL") {
            return make_unique<PostgreSQL>();
        } else if(type == "MongoDB") {
            return make_unique<MongoDB>();
        } else {
            return nullptr;
        }
    }
};

void testFactory() {
    cout << "Testing Factory Pattern" << "\n";

    auto mysql = DatabaseFactory::createDatabase("MySQL");
    mysql->connect();
    mysql->execute("Select * from users");

    auto postgres = DatabaseFactory::createDatabase("PostgreSQL");
    postgres->connect();
    postgres->execute("Select * from pUsers");

    cout << "\n\n";
}

int main() {

    testSingleton();
    testFactory();

    return 0;
}
// compile: 
// >> g++ -std=c++17 -pthread basic.cpp -o output
// >> ./output