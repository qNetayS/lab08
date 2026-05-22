#include <iostream>
#include <fstream>
#include <cstdlib>

int main() {
    const char* log_path = std::getenv("LOG_PATH");
    std::ofstream log(log_path ? log_path : "/home/logs/log.txt");
    std::string line;
    
    std::cout << "Enter text (Ctrl+D to finish):" << std::endl;
    while (std::getline(std::cin, line)) {
        std::cout << "You entered: " << line << std::endl;
        if (log.is_open()) {
            log << line << std::endl;
        }
    }
    
    std::cout << "Log saved" << std::endl;
    return 0;
}
