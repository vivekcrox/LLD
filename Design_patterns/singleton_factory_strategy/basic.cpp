#include<iostream>
#include<mutex>
#include <memory>
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

// ===========================================================================

// Problem 3: Strategy - Payment Processing
/**
 * Implement payment strategies: CreditCard, PayPal, Cryptocurrency
 * Context should be able to switch payment method at runtime
 */

class PaymentStrategy {
public: 
    virtual ~PaymentStrategy() = default;
    virtual void pay(double amount) = 0;
};

class CreditCardPayment : public PaymentStrategy {
private: 
    string cardNumber;
public: 
    CreditCardPayment(const string& card) : cardNumber(card) {}

    void pay(double amount) override {
        cout << "Paid $" << amount << " using Credit Card: " <<
        cardNumber.substr(cardNumber.length() - 4) << "\n";
    }
};

class PayPalPayment : public PaymentStrategy {
private: 
    string email;
public:    
    PayPalPayment(const string& email) : email(email) {}

    void pay(double amount) override {
        cout << "Paid $" << amount << " using PayPal: " << email << "\n";
    }
};

class CryptoPayment : public PaymentStrategy {
private: 
    string walletAddress;
public: 
    CryptoPayment(const string& wallet) : walletAddress(wallet) {}

    void pay(double amount) override {
        cout << "Paid $" << amount << " using Crypto wallet: " << walletAddress.substr(0, 8) << "..." << "\n";
    }
};

class PaymentContext {
private: 
    PaymentStrategy* strategy;
public: 
    PaymentContext() {
        strategy = nullptr;
    }

    void setStrategy(PaymentStrategy* newStrategy) {
        strategy = newStrategy;
    }

    void executePayment(double amount) {
        if(strategy) {
            strategy->pay(amount);
        } else {
            cout << "No Payment strategy set !" << "\n";
        }
    }
};

void testStrategy() {
    cout << "Testing Strategy Pattern" << "\n";

    PaymentContext context;

    context.setStrategy(new CreditCardPayment("2322-1232-2231-0292"));
    context.executePayment(100.2);

    context.setStrategy(new CryptoPayment("0x29297562202672020220010"));
    context.executePayment(2921.63);

    cout << "\n\n";
}

int main() {

    testSingleton();
    testFactory();
    testStrategy();

    return 0;
}
// compile: 
// >> g++ -std=c++17 -pthread basic.cpp -o output
// >> ./output