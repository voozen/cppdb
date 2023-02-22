#include <iostream>
#include <fstream>

typedef unsigned long long loong;
loong currentid, dbsize;

struct Record {
    loong id;
    std::string name;
    float price;
};

struct RecordNode {
    Record record;
    RecordNode* nextNode;
};

void SaveToFile(RecordNode* &db, Record);
bool LoadFromFile(RecordNode* &db);
void AddToDatabase(RecordNode* &db, Record);
void DisplayDatabase(RecordNode*);
void DisplayRecord(Record);
void FindRecord(RecordNode*);
void SortDatabase(RecordNode* &db, int);
Record CreateNewRecord();
void EditRecord(RecordNode*, int);
void RemoveRecord(RecordNode* &db, int);
int Menu();
int SearchMenu();
int SortMenu();

template <class T> T RequestValueFromUser(std::string message, std::string error) {
    T value;
    for (;;) {
        std::cout << message << std::endl;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << error << std::endl;
            std::cin.clear();
            continue;
        }
        break;
    }
    return value;
}

template <class T> T RequestValueFromUser(std::string message, std::string error, T min, T max) {
    T value;
    for (;;) {
        std::cout << message << std::endl;
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cout << error << std::endl;
            std::cin.clear();
            continue;
        }
        break;
    }
    return value;
}

int main() {
    RecordNode* db = nullptr;
    dbsize = 0;
    int choice = Menu();
    while (choice != 8) {
        switch (choice) {
            case 1:
                DisplayDatabase(db);
                break;
            case 2:
                int search_choice = SearchMenu();
        }
    }
}

bool LoadFromFile(RecordNode* &db) {
    std::ifstream input("DB.dbf");
    if (!input.good()) {
        currentid = 0; dbsize = 0;
        return false;
    }
    else {
        loong tmpsize = 0;
        input >> currentid;
        input >> tmpsize;
        Record record;
        try {
            while (input >> record.id >> record.name >> record.price) {
                AddToDatabase(db, record);
            }
            input.close();
            dbsize = tmpsize;
            return true;
        }
        catch (std::string &error){
            std::cout << error << std::endl;
            input.close();
            return false;
        }
    }
}

int Menu() {
    std::cout << "MENU\n"
                 "1. Display all records in the database\n"
                 "2. Search for a record\n"
                 "3. Add a new record\n"
                 "4. Edit existing record\n"
                 "5. Delete record\n"
                 "6. Sort database\n"
                 "7. Save changes\n"
                 "8. Exit" << std::endl;
    return RequestValueFromUser<int>("\nSelect an option: ", "Wrong option! Select a value from 1 to 8!", 1, 8);
}

int SearchMenu() {
    std::cout << "SEARCH MENU\n"
                 "1. Search by id\n"
                 "2. Search by name\n"
                 "3. Search by price" << std::endl;
    return RequestValueFromUser<int>("\nSelect an option: ", "Wrong option! Select a value fromr 1 to 4!");
}