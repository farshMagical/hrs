#include "session.h"

#include <fmt/format.h>
#include <format>
#include <iostream>


using namespace std;

namespace ws {

Session::Session(boost::asio::ip::tcp::socket socket)
    : wstream_(std::move(socket)) {
    cout << format("Session const\n");
}

void Session::Run() {
    wstream_.async_accept(
        beast::bind_front_handler(&Session::on_accept, shared_from_this()));
}

void Session::on_accept(beast::error_code ec) {
    if (ec) {
        std::cerr << "Error on accept: " << ec.message() << std::endl;
        return;
    }
    cout << format("Reading\n");
    do_read();
}

void Session::do_read() {
    wstream_.async_read(buffer_, beast::bind_front_handler(&Session::on_read,
                                                           shared_from_this()));
    // wstream_.
}

void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if (ec) {
        std::cerr << "Error on read: " << ec.message() << std::endl;
        return;
    }

    // Эхо-ответ
    wstream_.async_write(
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
