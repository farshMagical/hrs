#include <format>

#include "core.hpp"
#include "listener.hpp"


int main(int argc, char *argv[]) {

    Core::GetCore();

    auto const address = boost::asio::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(3000);
    auto const doc_root = std::make_shared<std::string>(".");

    boost::asio::io_context ioc;

    std::make_shared<listener>(
        ioc, boost::asio::ip::tcp::endpoint{address, port}, doc_root)
        ->run();

    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&](boost::beast::error_code const &, int) { ioc.stop(); });

    ioc.run();

    return EXIT_SUCCESS;
}
