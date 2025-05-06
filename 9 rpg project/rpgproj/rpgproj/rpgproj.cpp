#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <random>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <limits>

template <typename T>
class Logger {
private:
    std::string filename;

public:
    Logger(const std::string& fname) : filename(fname) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeBuf[26];
        ctime_s(timeBuf, sizeof(timeBuf), &now);
        file << "Session started: " << timeBuf;
    }

    void log(const T& event) {
        std::ofstream file(filename, std::ios::app);
        if (!file) {
            throw std::runtime_error("Failed to append to log file: " + filename);
        }
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char timeBuf[26];
        ctime_s(timeBuf, sizeof(timeBuf), &now);
        file << "[" << timeBuf << "] " << event << "\n";
    }
};

class Inventory {
private:
    std::vector<std::string> items;

public:
    void addItem(const std::string& item) {
        if (item.empty()) {
            throw std::invalid_argument("Item name cannot be empty.");
        }
        items.push_back(item);
        std::cout << "Stored " << item << " in inventory.\n";
    }

    void removeItem(const std::string& item) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (*it == item) {
                items.erase(it);
                std::cout << "Discarded " << item << " from inventory.\n";
                return;
            }
        }
        throw std::invalid_argument("Item " + item + " not found in inventory.");
    }

    void displayInventory() const {
        if (items.empty()) {
            std::cout << "Inventory is empty.\n";
            return;
        }
        std::cout << "Inventory Contents:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i + 1 << ". " << items[i] << "\n";
        }
    }

    std::string serialize() const {
        std::stringstream ss;
        ss << items.size();
        for (const auto& item : items) {
            ss << "|" << item;
        }
        return ss.str();
    }

    void deserialize(const std::string& data) {
        std::istringstream iss(data);
        size_t count;
        iss >> count;
        items.clear();
        std::string item;
        std::getline(iss, item);
        for (size_t i = 0; i < count; ++i) {
            std::getline(iss, item, '|');
            if (!item.empty()) {
                items.push_back(item);
            }
        }
    }

    std::string getItem(size_t index) const {
        if (index >= items.size()) {
            throw std::out_of_range("Invalid item index.");
        }
        return items[index];
    }

    size_t size() const {
        return items.size();
    }
};

class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Entity(const std::string& n, int h, int a, int d) : name(n), health(h), attack(a), defense(d) {}
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << "\n";
    }

    void takeDamage(int damage, Logger<std::string>& logger) {
        std::stringstream ss;
        int actualDamage = std::max(damage - defense, 1);
        health -= actualDamage;
        ss << name << " suffered " << actualDamage << " damage";
        std::cout << ss.str() << "\n";
        logger.log(ss.str());
    }

    virtual int getHealth() const { return health; }
    virtual int getAttack() const { return attack; }
    virtual std::string getName() const { return name; }
    virtual int getDefense() const { return defense; }
    virtual ~Entity() {}
};

class Character : public Entity {
private:
    int level = 0;
    int nextlevelexp = 100;
    int exp = 0;
    int maxhealth;
    Inventory inventory;

public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d), maxhealth(h) {}

    void displayInfo() const override {
        std::cout << "Name: " << name << ", HP: " << health << ", Level: " << level << ", exp" << exp << "/" << nextlevelexp << "\n";
    }

    void attacking(Entity& enemy, Logger<std::string>& logger) const {
        std::stringstream ss;
        int attackDamage = getAttack();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> critChance(1, 100);
        if (critChance(gen) <= 25) {
            attackDamage = static_cast<int>(attackDamage * 1.8);
            ss << "Critical Hit! " << name << " strikes " << enemy.getName()
                << " for " << attackDamage << " damage!\n";
        }
        else {
            ss << name << " attacks " << enemy.getName()
                << " for " << attackDamage << " damage!\n";
        }
        std::cout << ss.str();
        logger.log(ss.str());
        enemy.takeDamage(attackDamage, logger);
    }

    void gainexp(int amount, Logger<std::string>& logger) {
        exp += amount;
        while (exp >= nextlevelexp) {
            level++;
            health = maxhealth;
            exp -= nextlevelexp;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> statBoost(8, 25);
            int attackBoost = statBoost(gen);
            attack += attackBoost;
            nextlevelexp = static_cast<int>(nextlevelexp * 1.3);

            std::stringstream ss;
            ss << name << " advanced to level " << level
                << ", health restored, gained +" << attackBoost << " attack\n";
            std::cout << ss.str();
            logger.log(ss.str());
        }
    }

    void heal(int amount, Logger<std::string>& logger) {
        health += amount;
        if (health > maxhealth) {
            health = maxhealth;
        }
        std::stringstream ss;
        ss << name << " recovered " << amount << " health\n";
        logger.log(ss.str());
    }

    void addItem(const std::string& item, Logger<std::string>& logger) {
        inventory.addItem(item);
        logger.log(name + " stored " + item + " in inventory.");
    }

    void removeItem(const std::string& item, Logger<std::string>& logger) {
        inventory.removeItem(item);
        logger.log(name + " discarded " + item + " from inventory.");
    }

    void displaylevel() {
        std::cout << name << " is at level: " << level
            << ", EXP: " << exp << "/" << nextlevelexp << "\n\n";
    }

    void setNextLevelExp(int value) { nextlevelexp = value; }

    void increaseAttack(int amount, Logger<std::string>& logger) {
        attack += amount;
        std::stringstream ss;
        ss << name << " attack increased by " << amount << "\n";
        std::cout << ss.str();
        logger.log(ss.str());
    }

    void increaseDefense(int amount, Logger<std::string>& logger) {
        defense += amount;
        std::stringstream ss;
        ss << name << " defense increased by " << amount << "\n";
        std::cout << ss.str();
        logger.log(ss.str());
    }

    void increaseMaxHealth(int amount, Logger<std::string>& logger) {
        maxhealth += amount;
        health += amount;
        if (health > maxhealth) health = maxhealth;
        std::stringstream ss;
        ss << name << " maxhealth increased by " << amount << " and healed\n";
        std::cout << ss.str();
        logger.log(ss.str());
    }

    int getExp() const { return exp; }
    int getMaxhealth() const { return maxhealth; }
    int getLevel() const { return level; }
    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    int getNextLevelExp() const { return nextlevelexp; }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << "\n";
    }

    void attacking(Entity& hero, Logger<std::string>& logger) {
        std::stringstream ss;
        int attackDamage = getAttack();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> critChance(1, 100);
        if (critChance(gen) <= 20) {
            attackDamage = static_cast<int>(attackDamage * 2.2);
            ss << "Devastating Blow! " << name << " hits "
                << hero.getName() << " for " << attackDamage << " damage!\n";
        }
        else {
            ss << name << " attacks " << hero.getName()
                << " for " << attackDamage << " damage!\n";
        }
        std::cout << ss.str();
        logger.log(ss.str());
        hero.takeDamage(attackDamage, logger);
    }
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 200, 5, 10) {}
};

class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 50, 15, 0) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 100, 10, 2) {}
};

class Game {
private:
    Character player;
    std::shared_ptr<Logger<std::string>> logger;
    std::vector<std::unique_ptr<Monster>> monsters;
    bool active;

public:
    Game(Character& p, std::shared_ptr<Logger<std::string>> l)
        : player(p), logger(l), active(true) {}

    void addMonster(std::unique_ptr<Monster> monster) {
        monsters.push_back(std::move(monster));
        logger->log("Encountered monster: " + monsters.back()->getName());
    }

    void combat() {
        if (monsters.empty()) {
            std::cout << "No monsters to fight!\n";
            return;
        }
        auto& monster = *monsters.front();
        std::cout << "\nEngaging " << monster.getName() << "!\n";

        try {
            while (monster.getHealth() > 0 and player.getHealth() > 0) {
                player.attacking(monster, *logger);
                if (monster.getHealth() > 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                    monster.attacking(player, *logger);
                }

                std::cout << player.getName() << " HP: " << player.getHealth()
                    << " | " << monster.getName() << " HP: " << monster.getHealth()
                    << "\n\n";

                std::this_thread::sleep_for(std::chrono::milliseconds(700));
            }
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> expGain(15, 30);
            int xpGained = expGain(gen);
            player.gainexp(xpGained, *logger);
            std::string monsterName = monster.getName();
            logger->log(player.getName() + " defeated " + monsterName);
            std::cout << "Hero " << player.getName() << " just defeated "
                << monsterName << " and gained " << xpGained << " XP!\n";
            player.displaylevel();

            std::uniform_int_distribution<> itemChance(1, 100);
            if (itemChance(gen) <= 50) {
                std::discrete_distribution<> itemDist({ 50, 25, 20, 5 });
                int itemType = itemDist(gen);
                std::string itemName;
                if (itemType == 0) {
                    itemName = "Healing Potion";
                }
                else if (itemType == 1) {
                    itemName = "Attack Potion";
                }
                else if (itemType == 2) {
                    itemName = "Defense Potion";
                }
                else {
                    itemName = "Maxhealth Potion";
                }
                player.addItem(itemName, *logger);
                std::cout << player.getName() << " obtained " << itemName << "!\n";
            }

            monsters.erase(monsters.begin());
            logger->log("Monster " + monsterName + " removed from game. Remaining monsters: " + std::to_string(monsters.size()));

            if (player.getHealth() <= 0) {
                active = false;
                logger->log(player.getName() + " has been defeated.");
                std::cout << player.getName() << " has been defeated in combat.\n";
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Combat interrupted: " << e.what() << "\n";
            logger->log("Combat interrupted due to error: " + std::string(e.what()));
            active = false;
        }
    }

    void showActionMenu() {
        while (active) {
            system("cls");

            std::cout << "===== Action Menu =====\n";
            std::cout << "Player: " << player.getName() << " | Level: " << player.getLevel()
                << " | HP: " << player.getHealth() << "/" << player.getMaxhealth()
                << " | Exp: " << player.getExp() << "/" << player.getNextLevelExp() << "\n";
            std::cout << "Monsters Remaining: " << monsters.size() << "\n";
            std::cout << "----------------------\n";
            std::cout << "1. Fight Monster\n";
            std::cout << "2. Explore\n";
            std::cout << "3. View Inventory\n";
            std::cout << "4. Use Item\n";
            std::cout << "5. Save Game\n";
            std::cout << "6. Load Game\n";
            std::cout << "7. Exit\n";
            std::cout << "----------------------\n";
            std::cout << "Enter choice (1-7): ";

            int choice;
            while (!(std::cin >> choice)) {
                std::cout << "Invalid input! Enter a number (1-7): ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore();

            switch (choice) {
            case 1:
                if (!monsters.empty()) {
                    combat();
                }
                else {
                    std::cout << "No monsters to fight! Try exploring to find one.\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                }
                break;

            case 2: 
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::discrete_distribution<> dist({ 6, 4, 2 });
                int monsterType = dist(gen);
                if (monsterType == 0) {
                    addMonster(std::make_unique<Goblin>());
                    std::cout << "You encountered a Goblin!\n";
                }
                else if (monsterType == 1) {
                    addMonster(std::make_unique<Skeleton>());
                    std::cout << "You encountered a Skeleton!\n";
                }
                else {
                    addMonster(std::make_unique<Dragon>());
                    std::cout << "You encountered a Dragon!\n";
                }
                std::cout << "Press Enter to continue...";
                std::cin.get();
            }
            break;

            case 3:
                player.getInventory().displayInventory();
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;

            case 4:
                if (player.getInventory().size() > 0) {
                    player.getInventory().displayInventory();
                    std::cout << "\nSelect an item to use (enter number, 0 to cancel): ";
                    size_t itemIndex;
                    while (!(std::cin >> itemIndex)) {
                        std::cout << "Invalid input! Enter a number: ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore();
                    if (itemIndex == 0) {
                        std::cout << "Cancelled.\n";
                    }
                    else {
                        try {
                            std::string item = player.getInventory().getItem(itemIndex - 1);
                            if (item == "Healing Potion") {
                                player.heal(50, *logger);
                                std::cout << "Healed for 50 HP!\n";
                            }
                            else if (item == "Attack Potion") {
                                player.increaseAttack(10, *logger);
                                std::cout << "Attack increased by 10!\n";
                            }
                            else if (item == "Defense Potion") {
                                player.increaseDefense(5, *logger);
                                std::cout << "Defense increased by 5!\n";
                            }
                            else if (item == "Maxhealth Potion") {
                                player.increaseMaxHealth(20, *logger);
                                std::cout << "Maxhealth increased by 20!\n";
                            }
                            else {
                                std::cout << "Unknown item: " << item << "\n";
                                logger->log("Attempted to use unknown item: " + item);
                            }
                            player.removeItem(item, *logger);
                        }
                        catch (const std::out_of_range& e) {
                            std::cout << "Invalid item number!\n";
                        }
                    }
                }
                else {
                    std::cout << "Inventory is empty.\n";
                }
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;

            case 5:
                try {
                    saveGame(player);
                    std::cout << "Game saved successfully!\n";
                }
                catch (const std::runtime_error& e) {
                    std::cout << "Error saving game: " << e.what() << "\n";
                }
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;

            case 6:
                try {
                    player = loadGame();
                    std::cout << "Game loaded successfully!\n";
                }
                catch (const std::runtime_error& e) {
                    std::cout << "Error loading game: " << e.what() << "\n";
                }
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;

            case 7:
                active = false;
                std::cout << "Exiting game...\n";
                logger->log("Game session ended.");
                break;

            default:
                std::cout << "Invalid choice! Please select 1-7.\n";
                std::cout << "Press Enter to continue...";
                std::cin.get();
                break;
            }
        }
    }


    void displayMonsters() const {
        if (monsters.empty()) {
            std::cout << "No monsters remaining.\n";
            return;
        }
        std::cout << "Current Monsters:\n";
        for (size_t i = 0; i < monsters.size(); ++i) {
            std::cout << i + 1 << ". " << monsters[i]->getName()
                << " (HP: " << monsters[i]->getHealth() << ")\n";
        }
    }

    void saveGame(const Character& character, const std::string& filename = "savegame.txt") {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open save file");
        }

        file << character.getName() << "\n"
            << character.getHealth() << "\n"
            << character.getMaxhealth() << "\n"
            << character.getAttack() << "\n"
            << character.getDefense() << "\n"
            << character.getLevel() << "\n"
            << character.getExp() << "\n"
            << character.getNextLevelExp() << "\n"
            << character.getInventory().serialize() << "\n";
        file.close();
        logger->log("Game saved to " + filename);
    }

    Character loadGame(const std::string& filename = "savegame.txt") {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open save file");
        }

        std::string name, inventoryData;
        int health, maxHealth, attack, defense, level, experience, nextLevelExp;

        std::getline(file, name);
        file >> health >> maxHealth >> attack >> defense >> level >> experience >> nextLevelExp;
        file.ignore();
        std::getline(file, inventoryData);

        Character character(name, maxHealth, attack, defense);
        character.heal(health - character.getHealth(), *logger);
        character.getInventory().deserialize(inventoryData);
        character.setNextLevelExp(nextLevelExp);
        for (int i = 0; i < level; ++i) {
            character.gainexp(nextLevelExp, *logger);
        }
        character.gainexp(experience, *logger);

        logger->log("Game loaded from " + filename);
        return character;
    }

    void setLogger(std::shared_ptr<Logger<std::string>> newLogger) {
        logger = newLogger;
    }

    bool isActive() const { return active; }

private:
    Inventory& getInventory(Character& c) { return c.getInventory(); }
    const Inventory& getInventory(const Character& c) const { return c.getInventory(); }
};

int main() {
    auto logger = std::make_shared<Logger<std::string>>("game.log");
    Character player("Hero", 100, 10, 5);
    Game game(player, logger);
    game.addMonster(std::make_unique<Goblin>());
    game.showActionMenu();
    return 0;
}