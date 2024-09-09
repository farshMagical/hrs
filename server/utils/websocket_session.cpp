#include "websocket_session.hpp"

#include <iostream>

namespace {
void fail(boost::beast::error_code ec, char const *what) {
    std::cerr << what << ": " << ec.message() << "\n";
}
} // namespace

websocket_session::websocket_session(boost::asio::io_context &ioc,
                                     boost::asio::ip::tcp::socket &&socket)
    : ioc_(ioc), ws_(std::move(socket)) {}

void websocket_session::on_accept(boost::beast::error_code ec) {
    if (ec)
        return fail(ec, "accept");

    do_read();
}

void websocket_session::do_write(std::string message) {
    ws_.async_write(boost::asio::buffer(message, message.size()),
                    boost::beast::bind_front_handler(
                        &websocket_session::on_write, shared_from_this()));
}

size_t websocket_session::write(std::string message) {
    std::lock_guard lc(ws_mutex_);
    if (!finished) {
        return ws_.write(boost::asio::buffer(message, message.size()));
    }
    return 0;
}

void websocket_session::do_read() {
    ws_.async_read(buffer_,
                   boost::beast::bind_front_handler(&websocket_session::on_read,
                                                    shared_from_this()));
}

void websocket_session::on_read(boost::beast::error_code ec,
                                std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This indicates that the websocket_session was closed
    if (ec == boost::beast::websocket::error::closed) {
        {
            std::lock_guard lc(ws_mutex_);
            finished = true;
        }
        std::cout << "Close some websocket connection" << std::endl;
        return;
    }

    if (ec) {
        {
            std::lock_guard lc(ws_mutex_);
            finished = true;
        }
        fail(ec, "read");
        return;
    }

    buffer_.clear();
    do_read();
}

bool websocket_session::is_open() {
    std::lock_guard lc(ws_mutex_);
    return finished ? false : true;
}

void websocket_session::close() {
    std::lock_guard lc(ws_mutex_);
    finished = true;
}

void websocket_session::on_write(boost::beast::error_code ec,
                                 std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write");
}
