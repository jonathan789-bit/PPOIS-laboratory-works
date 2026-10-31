#include "PostMachine.h"
#include "Vector3D.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

static double readDouble(const char* prompt) {
    double value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Please enter a valid number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

static void vectorMenu() {
    Vector3D a(readDouble("Vector A - x: "), readDouble("Vector A - y: "), readDouble("Vector A - z: "));
    Vector3D b(readDouble("Vector B - x: "), readDouble("Vector B - y: "), readDouble("Vector B - z: "));
    std::cout << "\nA = " << a << "\nB = " << b
              << "\n|A| = " << a.length() << "\n|B| = " << b.length()
              << "\nA + B = " << a + b
              << "\nA - B = " << a - b
              << "\nA x B = " << a * b
              << "\nA * 2 = " << a * 2.0
              << "\nA / 2 = " << a / 2.0 << '\n';
    try { std::cout << "cos(angle) = " << a.cosine(b) << '\n'; }
    catch (const std::domain_error& e) { std::cout << e.what() << '\n'; }
    std::cout << "Length comparison: ";
    if (a > b) std::cout << "A is longer\n";
    else if (a < b) std::cout << "B is longer\n";
    else std::cout << "Equal lengths\n";
}

static void postMachineMenu() {
    // Program: write 1, move right, write 1, halt.
    std::vector<PostInstruction> program = {
        PostInstruction::write1(), PostInstruction::right(),
        PostInstruction::write1(), PostInstruction::halt()
    };
    PostMachine machine(program);
    const auto result = machine.run(100);
    std::cout << "\nExample Post machine executed " << result.steps << " steps.\n"
              << "Halted: " << (result.halted ? "yes" : "no")
              << "\nTape cell 0: " << machine.tapeAt(0)
              << "\nTape cell 1: " << machine.tapeAt(1)
              << "\nHead position: " << machine.headPosition() << "\n\n";
    std::cout << "Serialized machine:\n" << machine;
}

int main() {
    while (true) {
        std::cout << "\n===== OOP LAB 1 =====\n"
                  << "1. Vector3D demonstration\n"
                  << "2. Post Machine demonstration\n"
                  << "0. Exit\n"
                  << "Choose: ";
        int choice;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) break;
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        try {
            if (choice == 1) vectorMenu();
            else if (choice == 2) postMachineMenu();
            else if (choice == 0) break;
            else std::cout << "Choose 0, 1, or 2.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}
