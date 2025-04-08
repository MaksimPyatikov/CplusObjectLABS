#include "Header.h"

Entity::Entity(const std::string& n, int h) : name(n), health(h) {}

void Entity::displayInfo() const {
    std::cout << "Name: " << name << ", HP: " << health << std::endl;
}

Entity::~Entity() {}

Player::Player(const std::string& n, int h, int exp)
    : Entity(n, h), experience(exp) {}

void Player::displayInfo() const {
    Entity::displayInfo();
    std::cout << "Experience: " << experience << std::endl;
}

Enemy::Enemy(const std::string& n, int h, const std::string& t)
    : Entity(n, h), type(t) {}

void Enemy::displayInfo() const {
    Entity::displayInfo();
    std::cout << "Type: " << type << std::endl;
}

Boss::Boss(const std::string& n, int h, const std::string& t, const std::string& ability)
    : Enemy(n, h, t), specialAbility(ability) {}

void Boss::displayInfo() const {
    Enemy::displayInfo(); 
    std::cout << "Special Ability: " << specialAbility << std::endl;
};