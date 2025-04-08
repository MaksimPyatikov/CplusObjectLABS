#include "Header.h"

Weapon::Weapon(const std::string& n, int d, int a)
    : name(n), durability(d), weight(a) {
    std::cout << "Weapon " << name << " created!\n";
}

Weapon::~Weapon() {
    std::cout << "Weapon " << name << " destroyed!\n";
}

void Weapon::displayInfo() const {
    std::cout << "Name: " << name << ", Durability: " << durability
        << ", Weight: " << weight << std::endl;
}