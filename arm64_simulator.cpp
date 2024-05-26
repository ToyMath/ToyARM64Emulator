#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

class ARM64Emulator {
private:
    std::unordered_map<std::string, int> registers;
    std::unordered_map<int, int> memory;
    std::vector<std::string> instructions;
    std::unordered_map<std::string, int> labels;
    int pc;

public:
    ARM64Emulator() : pc(0) {
        for (int i = 0; i < 31; i++) {
            registers["x" + std::to_string(i)] = 0;
        }
    }

    void loadProgram(const std::string& program) {
        std::istringstream stream(program);
        std::string line;
        while (std::getline(stream, line)) {
            std::string trimmed = trim(line);
            if (!trimmed.empty()) {
                instructions.push_back(trimmed);
            }
        }
        parseLabels();
    }

    void parseLabels() {
        for (size_t i = 0; i < instructions.size(); i++) {
            const std::string& line = instructions[i];
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string label = trim(line.substr(0, colonPos));
                labels[label] = i;
            }
        }
    }

    void run() {
        while (pc < instructions.size()) {
            const std::string& line = instructions[pc];
            if (line.back() != ':') {
                executeInstruction(line);
            }
            pc++;
        }
    }

    void executeInstruction(const std::string& line) {
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string part;
        while (iss >> part) {
            parts.push_back(part);
        }

        const std::string& cmd = parts[0];

        if (cmd == "ldr") {
            std::string reg = parts[1].substr(0, parts[1].length() - 1); // remove trailing comma
            std::string value = parts[2];
            if (value[0] == '=') {
                int addr = std::stoi(value.substr(1));
                registers[reg] = addr;
            } else {
                int addr = registers[value.substr(1, value.length() - 2)];
                registers[reg] = memory[addr];
            }
        } else if (cmd == "str") {
            std::string value = parts[1].substr(0, parts[1].length() - 1);
            std::string reg = parts[2].substr(1, parts[2].length() - 2);
            int addr = registers[reg];
            memory[addr] = registers[value];
        } else if (cmd == "add") {
            std::string dest = parts[1].substr(0, parts[1].length() - 1);
            std::string src1 = parts[2].substr(0, parts[2].length() - 1);
            std::string src2 = parts[3];
            registers[dest] = registers[src1] + registers[src2];
        } else if (cmd == "mul") {
            std::string dest = parts[1].substr(0, parts[1].length() - 1);
            std::string src1 = parts[2].substr(0, parts[2].length() - 1);
            std::string src2 = parts[3];
            registers[dest] = registers[src1] * registers[src2];
        } else if (cmd == "mov") {
            std::string reg = parts[1].substr(0, parts[1].length() - 1);
            int value = std::stoi(parts[2].substr(1));
            registers[reg] = value;
        } else if (cmd == "b") {
            std::string label = parts[1];
            pc = labels[label] - 1;
        } else {
            std::cout << "Unknown instruction: " << cmd << std::endl;
        }
    }

    void printMemory() {
        std::cout << "Memory:" << std::endl;
        for (const auto& [k, v] : memory) {
            std::cout << k << ": " << v << std::endl;
        }
    }

    void printRegisters() {
        std::cout << "Registers:" << std::endl;
        for (const auto& [k, v] : registers) {
            std::cout << k << ": " << v << std::endl;
        }
    }

    void initializeMemory(const std::unordered_map<std::string, int>& memoryInit) {
        for (const auto& [key, value] : memoryInit) {
            memory[std::stoi(key)] = value;
        }
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        size_t last = str.find_last_not_of(" \t");
        return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, (last - first + 1));
    }
};

int main() {
    std::string program = 
        "ldr x0, =5\n"
        "ldr x1, [x0]\n"
        "ldr x0, =7\n"
        "ldr x2, [x0]\n"
        "add x3, x1, x2\n"
        "ldr x0, =3\n"
        "ldr x4, [x0]\n"
        "mul x5, x3, x4\n"
        "ldr x0, =0\n"
        "str x5, [x0]\n";

    std::unordered_map<std::string, int> memoryInit = {
        {"5", 5},
        {"7", 7},
        {"3", 3},
        {"0", 0}
    };

    ARM64Emulator emulator;
    emulator.initializeMemory(memoryInit);
    emulator.loadProgram(program);
    emulator.run();
    emulator.printRegisters();
    emulator.printMemory();

    return 0;
}
