#include <iostream>
#include <string>
#include "Header.h"

int main() {
    // Создаем объекты игрока и врага
    Player hero("Hero", 100, 0);
    Enemy monster("Goblin", 50, "Goblin");
    Boss finalBoss("Medusa", 50, "Godness", "Freeze");

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();
    finalBoss.displayInfo();

    return 0;
}