#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <memory>

class listener : public std::enable_shared_from_this<listener> {
  public:
    listener(boost::asio::io_context &ioc,
             boost::asio::ip::tcp::endpoint endpoint,
             std::shared_ptr<std::string const> const &doc_root);
    void run();

  private:
    void do_accept();
    void on_accept(boost::beast::error_code ec,
                   boost::asio::ip::tcp::socket socket);

  private:
    boost::asio::io_context &ioc_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;
};
