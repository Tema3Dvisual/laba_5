#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdio>

using namespace std;

// Параметры склада
const char ZONES[] = {'A', 'B', 'C', 'D'};
const int NUM_ZONES = 4;
const int RACKS_PER_ZONE = 8;
const int SECTIONS_PER_RACK = 2;
const int SHELVES_PER_SECTION = 1;
const int CELL_CAPACITY = 10;
const int TOTAL_CAPACITY = NUM_ZONES * RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * CELL_CAPACITY; // 640

struct Cell {			// товары
    map<string, int> products;
    int total = 0;
};

map<string, Cell> warehouse;    // склад

// Инициализация склада
void initialize_warehouse() {
    for (char zone : ZONES) {
        for (int rack = 1; rack <= RACKS_PER_ZONE; rack++) {   //  стеллажи
            for (int section = 1; section <= SECTIONS_PER_RACK; section++) {  // секции
                for (int shelf = 1; shelf <= SHELVES_PER_SECTION; shelf++) { // полки
                    char address[6];
                    snprintf(address, sizeof(address), "%c%d%d%02d", zone, rack, section, shelf);  // обработка адреса без переполнения буфера (форматированный ввод)
                    warehouse[address] = Cell();  
			

		    // можно сделать по другому
		    /*
		    stringstream ss;
ss << zone << rack << section << setw(2) << setfill('0') << shelf;
string address = ss.str();
			*/
                }
            }
        }
    }
}

// Проверка корректности адреса
bool is_valid_address(const string& address) {
    if (address.length() != 5) return false;
    
    // Проверка зоны
    bool valid_zone = false;
    for (char zone : ZONES) {
        if (address[0] == zone) { // соответствие А, В и тд
            valid_zone = true;
            break;
        }
    }
    if (!valid_zone) return false;
    
    int rack = address[1] - '0';
    if (rack < 1 || rack > RACKS_PER_ZONE) return false;
    
    int section = address[2] - '0';
    if (section < 1 || section > SECTIONS_PER_RACK) return false;  // проверка на соответствие параметров склада
    
    int shelf = stoi(address.substr(3,2));
    return (shelf >= 1 && shelf <= SHELVES_PER_SECTION);
}

// Обработка команды ADD
void process_add(const vector<string>& tokens) {
    if (tokens.size() < 4) {  // если длина введенных параметров не соответствует
        cout << "ERROR: Invalid ADD format. Use: ADD <product> <quantity> <address>" << endl;
        return;
    }

    string product = tokens[1]; // продукт токен 1
    int quantity; // количество (макс 10)
    try {
        quantity = stoi(tokens[2]); // количество токен 2
        if (quantity <= 0) {
            cout << "ERROR: Quantity must be positive" << endl;
            return;
        }
    } catch (...) { // все остальные случаи
        cout << "ERROR: Invalid quantity" << endl;
        return;
    }

    string address = tokens[3]; // адрес на складе
    if (!is_valid_address(address)) {
        cout << "ERROR: Invalid address. Format: ZRSSS (Z-zone, R-rack, S-section, SS-shelf)" << endl;
        return;
    }

    Cell& cell = warehouse[address];  // проверка на вместимость
    if (cell.total + quantity > CELL_CAPACITY) {
        cout << "ERROR: Exceeds cell capacity (max " << CELL_CAPACITY << ")" << endl;
        return;
    }

    cell.products[product] += quantity;
    cell.total += quantity;
    cout << "Added " << quantity << " of " << product << " to " << address << endl;
}

// Обработка команды REMOVE
void process_remove(const vector<string>& tokens) {
    if (tokens.size() < 4) {
        cout << "ERROR: Invalid REMOVE format. Use: REMOVE <product> <quantity> <address>" << endl;
        return;
    }

    string product = tokens[1];
    int quantity;
    try {
        quantity = stoi(tokens[2]);
        if (quantity <= 0) {
            cout << "ERROR: Quantity must be positive" << endl;
            return;
        }
    } catch (...) {
        cout << "ERROR: Invalid quantity" << endl;
        return;
    }

    string address = tokens[3];
    if (!is_valid_address(address)) {
        cout << "ERROR: Invalid address" << endl;
        return;
    }

    Cell& cell = warehouse[address];
    if (cell.products.find(product) == cell.products.end() || cell.products[product] < quantity) {
        cout << "ERROR: Not enough products" << endl;
        return;
    }

    cell.products[product] -= quantity;
    cell.total -= quantity;
    if (cell.products[product] == 0) {
        cell.products.erase(product);
    }
    cout << "Removed " << quantity << " of " << product << " from " << address << endl;
}

// Обработка команды INFO
void process_info() {
    int total_products = 0;
    map<char, int> zone_products;
    map<string, int> product_counts;

    // Сбор статистики
    for (const auto& [address, cell] : warehouse) {
        total_products += cell.total;
        zone_products[address[0]] += cell.total;
        
        for (const auto& [product, qty] : cell.products) {
            product_counts[product] += qty;
        }
    }

    // Вывод общей информации
    cout << "\n=== Warehouse Status ===" << endl;
    cout << "Total capacity: " << TOTAL_CAPACITY << endl;
    cout << "Current load: " << fixed << setprecision(1) 
         << (static_cast<double>(total_products) / TOTAL_CAPACITY * 100)  // статик - вещественное деление вместо целочисленного, сетпресижн, ФИКСЕД - вывод с одной десятичной цифрой
         << "% (" << total_products << " items)" << endl;

    // Вывод по зонам
    cout << "\nZone loads:" << endl;
    for (char zone : ZONES) {
        double load = static_cast<double>(zone_products[zone]) / 
                     (RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * CELL_CAPACITY) * 100;
        cout << "Zone " << zone << ": " << fixed << setprecision(1) << load << "% (" 
             << zone_products[zone] << " items)" << endl;
    }

    // Вывод статистики по продуктам
    if (!product_counts.empty()) {
        cout << "\nProducts in warehouse:" << endl;
        for (const auto& [product, count] : product_counts) {
            cout << product << ": " << count << endl;
        }
    }

    // Вывод непустых ячеек
    int non_empty_count = 0;
    for (const auto& [address, cell] : warehouse) {
        if (cell.total > 0) non_empty_count++;
    }
    cout << "\nNon-empty cells: " << non_empty_count << "/" << warehouse.size() << endl;

    // Вывод пустых ячеек (по запросу)
    cout << "\nEnter 'SHOW EMPTY' to list empty cells or any command to continue..." << endl;
}

// Обработка команды SHOW EMPTY
void process_show_empty() {
    cout << "Empty cells:" << endl;
    int empty_count = 0;
    for (const auto& [address, cell] : warehouse) {
        if (cell.total == 0) {
            cout << address << " ";
            empty_count++;
            if (empty_count % 10 == 0) cout << endl;
        }
    }
    cout << "\nTotal empty: " << empty_count << endl;
}

int main() {
    initialize_warehouse();
    string line;

    cout << "=== Warehouse Management System ===" << endl;
    cout << "Available commands:" << endl;
    cout << "ADD <product> <quantity> <address>" << endl;
    cout << "REMOVE <product> <quantity> <address>" << endl;
    cout << "INFO - show warehouse status" << endl;
    cout << "SHOW EMPTY - list empty cells" << endl;
    cout << "QUIT - exit program" << endl;

    while (true) {
        cout << "\n> ";
        getline(cin, line);
        if (line.empty()) continue;

        istringstream iss(line);
        vector<string> tokens;
        string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) continue;

        string command = tokens[0];
        if (command == "ADD") {
            process_add(tokens);
        } else if (command == "REMOVE") {
            process_remove(tokens);
        } else if (command == "INFO") {
            process_info();
        } else if (command == "SHOW" && tokens.size() > 1 && tokens[1] == "EMPTY") {
            process_show_empty();
        } else if (command == "QUIT") {
            cout << "Exiting program..." << endl;
            break;
        } else {
            cout << "ERROR: Unknown command. Available commands: ADD, REMOVE, INFO, SHOW EMPTY, QUIT" << endl;
        }
    }

    return 0;
}
