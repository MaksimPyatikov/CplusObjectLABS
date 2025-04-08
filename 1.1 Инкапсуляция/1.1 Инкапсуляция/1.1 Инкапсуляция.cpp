#include <iostream>
#include "Header.h"

int main()
{
    Character hero("Hero", 100, 60, 10);
    Character monster("Goblin", 50, 15, 5);

    hero.displayInfo();
    monster.displayInfo();

    hero.attackEnemy(monster);
    monster.displayInfo();

    hero.TakeDamage(200);
    hero.displayInfo();

    hero.Heal(50);
    hero.displayInfo();

    hero.Heal(120);
    hero.displayInfo();

    return 0;
}
