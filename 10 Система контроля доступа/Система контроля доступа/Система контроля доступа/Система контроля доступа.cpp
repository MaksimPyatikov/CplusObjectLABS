#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <algorithm>  
#include <random>

class User {
private:
    std::string name;
    int id; 
    int accesslevel;
public:
    User(const std::string& n, int i, int al) { 
        if (validate(n, i, al)) {
            name = n;
            id = i;
            accesslevel = al;
        }
    }

    void setName(std::string newName) {
        if (!newName.empty()) name = newName;
        else std::cerr << "Error: Name can't be empty\n";
    }
    void setId(int newId) { 
        if (newId >= 1) id = newId; 
        else std::cerr << "Error: ID must be positive!\n";
    }
    void setAccessLevel(int newAccessLevel) {
        if (newAccessLevel >= 1 && newAccessLevel <= 3) accesslevel = newAccessLevel;
        else std::cerr << "Error: Access level must be between 1 and 3!\n";
    }
    virtual bool validate(std::string newName, int newId, int newAccessLevel) {
        bool validName = !newName.empty();
        bool validId = (newId >= 1); 
        bool validAccessLevel = (newAccessLevel >= 1 && newAccessLevel <= 3);
        return validName && validId && validAccessLevel;
    }

    std::string getName() const { return name; }
    int getId() const { return id; } 
    int getAccessLevel() const { return accesslevel; }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", ID: " << id << ", Access level: " << accesslevel << std::endl;
    }
    virtual std::string serialize() const {
        std::stringstream ss;
        ss << "User," << name << "," << id << "," << accesslevel;
        return ss.str();
    }

    virtual ~User() = default;
};

class Student : public User {
private:
    std::string group;
public:
    Student(const std::string& n, const std::string& g, int i) : User(n, i, 1) { 
        if (!g.empty()) group = g;
        else std::cerr << "Error: Group can't be empty\n";
    }

    void setGroup(std::string newGroup) {
        if (!newGroup.empty()) group = newGroup;
        else std::cerr << "Error: Group can't be empty\n";
    }
    std::string getGroup() const { return group; }

    void displayInfo() const override {
        std::cout << "Name: " << getName()
            << ", Group: " << group
            << ", ID: " << getId() 
            << ", Access level: " << getAccessLevel() << std::endl;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << "Student," << getName() << "," << getId() << "," << getAccessLevel() << "," << group;
        return ss.str();
    }
};

class Teacher : public User {
private:
    std::string department;
public:
    Teacher(const std::string& n, int i, const std::string& d) : User(n, i, 2) { 
        if (!d.empty()) department = d;
        else std::cerr << "Error: Department can't be empty\n";
    }

    void setDepartment(std::string newDepartment) {
        if (!newDepartment.empty()) department = newDepartment;
        else std::cerr << "Error: Department can't be empty\n";
    }
    std::string getGroup() const { return department; } 

    void displayInfo() const override {
        std::cout << "Teacher: " << getName()
            << ", Department: " << department
            << ", ID: " << getId()
            << ", Access Level: " << getAccessLevel() << std::endl;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << "Teacher," << getName() << "," << getId() << "," << getAccessLevel() << "," << department;
        return ss.str();
    }
};

class Administrator : public User {
private:
    std::string role;
public:
    Administrator(const std::string& n, int i, const std::string& r) : User(n, i, 3) { 
        if (!r.empty()) role = r;
        else std::cerr << "Error: Role can't be empty\n";
    }

    void setDepartment(std::string newRole) {
        if (!newRole.empty()) role = newRole;
        else std::cerr << "Error: Role can't be empty\n";
    }
    std::string getGroup() const { return role; } 

    void displayInfo() const override {
        std::cout << "Administrator: " << getName()
            << ", Role: " << role
            << ", ID: " << getId()
            << ", Access Level: " << getAccessLevel() << std::endl;
    }

    std::string serialize() const override {
        std::stringstream ss;
        ss << "Administrator," << getName() << "," << getId() << "," << getAccessLevel() << "," << role;
        return ss.str();
    }
};

class Resource {
private:
    std::string name;
    int requiredAccessLevel;
public:
    Resource(const std::string& n, int r) : name(n), requiredAccessLevel(r) {
        if (r <= 0) throw std::invalid_argument("Required access level must be > 0");
        if (n.empty()) throw std::invalid_argument("Resource name can't be empty");
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void displayInfo() const {
        std::cout << "Resource: " << name
            << ", Required Access Level: " << requiredAccessLevel << std::endl;
    }

    std::string getName() const { return name; }
    int getRequiredLevel() const { return requiredAccessLevel; }

    std::string serialize() const {
        std::stringstream ss;
        ss << name << "," << requiredAccessLevel;
        return ss.str();
    }
};

template <typename UserPtr, typename ResourcePtr>
class AccessControlSystem {
private:
    std::vector<UserPtr> users;
    std::vector<ResourcePtr> resources;
public:
    void addUser(UserPtr user) {
        users.push_back(std::move(user));
    }

    void addResource(ResourcePtr resource) {
        resources.push_back(std::move(resource));
    }

    void listUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void listResources() const {
        for (const auto& resource : resources) {
            resource->displayInfo();
        }
    }

    void checkAccessForAll() const {
        for (const auto& user : users) {
            std::cout << "\nChecking access for: " << user->getName() << std::endl;
            for (const auto& resource : resources) {
                if (resource->checkAccess(*user)) {
                    std::cout << "Have access to " << resource->getName() << std::endl;
                }
                else {
                    std::cout << "Dont have access to " << resource->getName() << std::endl;
                }
            }
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (!outFile) {
            throw std::runtime_error("Unable to open file for writing: " + filename);
        }
        outFile << users.size() << "\n";
        for (const auto& user : users) {
            outFile << user->serialize() << "\n";
        }
        outFile << resources.size() << "\n";
        for (const auto& resource : resources) {
            outFile << resource->serialize() << "\n";
        }
        outFile.close();
        std::cout << "Data saved to " << filename << ".\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            throw std::runtime_error("Unable to open file for reading: " + filename);
        }

        users.clear();
        resources.clear();

        int userCount;
        inFile >> userCount;
        inFile.ignore();
        for (int i = 0; i < userCount; ++i) {
            std::string line;
            if (!std::getline(inFile, line)) {
                throw std::runtime_error("Unexpected end of file while reading users");
            }
            std::stringstream ss(line);
            std::string type, name, specific;
            int id, accessLevel;
            char removechar;

            std::getline(ss, type, ',');
            std::getline(ss, name, ',');
            ss >> id >> removechar >> accessLevel >> removechar;
            std::getline(ss, specific);

            try {
                if (type == "Student") {
                    users.push_back(std::make_unique<Student>(name, specific, id));
                }
                else if (type == "Teacher") {
                    users.push_back(std::make_unique<Teacher>(name, id, specific));
                }
                else if (type == "Administrator") {
                    users.push_back(std::make_unique<Administrator>(name, id, specific));
                }
                else {
                    std::cerr << "Warning: Unknown user type '" << type << "' skipped\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading user: " << e.what() << "\n";
            }
        }

        int resourceCount;
        inFile >> resourceCount;
        inFile.ignore();
        for (int i = 0; i < resourceCount; ++i) {
            std::string line;
            if (!std::getline(inFile, line)) {
                throw std::runtime_error("Unexpected end of file while reading resources");
            }
            std::stringstream ss(line);
            std::string name;
            int requiredLevel;

            std::getline(ss, name, ',');
            ss >> requiredLevel;

            try {
                resources.push_back(std::make_unique<Resource>(name, requiredLevel));
            }
            catch (const std::exception& e) {
                std::cerr << "Error loading resource: " << e.what() << "\n";
            }
        }

        inFile.close();
    }

    void displayAllUsers() const {
        if (not(users.empty())) {
            for (const auto& user : users) {
                user->displayInfo();
            }
            return;
        }
        std::cout << "No users in the system.\n";
        return;
    }
    void displayAllResources() const {
        if (not(resources.empty())) {
            for (const auto& resource : resources) {
                resource->displayInfo();
            }
            return;
        }
        std::cout << "No resources in the system.\n";
        return;
    }

    User* findUserByName(const std::string& name) const {
        for (const auto& user : users) {
            if (user->getName() == name) {
                return user.get();
            }
        }
        throw std::invalid_argument("User " + name + " not found.");
    }

    User* findUserById(int id) const {
        for (const auto& user : users) {
            if (user->getId() == id) {
                return user.get();
            }
        }
        throw std::invalid_argument("User with ID " + std::to_string(id) + " not found.");
    }
    bool checkAccess(const User& user, const std::string& resourceName) const {
        for (const auto& resource : resources) {
            if (resource->getName() == resourceName) {
                bool access = resource->checkAccess(user);
                std::cout << user.getName() << (access ? " has access to " : " does not have access to ")
                    << resourceName << ".\n";
                return access;
            }
        }
        throw std::invalid_argument("Resource " + resourceName + " not found.");
    }
    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
        std::cout << "Users sorted by access level.\n";
    }
};

//------------------------------------------------------------------------------------------
std::vector<std::string> firstNames = {
        "Alex", "Maria", "Sergei", "Olga", "Dmitri", "Anna", "Victor", "Yulia", "Pavel", "Irina",
        "Nikita", "Tatiana", "Andrei", "Ekaterina", "Roman"
};
std::vector<std::string> lastNames = {
    "Smirnov", "Ivanova", "Kuznetsov", "Popov", "Sokolov", "Lebedeva", "Morozov", "Petrov",
    "Volkov", "Romanova", "Orlov", "Pavlov", "Zaitsev", "Egorov", "Voronin"
};
std::vector<std::string> groups = { "IT405", "CS301", "SE202" };
std::vector<std::string> departments = { "Cybersecurity", "Artificial Intelligence", "Software Engineering", "Computer science" };
std::vector<std::string> roles = { "Security Officer", "Network Manager", "System Admin" };
void addRandomUsers(AccessControlSystem<std::unique_ptr<User>, std::unique_ptr<Resource>>& accessSystem, int count, int accesslevel) {
    if (accesslevel < 1 or accesslevel > 3) {
        std::cerr << "Error: Access level must be between 1 and 3!\n";
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> firstNameDist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> lastNameDist(0, lastNames.size() - 1);
    std::uniform_int_distribution<> groupDist(0, groups.size() - 1);
    std::uniform_int_distribution<> deptDist(0, departments.size() - 1);
    std::uniform_int_distribution<> roleAttrDist(0, roles.size() - 1);

    static int nextId = 5001;

    for (int i = 0; i < count; ++i) {
        std::string name = firstNames[firstNameDist(gen)] + " " + lastNames[lastNameDist(gen)];
        int id = nextId++;
        try {
            if (accesslevel == 1) { 
                std::string group = groups[groupDist(gen)];
                accessSystem.addUser(std::make_unique<Student>(name, group, id));
            }
            else if (accesslevel == 2) { 
                std::string department = departments[deptDist(gen)];
                accessSystem.addUser(std::make_unique<Teacher>(name, id, department));
            }
            else {
                std::string roleAttr = roles[roleAttrDist(gen)];
                accessSystem.addUser(std::make_unique<Administrator>(name, id, roleAttr));
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error adding random user: " << e.what() << "\n";
        }
    }

    std::cout << "Added " << count << " random users.\n";
}

int main() {
    AccessControlSystem<std::unique_ptr<User>, std::unique_ptr<Resource>> system;
    try {
        system.addUser(std::make_unique<Administrator>("Pyatikov Maksim", 4001, "Administator"));

        addRandomUsers(system, 10, 1);
        system.addUser(std::make_unique<Student>("Andrei Volodin", "IT405", 2001));
        system.addUser(std::make_unique<Teacher>("Larz Herbert", 3001, "Cybersecurity"));


        addRandomUsers(system, 5, 2);
        system.addUser(std::make_unique<Teacher>("Natalia Zaitseva", 3002, "Artificial Intelligence"));

        system.addResource(std::make_unique<Resource>("Seminar Room", 1));
        system.addResource(std::make_unique<Resource>("Data Center", 3));
        system.addResource(std::make_unique<Resource>("Resource Library", 2));

        std::cout << "\nAll Users:\n";
        system.displayAllUsers();
        std::cout << "\nAll Resources:\n";
        system.displayAllResources();

        std::cout << "\nChecking Access:\n";
        system.checkAccess(*system.findUserByName("Pyatikov Maksim"), "Data Center");          // Should pass (access level 3)
        system.checkAccess(*system.findUserByName("Andrei Volodin"), "Seminar Room");       // Should pass (access level 1)
        system.checkAccess(*system.findUserByName("Andrei Volodin"), "Data Center");        // Should fail (access level 1)
        system.checkAccess(*system.findUserByName("Larz Herbert"), "Data Center");       // Should fail (access level 2)
        system.checkAccess(*system.findUserByName("Natalia Zaitseva"), "Resource Library"); // Should pass (access level 2)

        system.saveToFile("access_control.txt");

        AccessControlSystem<std::unique_ptr<User>, std::unique_ptr<Resource>> newSystem;
        newSystem.loadFromFile("access_control.txt");
        std::cout << "\nLoaded Users:\n";
        newSystem.displayAllUsers();

        std::cout << "\nSearching Users:\n";
        User* user = newSystem.findUserByName("Natalia Zaitseva");
        user->displayInfo();
        user = newSystem.findUserById(3001);
        user->displayInfo();

        std::cout << "\nSorting Users by Access Level:\n";
        newSystem.sortUsersByAccessLevel();
        newSystem.displayAllUsers();

        std::cout << "\nTesting Invalid Inputs:\n";
        try {
            system.addUser(std::make_unique<Student>("", "IT405", 2015));
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        try {
            system.checkAccess(*system.findUserByName("Natalia Zaitseva"), "Nonexistent");
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}