#include <iostream>       
#include <vector>         
#include <algorithm>      
#include <iomanip>        
#include <sstream>        

using namespace std;      

// Структура для хранения информации о посетителе
struct Visitor {
    string ticket;  // Номер билета (например, "T001")
    int time;       // Время обслуживания в минутах
    int order;      // Порядковый номер в очереди
};

// Функция сравнения для сортировки по времени (по убыванию)
bool compareByTimeDesc(const Visitor &a, const Visitor &b) {
    if (a.time == b.time)          // Если время одинаковое
        return a.order < b.order;  // Сортируем по порядковому номеру
    return a.time > b.time;        // Иначе сортируем по убыванию времени
}

// Функция сравнения для сортировки по порядковому номеру (по возрастанию)
bool compareByOrder(const Visitor &a, const Visitor &b) {
    return a.order < b.order;
}

int main() {
    // Запрос количества окон
    int windowsCount;
    cout << ">>> Введите кол-во окон" << endl;
    cin >> windowsCount;          // Чтение количества окон
    cin.ignore();                 // Очистка буфера после ввода числа

    vector<Visitor> visitors;     // Вектор для хранения посетителей
    string command;               // Переменная для хранения команды
    int orderCounter = 0;         // Счетчик для генерации номеров билетов

    // Основной цикл обработки команд
    while (true) {
        getline(cin, command);    // Чтение всей строки ввода
        if (command.empty()) continue; // Пропуск пустых строк
        
        // Обработка команды ENQUEUE
        if (command.find("ENQUEUE") == 0) {
            int time;
            stringstream ss(command); // Создание строкового потока
            string cmd;
            ss >> cmd >> time;        // Извлечение команды и времени
            
            // Генерация номера билета (формат T001, T002 и т.д.)
            stringstream ticketStream;
            ticketStream << "T" << setw(3) << setfill('0') << orderCounter;
            
            // Добавление посетителя в вектор
            visitors.push_back({ticketStream.str(), time, orderCounter});
            orderCounter++;            // Увеличение счетчика
            
            // Вывод номера билета добавленного посетителя
            cout << ">>> " << ticketStream.str() << endl;
        } 
        // Обработка команды DISTRIBUTE (завершение ввода)
        else if (command == "DISTRIBUTE") {
            break;
        }
    }

    // Инициализация окон
    vector<vector<Visitor>> windows(windowsCount); // Вектор окон с посетителями
    vector<int> windowTime(windowsCount, 0);       // Время обработки для каждого окна
    
    // Сортировка посетителей по времени (самые долгие сначала)
    sort(visitors.begin(), visitors.end(), compareByTimeDesc);
    
    // Распределение посетителей по окнам
    for (const auto& v : visitors) {
        int minIndex = 0;                      // Индекс окна с минимальной загрузкой
        int minTime = windowTime[0];           // Время обработки в первом окне
        
        // Поиск наименее загруженного окна
        for (int i = 1; i < windowsCount; i++) {
            if (windowTime[i] < minTime) {
                minTime = windowTime[i];
                minIndex = i;
            }
        }
        
        // Добавление посетителя в найденное окно
        windows[minIndex].push_back(v);
        windowTime[minIndex] += v.time;       // Увеличение времени обработки окна
    }
    
    // Сортировка посетителей в каждом окне по порядку прибытия
    for (int i = 0; i < windowsCount; i++) {
        sort(windows[i].begin(), windows[i].end(), compareByOrder);
    }
    
    // Вывод результатов распределения
    cout << ">>> ";
    for (int i = 0; i < windowsCount; i++) {
        // Вывод информации об окне
        cout << "Окно " << i+1 << " (" << windowTime[i] << " минут): ";
        
        // Вывод билетов посетителей в окне
        for (size_t j = 0; j < windows[i].size(); j++) {
            cout << windows[i][j].ticket;
            if (j < windows[i].size() - 1) 
                cout << ", ";  // Разделитель между билетами
        }
        if (i < windowsCount - 1) 
            cout << " >>> ";   // Разделитель между окнами
    }
    cout << endl;

    return 0;
}
