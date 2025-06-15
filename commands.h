#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <string>
#include <map>
#include <set>

enum class CommandType {
    CREATE_TRAIN,
    TRAINS_FOR_TOWN,
    TOWNS_FOR_TRAIN,
    TRAINS,
    UNKNOWN
};

// Определение типа команды по строке
CommandType parseCommand(const std::string& commandStr);

// Обработчики команд
void processCreateTrain(const std::vector<std::string>& tokens);
void processTrainsForTown(const std::vector<std::string>& tokens);
void processTownsForTrain(const std::vector<std::string>& tokens);
void processTrains();

#endif // COMMANDS_H
