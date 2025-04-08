#include "Header.h"

Character::Character(const std::string& n, int h, int a, int d)
    : Entity(n, h, a, d) {}

// Переопределение метода attack
void Character::attacking(Entity& target) {
    int damage = attack - target.getDefense();
    if (damage > 0) {
        // Шанс на критический удар (20%)
        if (rand() % 100 < 20) {
            damage *= 2;
            std::cout << "Critical hit! ";
        }

        target.reduceHealth(damage);
        std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
    }
    else {
        std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
    }
}

void Character::displayInfo() const {
    std::cout << "Character: " << name << ", HP: " << health
        << ", Attack: " << attack << ", Defense: " << defense << std::endl;
}

void Character::Heal(int amount)
{
    std::cout << "Hero " << name << " healed for " << amount << " health!" << std::endl;
    if (health + amount > maxhealth)
    {
        health = maxhealth;
    }
    else
    {
        health += amount;
    }
}