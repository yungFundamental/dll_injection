#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <process_name>" << std::endl;
        return 1;
    }

    std::cout << "Starting inject process on \"" << argv[1] << "\" process..." << std::endl;

    return 0;
}
