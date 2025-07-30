// Copyright 2025 Danny Nguyen
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "RandWriter.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " k L\n";
        return 1;
    }

    size_t k = std::stoul(argv[1]);
    size_t L = std::stoul(argv[2]);

    // Read all input into a string
    std::ostringstream buffer;
    buffer << std::cin.rdbuf();
    std::string text = buffer.str();

    if (text.size() < k) {
        std::cerr << "Error: input text too short (length " << text.size()
                  << ") for order k=" << k << "\n";
        return 1;
    }

    try {
        RandWriter rw(text, k);
        std::string seed = text.substr(0, k);  // first k chars
        std::string output = rw.generate(seed, L);
        std::cout << output << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

