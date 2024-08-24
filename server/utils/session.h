#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace ws {

namespace beast = boost::beast;

class Session : public std::enable_shared_from_this<Session> {
  public:
    explicit Session(boost::asio::ip::tcp::socket socket);
    void Run();

  private:
    void on_accept(beast::error_code ec);
    void do_read();
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void on_write(beast::error_code ec, std::size_t bytes_transferred);

    beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    beast::flat_buffer buffer_;
};

} // namespace ws