#include <fmt/format.h>

#include "utils/server.h"

int main() {
    try {
        ws::Server server(3000);
        fmt::print("Server is running on port 3000...\n");
        server.Run();
    } catch (const std::exception &e) {
        fmt::print(stderr, "Error: {}\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
