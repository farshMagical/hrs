#include "session.h"

#include <fmt/format.h>
#include <iostream>

namespace ws {

Session::Session(boost::asio::ip::tcp::socket socket) : ws_(std::move(socket)) {
    fmt::print("ws constructor\n");
}

void Session::Run() {
    ws_.async_accept(
        beast::bind_front_handler(&Session::on_accept, shared_from_this()));
}

void Session::on_accept(beast::error_code ec) {
    if (ec) {
        std::cerr << "Error on accept: " << ec.message() << std::endl;
        return;
    }
    fmt::print("Reading\n");
    do_read();
}

void Session::do_read() {
    ws_.async_read(buffer_, beast::bind_front_handler(&Session::on_read,
                                                      shared_from_this()));
}

void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if (ec) {
        std::cerr << "Error on read: " << ec.message() << std::endl;
        return;
    }

    // Эхо-ответ
    ws_.async_write(
        buffer_.data(),
        beast::bind_front_handler(&Session::on_write, shared_from_this()));
}

void Session::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    buffer_.consume(bytes_transferred);
    if (ec) {
        std::cerr << "Error on write: " << ec.message() << std::endl;
        return;
    }
    do_read();
}

} // namespace ws
