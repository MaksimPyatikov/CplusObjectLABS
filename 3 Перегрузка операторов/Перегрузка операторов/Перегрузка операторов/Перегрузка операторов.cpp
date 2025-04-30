#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
public:
    Weapon(const std::string& n, int d)
    {
        name = n;
        damage = d;
        std::cout << "Weapon " << name << " was created with: " << damage << " damage! \n";
    }

    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    std::string GetName() const
    {
        return name;
    }

    int GetDamage() const
    {
        return damage;
    }

    Weapon operator+ (const Weapon& other) const
    {
        std::string newName = name + "-" + other.GetName();
        int newDamage = damage + other.GetDamage();
        return Weapon(newName, newDamage);
    }

    bool operator>(const Weapon& other) const {
        return damage > other.GetDamage();
    }

    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon with name: " << weapon.GetName() << " and " << weapon.GetDamage() << " :damage \n";
        return os;
    }
};

int main() {
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    std::cout << hero1 << std::endl; // Вывод информации о персонаже

    Weapon greatsword("Great Sword", 65);
    Weapon bow("Bow", 50);
    Weapon axe("Axe", 30);

    Weapon bowaxe = bow + axe;
    if (bowaxe > greatsword) {
        std::cout << bowaxe.GetName() << " attack is higher than " << greatsword.GetName() << "\n";
    }
    else {
        std::cout << greatsword.GetName() << " attack is higher than " << bowaxe.GetName() << "\n";
    }
    if (axe > greatsword) {
        std::cout << axe.GetName() << " attack is higher than " << greatsword.GetName() << "\n";
    }
    else {
        std::cout << greatsword.GetName() << " attack is higher than " << axe.GetName() << "\n";
    }

    return 0;
}