#include "server.h"

#include <memory>

namespace ws {

Server::Server(short port)
    : acceptor_(ioc_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                                     port)) {
    do_accept();
}

void Server::Run() { ioc_.run(); }

void Server::do_accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<ws::Session>(std::move(socket))->Run();
            }
            do_accept();
        });
}

} // namespace ws