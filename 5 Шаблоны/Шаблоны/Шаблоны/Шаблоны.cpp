#include <iostream>
#include <string>
#include <vector>

class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
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
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        entities.push_back(entity);
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
        if (!items.empty()) {
            items.erase(items.begin());
        }
    }

    void display() const {
        std::cout << "Queue contents:\n";
        for (int i = 0; i < items.size(); i++)
        {
            std::cout << items[i] << std::endl;
        }
    }
    ~Queue() {
        std::cout << "Queue ended!\n";
    }
};

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 0));
    manager.addEntity(new Enemy("Goblin", 50, "Goblin"));
    manager.displayAll();

    Queue<std::string> stringQueue;
    stringQueue.push("Sword");
    stringQueue.push("Potion");
    stringQueue.display();
    stringQueue.pop();
    stringQueue.display();

    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.display();
    intQueue.pop();
    intQueue.display();
    return 0;
}