#include "file_reader.hpp"
#include "entropy_calculator.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    FileReader reader(argv[1]);
    if (!reader.read_file()) {
        std::cerr << "Error: " << reader.get_error_message() << "\n";
        return 1;
    }

    EntropyCalculator calculator(reader.get_data());
    double entropy = calculator.calculate_entropy();

    std::cout << "Entropy: " << entropy << " bits/byte\n";

    return 0;
}
