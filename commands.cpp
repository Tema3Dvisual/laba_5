#include "commands.h"
#include <iostream>
#include <algorithm>
#include <unordered_set>

// Глобальные структуры данных
std::map<std::string, std::vector<std::string>> trainSchedule;
std::map<std::string, std::set<std::string>> townTrains;

CommandType parseCommand(const std::string& commandStr) {
    if (commandStr == "CREATE_TRAIN") return CommandType::CREATE_TRAIN;
    if (commandStr == "TRAINS_FOR_TOWN") return CommandType::TRAINS_FOR_TOWN;
    if (commandStr == "TOWNS_FOR_TRAIN") return CommandType::TOWNS_FOR_TRAIN;
    if (commandStr == "TRAINS") return CommandType::TRAINS;
    return CommandType::UNKNOWN;
}

void processCreateTrain(const std::vector<std::string>& tokens) {
    // Проверка на минимальное количество аргументов (имя поезда + хотя бы 2 города)
    if (tokens.size() < 3) {
        std::cout << "Error: Train must have at least 2 or more towns in route" << std::endl;
        return;
    }
    
    std::string trainName = tokens[0];
    std::vector<std::string> route(tokens.begin() + 1, tokens.end());
    
    // Проверка на дубликаты городов в маршруте
    std::unordered_set<std::string> uniqueTowns;
    for (const auto& town : route) {
        if (uniqueTowns.count(town)) {
            std::cout << "Error: Duplicate town " << town << " in route" << std::endl;
            return;
        }
        uniqueTowns.insert(town);
    }
    
    // Проверка на существующий поезд
    if (trainSchedule.count(trainName)) {
        std::cout << "Error: Train " << trainName << " already exists" << std::endl;
        return;
    }
    
    // Добавление нового маршрута
    trainSchedule[trainName] = route;
    for (const auto& town : route) {
        townTrains[town].insert(trainName);
    }
    
    std::cout << "Train " << trainName << " created successfully with " << route.size() << " towns" << std::endl;
}



// проверка названия города, поиск города
void processTrainsForTown(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        std::cout << "Incorrect command" << std::endl;
        return;
    }
    
    std::string town = tokens[0];
    auto it = townTrains.find(town);
    
    if (it == townTrains.end() || it->second.empty()) {
        std::cout << "No trains for town " << town << std::endl;
        return;
    }
    
    std::cout << "Trains for town " << town << ":";
    for (const auto& train : it->second) {
        std::cout << " " << train;
    }
    std::cout << std::endl;
}




// проверка названия поезда, поиск поезда, для каждого города: название города, поезда через город, удаление поезда из списка, вывод оставшихся или no connection
void processTownsForTrain(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        std::cout << "Incorrect command" << std::endl;
        return;
    }
    
    std::string trainName = tokens[0];
    auto it = trainSchedule.find(trainName);
    
    if (it == trainSchedule.end()) {
        std::cout << "Train " << trainName << " not found" << std::endl;
        return;
    }
    
    std::cout << "Connections for train " << trainName << ":" << std::endl;
    for (const auto& town : it->second) {
        std::cout << "  " << town << ":";
        auto trains = townTrains[town];
        trains.erase(trainName);
        
        if (trains.empty()) {
            std::cout << " no connections";
        } else {
            for (const auto& t : trains) {
                std::cout << " " << t;
            }
        }
        std::cout << std::endl;
    }
}


// есть ли поезда в расписании, вывод списка поездов с маршрутами
void processTrains() {
    if (trainSchedule.empty()) {
        std::cout << "No trains in schedule" << std::endl;
        return;
    }
    
    std::cout << "All trains:" << std::endl;
    for (const auto& [train, route] : trainSchedule) {
        std::cout << "  " << train << ":";
        for (const auto& town : route) {
            std::cout << " " << town;
        }
        std::cout << std::endl;
    }
}
