#pragma once

class Character
{
public:
	Character(std::string n, int h, int a, int d);
	int GetHealth();
	void displayInfo();
	void attackEnemy(Character& enemy);
	void TakeDamage(int amount);
	void Heal(int amount);
	~Character();

private:
	std::string name;
	int health;
	int attack;
	int defense;
	int maxhealth;
};
