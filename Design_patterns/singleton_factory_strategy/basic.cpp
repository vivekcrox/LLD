#include<iostream>
#include<mutex>
using namespace std;

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
}

int main() {

    testSingleton();

    return 0;
}
// compile: 
// >> g++ -std=c++17 -pthread basic.cpp -o output
// >> ./output