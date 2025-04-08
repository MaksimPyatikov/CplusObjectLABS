#include <iostream>
#include "Header.h"
#include <string>

Character::Character(std::string n, int h, int a, int d)
{
	name = n;
	health = h;
	attack = a;
	defense = d;
	maxhealth = h;
}

int Character::GetHealth() {
	return health;
}

void Character::displayInfo()
{
	std::cout <<
		"Name: " << name <<
		", HP: " << health <<
		", Attack: " << attack <<
		", Defense: " << defense << std::endl;
}

void Character::attackEnemy(Character& enemy)
{
	int damage = attack - enemy.defense;
	if (damage > 0) {
		enemy.TakeDamage(damage);
		std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
	}
	else {
		std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
	}
}

void Character::TakeDamage(int amount)
{
	std::cout << name << " took " << amount << " of damage!" << std::endl;
	if (health < amount)
	{
		health = 0;
	}
	else
	{
		health -= amount - defense;
	}
}

void Character::Heal(int amount)
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

Character::~Character() {

}