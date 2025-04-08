// 1.3 Полиморфизм.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Header.h"

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Jeb", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 150, 25, 20, "fire");

    Entity* entities[] = { &hero, &goblin, &dragon };

    for (auto& entity : entities) {
        entity->displayInfo();
    }

    hero.attacking(goblin);
    goblin.attacking(hero);
    dragon.attacking(hero);
    hero.Heal(10);
    dragon.Heal(10);

    return 0;
}