#include<iostream>
#include<vector>
#include<string>
using namespace std;

// Design Pattern used: Decorator, Observer, Strategy & Singleton


// 1. Notification & Decorator
class INotification {
public: 
    virtual string getContent() const = 0;
    virtual ~INotification() {}
};

// Concrete Notification
class SimpleNotification : public INotification {
private: 
    string text;
public: 
    SimpleNotification(const string& txt) : text(txt) {}

    string getContent() const override {
        return text;
    }
};

// Abstract Decorator : wraps a Notification object
class INotificationDecorator : public INotification {
protected: 
    INotification* notification;
public: 
    INotificationDecorator(INotification* n) : notification(n) {}

    virtual ~INotificationDecorator() {
        delete notification;
    }
};

// Decorator to add a timestamp to the content.
class TimestampDecorator : public INotificationDecorator {
public: 
    TimestampDecorator(INotification* n) : INotificationDecorator(n) {}

    string getContent() const override {
        return "[2025-01-10 10:00:00] " + notification->getContent();
    }
};

// Decorator to append a signature to the content.
class SignatureDecorator : public INotificationDecorator {
private: 
    string signature;
public: 
    SignatureDecorator(INotification* n, const string& sign) : INotificationDecorator(n), signature(sign) {}

    string getContent() const override {
        return notification->getContent() + "\n-- " + signature + "\n\n";
    }
};

// 2. Observer Pattern
// Observer interface: each observer gets an update with a Notification pointer.
class IObserver {
public: 
    virtual void update() = 0;
    virtual ~IObserver() {}
};

class IObservable {
public: 
    virtual void addObserver(IObserver* observer) = 0;
    virtual void removeObserver(IObserver* observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~IObservable() {}
};

// Concrete Observerable
class NotificationObservable : public IObservable {
private: 
    INotification* currentNotification;
    vector<IObserver*> observers;
public: 
    NotificationObservable() {
        currentNotification = nullptr;
    }

    void addObserver(IObserver* ob) override {
        observers.push_back(ob);
    }
    void removeObserver(IObserver* ob) override {
        observers.erase(remove(observers.begin(), observers.end(), ob), observers.end());
    }
    void notifyObservers() override {
        for(unsigned int i = 0; i<observers.size(); i++) {
            observers[i]->update();
        }
    }
    void setNotification(INotification* notification) {
        if(currentNotification != nullptr) {
            delete currentNotification;
        }
        currentNotification = notification;
        notifyObservers();
    }
    INotification* getNotification() {
        return currentNotification;
    }
    string getNotificationContent() {
        return currentNotification->getContent();
    }

    ~NotificationObservable() {
        if(currentNotification != nullptr) {
            delete currentNotification;
        }
    }
};

// Concrete Observer 1
class Logger : public IObserver {
private: 
    NotificationObservable* notificationObservable;
public: 
    Logger(NotificationObservable* nobs) : notificationObservable(nobs) {}

    void update() override {
        cout << "Logging new Notification: \n" << notificationObservable->getNotificationContent() << "\n";
    }
};

// Strategy Pattern - Concrete Observer 2
class INotificationStrategy {
public: 
    virtual void sendNotification(string msg) = 0;
    virtual ~INotificationStrategy() {};
};

class EmailStrategy : public INotificationStrategy {
private: 
    string email;
public:    
    EmailStrategy(string email) : email(email) {}

    void sendNotification(string msg) override {
        cout << "Notification send via email: " << email << "\n";
        cout << msg << "\n";
    }
};

class SMSStrategy : public INotificationStrategy {
private: 
    string contactNo; 
public: 
    SMSStrategy(string contactNo) : contactNo(contactNo) {}

    void sendNotification(string msg) override {
        cout << "Notification send via SMS: " << contactNo << "\n";
        cout << msg << "\n";
    }
};

class NotificationEngine : public IObserver {
private: 
    NotificationObservable* notificationObservable;
    vector<INotificationStrategy*> notifStrategies;
public:
    NotificationEngine(NotificationObservable* nobs) : notificationObservable(nobs) {}

    void addNotificationStrategy(INotificationStrategy* strategy) {
        notifStrategies.push_back(strategy);
    }

    void update() override {
        string notifContent = notificationObservable->getNotificationContent();
        for(INotificationStrategy* curStrategy : notifStrategies) {
            curStrategy->sendNotification(notifContent);
        }
    }
};

// 3. Notification Engine 
// Any client will interact with this service.
// Singleton Pattern
class NotificationService {
private: 
    NotificationObservable* observable;
    vector<INotification*> notifications;
    static NotificationService* instance;

    NotificationService() { // private ctor
        observable = new NotificationObservable();
    } 

public: 
    static NotificationService* getInstance() {
        if(instance == nullptr) {
            instance = new NotificationService;
        }
        return instance;
    }

    // expose observable so that observers can attach
    NotificationObservable* getObservable() {
        return observable;
    }

    // Create new notification and notity observers
    void sendNotification(INotification* notification) {
        notifications.push_back(notification);
        observable->setNotification(notification);
    }

    ~NotificationService() {
        delete observable;
    }
};
NotificationService* NotificationService::instance = nullptr;

int main() {

    // Create Notification Service
    NotificationService* notificationService = NotificationService::getInstance();

    NotificationObservable* notificationObservable = notificationService->getObservable();

    Logger* logger = new Logger(notificationObservable);

    NotificationEngine* notificationEngine = new NotificationEngine(notificationObservable);
    notificationEngine->addNotificationStrategy(new EmailStrategy("vivek.singh1@gmail.com"));
    notificationEngine->addNotificationStrategy(new SMSStrategy("+91-9200202000"));

    // Attach these observers
    notificationObservable->addObserver(logger);
    notificationObservable->addObserver(notificationEngine);

    // Create a notification with decorators.
    INotification* notification = new SimpleNotification("Congratulations!! You are selected for this opportunity");
    notification = new TimestampDecorator(notification);
    notification = new SignatureDecorator(notification, "Google :)");

    // Send Notification
    notificationService->sendNotification(notification);

    
    delete logger;
    delete notificationEngine;

    return 0;
}