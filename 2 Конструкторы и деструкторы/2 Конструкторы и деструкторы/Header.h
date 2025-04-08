#pragma once
#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d);
    ~Character();
    void displayInfo() const;
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d);
    ~Monster();
    void displayInfo() const;
};

class Weapon {
private:
    std::string name;
    int durability;
    int weight;
public:
    Weapon(const std::string& n, int h, int a);
    ~Weapon();
    void displayInfo()const;
};