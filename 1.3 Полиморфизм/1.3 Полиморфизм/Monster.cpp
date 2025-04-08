#include "Header.h"

Monster::Monster(const std::string& n, int h, int a, int d)
    : Entity(n, h, a, d) {}

// ��������������� ������ attack
void Monster::attacking(Entity& target){
    int damage = attack - target.getDefense();
    if (damage > 0) {
        // ���� �� �������� ����� (30%)
        if (rand() % 100 < 30) {
            damage += 5; // �������������� ���� �� ���
            std::cout << "Poisonous attack! ";
        }
        target.reduceHealth(damage);
        std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
    }
    else {
        std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
    }
}

void Monster::displayInfo() const {
    std::cout << "Monster: " << name << ", HP: " << health
        << ", Attack: " << attack << ", Defense: " << defense << std::endl;
}