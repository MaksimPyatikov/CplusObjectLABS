#include <iostream>
#include <string>
#include <memory>

class Inventory {
    std::unique_ptr<std::string[]> inventory;
    int size = 0;
public:
    Inventory() {
        std::cout << "Inventory was created";
    }
    ~Inventory() {
        std::cout << "Inventory was destroyed";
    }
    void addItem(const std::string& item) {
        std::unique_ptr<std::string[]> newInventory(new std::string[size + 1]);
        for (int i = 0; i < size; ++i) {
            newInventory[i] = inventory[i];
        }
        newInventory[size] = item;
        inventory = std::move(newInventory);
        ++size;
    }
    void displayInventory() {
        if (size == 0) {
            std::cout << "Inventory is empty!";
        }
        else {
            std::cout << "Inventory contains: ";
            for (int i = 0; i < size; i++)
            {
                std::cout << inventory[i] << ", ";
            }
            std::cout << "that's " << size << " items \n";
        }
    }
};

int main() {
    Inventory inv;
    inv.addItem("Greatsword");
    inv.addItem("Bow");
    inv.addItem("Axe");

    std::cout << "\n";
    inv.displayInventory();

    return 0;
}