#include <format>
#include <iostream>

#include "utils/server.h"

int main() {
    try {
        ws::Server server(3000);
        std::cout << std::format("Server is running on port 3000...\n");
        server.Run();
    } catch (const std::exception &e) {
        std::cerr << std::format("Error: {}\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
