#include "commands.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        
        while (iss >> token) {
            tokens.push_back(token);
        }
        
        if (tokens.empty()) continue;
        
        CommandType cmd = parseCommand(tokens[0]);
        tokens.erase(tokens.begin());
        
        switch (cmd) {
            case CommandType::CREATE_TRAIN:
                processCreateTrain(tokens);
                break;
            case CommandType::TRAINS_FOR_TOWN:
                processTrainsForTown(tokens);
                break;
            case CommandType::TOWNS_FOR_TRAIN:
                processTownsForTrain(tokens);
                break;
            case CommandType::TRAINS:
                processTrains();
                break;
            default:
                std::cout << "Unknown command" << std::endl;
        }
    }
    return 0;
}
