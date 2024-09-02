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

#include <memory>
#include <string>
#include <vector>

// Handles an HTTP server connection
class http_session : public std::enable_shared_from_this<http_session> {
    boost::beast::tcp_stream stream_;
    boost::beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_;

    static constexpr std::size_t queue_limit = 8; // max responses
    std::vector<boost::beast::http::message_generator> response_queue_;

    // The parser is stored in an optional container so we can
    // construct it from scratch it at the beginning of each new message.
    boost::optional<
        boost::beast::http::request_parser<boost::beast::http::string_body>>
        parser_;

  public:
    // Take ownership of the socket
    http_session(boost::asio::ip::tcp::socket &&socket,
                 std::shared_ptr<std::string const> const &doc_root);

    // Start the session
    void run();

  private:
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void queue_write(boost::beast::http::message_generator response);

    // Called to start/continue the write-loop. Should not be called when
    // write_loop is already active.
    //
    // Returns `true` if the caller may initiate a new read
    bool do_write();
    void on_write(bool keep_alive, boost::beast::error_code ec,
                  std::size_t bytes_transferred);
    void do_close();
};