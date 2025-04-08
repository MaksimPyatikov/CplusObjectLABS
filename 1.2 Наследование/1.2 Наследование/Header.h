#pragma once
#include <iostream>
#include <string>

class Entity {
protected:
    std::string name;
    int health;
public:
    Entity(const std::string& n, int h);
    virtual void displayInfo() const;
    ~Entity();
};

class Player : public Entity {
private:
    int experience; // Приватное поле: опыт
public:
    // Конструктор производного класса
    Player(const std::string& n, int h, int exp);
    void displayInfo() const override;
};

class Enemy : public Entity {
private:
    std::string type; // Приватное поле: тип врага
public:
    Enemy(const std::string& n, int h, const std::string& t);
    void displayInfo() const override;
};

class Boss : public Enemy {
private:
    std::string specialAbility;
public:
    Boss(const std::string& n, int h, const std::string& t, const std::string& ability);
    void displayInfo() const override;
};