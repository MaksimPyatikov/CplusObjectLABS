#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }
    virtual int getHealth() const {
        return health;
    }
    virtual ~Entity() {}
};

class Player : public Entity {
    int defence;
public:
    Player(const std::string& n, int h, int d)
        : Entity(n, h), defence(d) {}

    void displayInfo() const override {
        std::cout << "Player: " << name << ", HP: " << health
            << ", Defence: " << defence << std::endl;
    }
};

class Enemy : public Entity {
    std::string type;
public:
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {}

    void displayInfo() const override {
        std::cout << "Enemy: " << name << ", HP: " << health
            << ", Type: " << type << std::endl;
    }
};

template <typename T>
class GameManager {
private:
    std::vector<std::unique_ptr<T>> entities;

public:
    void addEntity(std::unique_ptr<T> entity) {
        if (entity->getHealth() <= 0) {
            throw std::invalid_argument("Entity has invalid health\n");
        }
        entities.push_back(std::move(entity));
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
};

template <typename T>
class Queue {
private:
    std::vector<T> items;

public:
    Queue() {
        std::cout << "Queue created!\n";
    }

    void push(const T& item) {
        items.push_back(item);
    }

    void pop() {
        if (items.empty()) {
            throw std::out_of_range("Queue is empty, can't remove any items\n");
        }
        items.erase(items.begin());
    }

    bool empty() const {
        return items.empty();
    }

    void display() const {
        std::cout << "Queue contents:\n";
        for (const auto& item : items) {
            std::cout << item << std::endl;
        }
    }

    ~Queue() {
        std::cout << "Queue ended!\n";
    }
};

int main() {
    try {
        GameManager<Entity> gamemanager;
        gamemanager.addEntity(std::make_unique<Player>("Invalid", -100, 0));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "GameManager error: " << e.what() << std::endl;
    }

    std::cout << "\n";

    try {
        Queue<std::string> stringQueue;

        stringQueue.push("Hello!");
        stringQueue.display();
        stringQueue.pop();
        stringQueue.pop();
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Queue error: " << e.what() << std::endl;
    }

    try {
        Queue<std::string> stringQueue;

        stringQueue.push("Hello!");
        stringQueue.display();
        stringQueue.pop();
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Queue error: " << e.what() << std::endl;
    }
    std::cout << "\n";
    try {
        Queue<int> intQueue;

        intQueue.push(1);
        intQueue.display();
        intQueue.pop();
        intQueue.pop();
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Queue error: " << e.what() << std::endl;
    }

    return 0;
}
