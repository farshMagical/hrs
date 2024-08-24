#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <memory>
#include <string>

#include <fmt/format.h>

namespace beast = boost::beast;

// Обработчик соединения
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
  beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;

public:
  explicit WebSocketSession(boost::asio::ip::tcp::socket socket)
      : ws_(std::move(socket)) {
    fmt::print("New session\n");
  }

  void run() {
    // Запускаем асинхронный прием соединения
    ws_.async_accept(beast::bind_front_handler(&WebSocketSession::on_accept,
                                               shared_from_this()));
  }

private:
  void on_accept(beast::error_code ec) {
    if (ec) {
      std::cerr << "Error on accept: " << ec.message() << std::endl;
      return;
    }

    fmt::print("Reading\n");
    do_read();
  }

  void do_read() {
    ws_.async_read(buffer_,
                   beast::bind_front_handler(&WebSocketSession::on_read,
                                             shared_from_this()));
  }

  void on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if (ec) {
      std::cerr << "Error on read: " << ec.message() << std::endl;
      return;
    }

    // Эхо-ответ
    ws_.async_write(buffer_.data(),
                    beast::bind_front_handler(&WebSocketSession::on_write,
                                              shared_from_this()));
  }

  void on_write(beast::error_code ec, std::size_t bytes_transferred) {
    buffer_.consume(bytes_transferred);
    if (ec) {
      std::cerr << "Error on write: " << ec.message() << std::endl;
      return;
    }
    do_read();
  }

  beast::flat_buffer buffer_; // Буфер для чтения сообщений
};

// Запуск сервера
class WebSocketServer {
  boost::asio::io_context ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;

public:
  WebSocketServer(short port)
      : acceptor_(ioc_, boost::asio::ip::tcp::endpoint(
                            boost::asio::ip::tcp::v4(), port)) {
    do_accept();
  }

  void run() { ioc_.run(); }

private:
  void do_accept() {
    acceptor_.async_accept(
        [this](beast::error_code ec, boost::asio::ip::tcp::socket socket) {
          if (!ec) {
            std::make_shared<WebSocketSession>(std::move(socket))->run();
          }
          do_accept(); // Принимаем следующее соединение
        });
  }
};

int main() {
  try {
    WebSocketServer server(3000);
    std::cout << "Server is running on port 3000..." << std::endl;
    server.run();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
