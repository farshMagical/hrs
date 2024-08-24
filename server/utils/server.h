#pragma once

#include <string>

#include <fmt/format.h>

#include "utils/session.h"

namespace beast = boost::beast;

namespace ws {

class Server {
  public:
    Server(short port);
    void Run();

  private:
    void do_accept();

  private:
    boost::asio::io_context ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
};

} // namespace ws
