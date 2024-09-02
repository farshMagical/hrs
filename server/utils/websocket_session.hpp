#pragma once

#include <algorithm>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/make_unique.hpp>
#include <boost/optional.hpp>

#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <memory>

// Echoes back all received WebSocket messages
class websocket_session
    : public std::enable_shared_from_this<websocket_session> {
  public:
    // Take ownership of the socket
    explicit websocket_session(boost::asio::ip::tcp::socket &&socket);

    // Start the asynchronous accept operation
    template <class Body, class Allocator>
    void do_accept(boost::beast::http::request<
                   Body, boost::beast::http::basic_fields<Allocator>>
                       req) {
        // Set suggested timeout settings for the websocket
        ws_.set_option(boost::beast::websocket::stream_base::timeout::suggested(
            boost::beast::role_type::server));

        // Set a decorator to change the Server of the handshake
        ws_.set_option(boost::beast::websocket::stream_base::decorator(
            [](boost::beast::websocket::response_type &res) {
                res.set(boost::beast::http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                            " advanced-server");
            }));

        // Accept the websocket handshake
        ws_.async_accept(
            req, boost::beast::bind_front_handler(&websocket_session::on_accept,
                                                  shared_from_this()));
    }

  private:
    void on_accept(boost::beast::error_code ec);
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);

  private:
    boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
    boost::beast::flat_buffer buffer_;
};
