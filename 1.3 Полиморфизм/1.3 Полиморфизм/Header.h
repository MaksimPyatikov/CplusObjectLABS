#pragma once
#include <iostream>
#include <string>

class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
    int maxhealth;
public:
    Entity(const std::string& n, int h, int a, int d);

    virtual void attacking(Entity& target);
    virtual void displayInfo() const;
    virtual void Heal(int amount);
    virtual ~Entity();

    std::string getName() const;
    int getHealth() const;
    int getDefense() const;
    void reduceHealth(int amount);
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d);
    void attacking(Entity& target) override;
    void displayInfo() const override;
    void Heal(int amount) override;
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d);
    void attacking(Entity& target) override;
    void displayInfo() const override;
};

class Boss : public Monster {
private:
    std::string specialAbility;
public:
    Boss(const std::string& n, int h, int a, int d, const std::string& ability);
    void attacking(Entity& target) override;
    void displayInfo() const override;
};