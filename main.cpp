#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>

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

// Operating on files
bool LoadFromFile(RecordNode* &db);
void SaveToFile(RecordNode* &db);

// Displaying, adding, removing and modifying records
void DisplayDatabase(RecordNode*);
void DisplayRecord(Record);
void AddToDatabase(RecordNode* &db, Record);
void EditRecord(RecordNode*, int);
void RemoveRecord(RecordNode* &db, int);
void FindRecord(RecordNode*, int);
void FindRecord(RecordNode*, std::string);
void FindRecord(RecordNode*, float);
void SortDatabase(RecordNode* &db, int);
Record CreateNewRecord();

// Option menus
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
    RecordNode* db = NULL;
    dbsize = 0;
    if (!LoadFromFile(db)) std::cout << "Unable to load a database file. Creating new." << std::endl;
    int choice = Menu();
    while (choice != 8) {
        switch (choice) {
            case 1: {
                DisplayDatabase(db);
                break;
            }
            case 2: {
                int search_choice = SearchMenu();
                std::cin.ignore();
                switch (search_choice) {
                    case 1: {
                        int id;
                        std::cout << "ID: " << std::endl;
                        std::cin >> id;
                        FindRecord(db, id);
                        break;
                    }
                    case 2: {
                        std::string name;
                        std::cout << "Name: " << std::endl;
                        std::cin >> name;
                        FindRecord(db, name);
                        break;
                    }
                    case 3: {
                        float price;
                        std::cout << "Price: " << std::endl;
                        std::cin >> price;
                        FindRecord(db, price);
                        break;
                    }
                }
                break;
            }
            case 3: {
                AddToDatabase(db, CreateNewRecord());
                break;
            }
            case 4: {
                int id;
                std::cout << "Enter id of the record you wish to modify: ";
                std::cin >> id;
                EditRecord(db, id);
                break;
            }
            case 5: {
                int id;
                std::cout << "Enter id of the record you wish to remove: ";
                std::cin >> id;
                RemoveRecord(db, id);
                break;
            }
            case 6: {
                int sort_choice = SortMenu();
                SortDatabase(db, sort_choice);
                break;
            }
            case 7: {
                SaveToFile(db);
                break;
            }
        }
        choice = Menu();
    }
    while (db != NULL) {
        RecordNode* tmp = db->nextNode;
        delete db;
        db = tmp;
    }
    return 0;
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

void SaveToFile(RecordNode* &db) {
    std::ifstream file("DB.dbf");
    std::string error;
    if (file.good()) {
        rename("DB.dbf", "DB.transact.dbf");
        file.close();
    }
    try {
        std::ofstream output("DB.dbf", std::ios::trunc);
        output << currentid << std::endl << dbsize << std::endl;
        RecordNode* tmp = db;
        if (tmp != NULL) {
            while (tmp != NULL) {
                output << tmp->record.id << " " << tmp->record.name << " " << tmp->record.price << std::endl;
                tmp = tmp->nextNode;
            }
        }
        output.close();
    }
    catch (std::string &error) {
        std::cout << error << std::endl;
        remove("DB.dbf");
        rename("DB.transact.dbf", "DB.dbf");
    }
    remove("DB.transact.dbf");
}

void DisplayDatabase(RecordNode* db) {
    int i = 0;
    RecordNode* tmp = db;
    if (tmp != NULL) {
        while (tmp != NULL) {
            std::cout << ">> Element number " << i << std::endl;
            DisplayRecord(tmp->record);
            i++;
            tmp = tmp->nextNode;
        }
    }
    else {
        std::cout << "Record not found." << std::endl;
    }
}

void DisplayRecord(Record record) {
    std::cout << "> ID: " << record.id << std::endl;
    std::cout << "> Name: " << record.name << std::endl;
    std::cout << "> Price: " << record.price << std::endl;
}

void AddToDatabase(RecordNode* &db, Record record) {
    RecordNode* tmp = db;
    RecordNode* newRecord = new RecordNode;
    newRecord->record = record;
    newRecord->nextNode = NULL;
    if (tmp == NULL) db = newRecord;
    else {
        while (tmp->nextNode != NULL) tmp = tmp->nextNode;
        tmp->nextNode = newRecord;
    }
    dbsize++;
}

void EditRecord(RecordNode* db, int id) {
    RecordNode* tmp = db;
    while (tmp != NULL) {
        if (tmp->record.id == id) {
            int tmpid = id;
            tmp->record = CreateNewRecord();
            tmp->record.id = tmpid;
            return;
        }
        tmp = tmp->nextNode;
    }
    std::cout << "Record not found." << std::endl;
}

void RemoveRecord(RecordNode* &db, int id) {
    RecordNode* tmp = db;
    if (db == NULL) return;
    if (db->record.id == id) {
        tmp = NULL;
        if (db->nextNode != NULL) {
            tmp = db->nextNode;
        }
        delete db;
        db = tmp;
        dbsize--;
        return;
    }
    if (db != NULL) {
        while (tmp->nextNode->record.id != id) {
            if (tmp->nextNode == NULL) return;
            tmp = tmp->nextNode;
        }
        RecordNode* temp = tmp->nextNode;
        tmp->nextNode = temp->nextNode;
        delete temp;
        dbsize--;
    }
}

void FindRecord(RecordNode* db, int id) {
    RecordNode* tmp = db;
    if (tmp != NULL) {
        while (tmp != NULL) {
            if (tmp->record.id == id) {
                DisplayRecord(tmp->record);
                return;
            }
            tmp = tmp->nextNode;
        }
    }
    else {
        std::cout << "Record not found." << std::endl;
    }
}

void FindRecord(RecordNode* db, std::string name) {
    RecordNode* tmp = db;
    if (tmp != NULL) {
        while (tmp != NULL) {
            if (tmp->record.name == name) {
                DisplayRecord(tmp->record);
                return;
            }
            tmp = tmp->nextNode;
        }
    }
    else {
        std::cout << "Record not found." << std::endl;
    }
}

void FindRecord(RecordNode* db, float price) {
    RecordNode* tmp = db;
    if (tmp != NULL) {
        while (tmp != NULL) {
            if (tmp->record.price == price) {
                DisplayRecord(tmp->record);
                return;
            }
            tmp = tmp->nextNode;
        }
    }
    else {
        std::cout << "Record not found." << std::endl;
    }
}

void SortDatabase(RecordNode* &db, int mode) {
	if (db == NULL) {
		std::cout << "Unable to sort empty database" << std::endl;
		return;
	}
	Record* table = new Record[dbsize];
	RecordNode* tmp = db;
	RecordNode* tmp2 = db;
	int iterator = 0;
	while (tmp != NULL) {
		table[iterator] = tmp->record;
		tmp2 = tmp->nextNode;
		delete tmp;
		tmp = tmp2;
		iterator++;
	}
	switch (mode) {
	case 1: {
			bool p = true;
			for (loong i = dbsize - 1; i > 0; i--) {
				for (loong j = 0; j < i; j++) {
					if (table[j].id > table[j + 1].id) std::swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
            break;
		}
	case 2: {
			bool p = true;
			for (loong i = dbsize - 1; i > 0; i--) {
				for (loong j = 0; j < i; j++) {
					if (table[j].name > table[j + 1].name) std::swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
            break;
		}
	case 3: {
			bool p = true;
			for (loong i = dbsize - 1; i > 0; i--) {
				for (loong j = 0; j < i; j++) {
					if (table[j].price > table[j + 1].price) std::swap(table[j], table[j + 1]);
					p = false;
				}
				if (p) break;
			}
            break;
		}
	}
    db = NULL;
	loong temp = dbsize;
	dbsize = 0;
	for (int i = 0; i < temp; i++) AddToDatabase(db, table[i]);
	std::cout << "Database sorted." << std::endl;
	return;
}

Record CreateNewRecord() {
    Record newRecord;
    std::cout << "Please provide information: " << std::endl;
    std::cout << "> Name: " << std::endl;
    std::cin >> newRecord.name;
    newRecord.price = RequestValueFromUser<float>("> Price:", "Enter numeric value!");
    std::cout << "Assigned id: " << currentid << std::endl;
    newRecord.id = currentid;
    currentid++;
    return newRecord;
}

int Menu() {
    std::cout << "MENU\n"
                 "1. Display all records in the database\n"
                 "2. Search for a record\n"
                 "3. Add a new record\n"
                 "4. Edit existing record\n"
                 "5. Remove record\n"
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
    return RequestValueFromUser<int>("\nSelect an option: ", "Wrong option! Select a value from 1 to 4!");
}

int SortMenu() {
    std::cout << "SORT MENU\n"
                 "1. Sort by id\n"
                 "2. Sort by name\n"
                 "3. Sort by price" << std::endl;
    return RequestValueFromUser<int>("Select an option: ", "Enter numeric value ", 1, 3);
}
