#include "Header.h"

Boss::Boss(const std::string& n, int h, int a, int d, const std::string& ability)
    : Monster(n, h, a, d), specialAbility(ability) {}

void Boss::attacking(Entity& target) {
    int damage = attack - target.getDefense();
    if (damage > 0) {
        if (rand() % 100 < 30) {
            damage += 5;
            std::cout << "Poisonous attack! ";
        }
        damage += 25;
        target.reduceHealth(damage);
        std::cout << name << " attacks " << target.getName() << " for " << damage << " damage using special ability: " << specialAbility << "\n";
    }
    else {
        std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
    }
}

void Boss::displayInfo() const {
    std::cout << "Monster: " << name << ", HP: " << health
        << ", Attack: " << attack << ", Defense: " << defense << std::endl;
}