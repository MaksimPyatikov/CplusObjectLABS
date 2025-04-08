#include <iostream>
#include <string>
#include "Header.h"

int main()
{
    Character hero("Jeb", 150, 30, 20);
    hero.displayInfo();

    Monster goblin("Slime", 20, 15, 5);
    goblin.displayInfo();

    Weapon sword("Sword", 30, 5);
    sword.displayInfo();
    Weapon bow("bow", 10, 1);
    bow.displayInfo();
    Weapon* spear = new Weapon("spear", 50, 10);
    delete spear;

    std::cout << "----End of programm-----\n";
    return 0;
}
