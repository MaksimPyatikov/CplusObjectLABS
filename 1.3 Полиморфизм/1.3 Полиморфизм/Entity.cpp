#include "Header.h"

Entity::Entity(const std::string & n, int h, int a, int d)
    : name(n), health(h), attack(a), defense(d), maxhealth(h) {}

void Entity::attacking(Entity& target) {
    int damage = attack - target.defense;
    if (damage > 0) {
        target.health -= damage;
        std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
    }
    else {
        std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
    }
}

void Entity::displayInfo() const {
    std::cout << "Name: " << name << ", HP: " << health
        << ", Attack: " << attack << ", Defense: " << defense << std::endl;
}

void Entity::Heal(int amount)
{
    std::cout << name << " healed for " << amount << " health!" << std::endl;
    if (health + amount > maxhealth)
    {
        health = maxhealth;
    }
    else
    {
        health += amount;
    }
}

Entity::~Entity() {}

std::string Entity::getName() const 
{ 
    return name; 
}
int Entity::getHealth() const 
{ 
    return health;
}
int Entity::getDefense() const 
{ 
    return defense; 
}
void Entity::reduceHealth(int amount)
{
    health -= amount;
}