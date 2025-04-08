#include "Header.h"

Monster::Monster(const std::string& n, int h, int a, int d)
    : name(n), health(h), attack(a), defense(d) {
    std::cout << "Monster " << name << " created!\n";
}

Monster::~Monster() {
    std::cout << "Monster " << name << " destroyed!\n";
}

void Monster::displayInfo() const {
    std::cout << "Name: " << name << ", HP: " << health
        << ", Attack: " << attack << ", Defense: " << defense << std::endl;
}