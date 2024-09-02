#include "http_session.hpp"

#include "websocket_session.hpp"
#include <iostream>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>

namespace {
void fail(beast::error_code ec, char const *what) {
    std::cerr << what << ": " << ec.message() << "\n";
}
} // namespace

// Return a reasonable mime type based on the extension of a file.
boost::beast::string_view mime_type(boost::beast::string_view path) {
    using boost::beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (pos == boost::beast::string_view::npos)
            return boost::beast::string_view{};
        return path.substr(pos);
    }();
    if (iequals(ext, ".htm"))
        return "text/html";
    if (iequals(ext, ".html"))
        return "text/html";
    if (iequals(ext, ".php"))
        return "text/html";
    if (iequals(ext, ".css"))
        return "text/css";
    if (iequals(ext, ".txt"))
        return "text/plain";
    if (iequals(ext, ".js"))
        return "application/javascript";
    if (iequals(ext, ".json"))
        return "application/json";
    if (iequals(ext, ".xml"))
        return "application/xml";
    if (iequals(ext, ".swf"))
        return "application/x-shockwave-flash";
    if (iequals(ext, ".flv"))
        return "video/x-flv";
    if (iequals(ext, ".png"))
        return "image/png";
    if (iequals(ext, ".jpe"))
        return "image/jpeg";
    if (iequals(ext, ".jpeg"))
        return "image/jpeg";
    if (iequals(ext, ".jpg"))
        return "image/jpeg";
    if (iequals(ext, ".gif"))
        return "image/gif";
    if (iequals(ext, ".bmp"))
        return "image/bmp";
    if (iequals(ext, ".ico"))
        return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff"))
        return "image/tiff";
    if (iequals(ext, ".tif"))
        return "image/tiff";
    if (iequals(ext, ".svg"))
        return "image/svg+xml";
    if (iequals(ext, ".svgz"))
        return "image/svg+xml";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string path_cat(boost::beast::string_view base,
                     boost::beast::string_view path) {
    if (base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for (auto &c : result)
        if (c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}

// Return a response for the given request.
//
// The concrete type of the response message (which depends on the
// request), is type-erased in message_generator.
template <class Body, class Allocator>
boost::beast::http::message_generator
handle_request(boost::beast::string_view doc_root,
               boost::beast::http::request<
                   Body, boost::beast::http::basic_fields<Allocator>> &&req) {
    // Returns a bad request response
    auto const bad_request = [&req](boost::beast::string_view why) {
        boost::beast::http::response<boost::beast::http::string_body> res{
            boost::beast::http::status::bad_request, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found = [&req](boost::beast::string_view target) {
        boost::beast::http::response<boost::beast::http::string_body> res{
            boost::beast::http::status::not_found, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() =
            "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error = [&req](boost::beast::string_view what) {
        boost::beast::http::response<boost::beast::http::string_body> res{
            boost::beast::http::status::internal_server_error, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    // Make sure we can handle the method
    if (req.method() != boost::beast::http::verb::get &&
        req.method() != boost::beast::http::verb::head)
        return bad_request("Unknown HTTP-method");

    // Request path must be absolute and not contain "..".
    if (req.target().empty() || req.target()[0] != '/' ||
        req.target().find("..") != boost::beast::string_view::npos)
        return bad_request("Illegal request-target");

    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if (req.target() == "/monitor") {
        boost::beast::http::string_body::value_type body1;
        body1 = "BLYAAAA";

        // boost::beast::http::response<boost::beast::http::string_body>
        // res(boost::beast::http::status::ok, req.version());
        // res.set(boost::beast::http::field::server,
        // BOOST_BEAST_VERSION_STRING);
        // res.set(boost::beast::http::field::content_type, mime_type(path));
        // res.body() = "LOL"; res.prepare_payload(); return res;

        // Respond to GET request
        std::cout << "GET" << std::endl;
        boost::beast::http::response<boost::beast::http::string_body> res{
            boost::beast::http::status::ok,
            req.version(),
        };
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, mime_type(path));
        res.set("key", "I am a header2");
        res.body() = "HUI";
        res.content_length(body1.size());
        res.keep_alive(req.keep_alive());
        res.prepare_payload();
        return res;
    }

    // Attempt to open the file
    boost::beast::error_code ec;
    boost::beast::http::file_body::value_type body;
    body.open(path.c_str(), boost::beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if (ec == boost::beast::errc::no_such_file_or_directory)
        return not_found(req.target());

    // Handle an unknown error
    if (ec)
        return server_error(ec.message());

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if (req.method() == boost::beast::http::verb::head) {
        boost::beast::http::response<boost::beast::http::empty_body> res{
            boost::beast::http::status::ok, req.version()};
        res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(boost::beast::http::field::content_type, mime_type(path));
        req.set("key", "I am a header");
        req.body() = "lol";
        // res.content_length(req.body().size());
        res.keep_alive(req.keep_alive());
        return res;
    }

    boost::beast::http::string_body::value_type body1;
    body1 = "BLYAAAA";
    // Respond to GET request
    std::cout << "GET" << std::endl;
    boost::beast::http::response<boost::beast::http::string_body> res{
        std::piecewise_construct, std::make_tuple(std::move(body1)),
        std::make_tuple(boost::beast::http::status::ok, req.version())};
    res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(boost::beast::http::field::content_type, mime_type(path));
    res.set("key", "I am a header2");
    res.content_length(body1.size());
    res.keep_alive(req.keep_alive());
    return res;
}

// Take ownership of the socket
http_session::http_session(boost::asio::ip::tcp::socket &&socket,
                           std::shared_ptr<std::string const> const &doc_root)
    : stream_(std::move(socket)), doc_root_(doc_root) {
    static_assert(queue_limit > 0, "queue limit must be positive");
    response_queue_.reserve(queue_limit);
}

// Start the session
void http_session::run() {
    boost::asio::dispatch(stream_.get_executor(),
                          beast::bind_front_handler(&http_session::do_read,
                                                    this->shared_from_this()));
}

void http_session::do_read() {
    // Construct a new parser for each message
    parser_.emplace();

    // Apply a reasonable limit to the allowed size
    // of the body in bytes to prevent abuse.
    parser_->body_limit(10000);

    // Set the timeout.
    stream_.expires_after(std::chrono::seconds(30));

    // Read a request using the parser-oriented interface
    http::async_read(
        stream_, buffer_, *parser_,
        beast::bind_front_handler(&http_session::on_read, shared_from_this()));
}

void http_session::on_read(beast::error_code ec,
                           std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if (ec == http::error::end_of_stream)
        return do_close();

    if (ec)
        return fail(ec, "read");

    // See if it is a WebSocket Upgrade
    if (boost::beast::websocket::is_upgrade(parser_->get())) {
        std::cout << "try to WEBSOCKET" << std::endl;
        if (parser_->get().target() == "/monitor") {
            std::cout << "WEBSOCKET" << std::endl;
            std::make_shared<websocket_session>(stream_.release_socket())
                ->do_accept(parser_->release());
            return;
        }
    }

    // Send the response
    queue_write(handle_request(*doc_root_, parser_->release()));

    // If we aren't at the queue limit, try to pipeline another request
    if (response_queue_.size() < queue_limit)
        do_read();
}

void http_session::queue_write(http::message_generator response) {
    // Allocate and store the work
    response_queue_.push_back(std::move(response));

    // If there was no previous work, start the write
    // loop
    if (response_queue_.size() == 1)
        do_write();
}

// Called to start/continue the write-loop. Should not be called when
// write_loop is already active.
//
// Returns `true` if the caller may initiate a new read
bool http_session::do_write() {
    bool const was_full = response_queue_.size() == queue_limit;

    if (!response_queue_.empty()) {
        http::message_generator msg = std::move(response_queue_.front());
        response_queue_.erase(response_queue_.begin());

        bool keep_alive = msg.keep_alive();

        beast::async_write(stream_, std::move(msg),
                           beast::bind_front_handler(&http_session::on_write,
                                                     shared_from_this(),
                                                     keep_alive));
    }

    return was_full;
}

void http_session::on_write(bool keep_alive, beast::error_code ec,
                            std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write");

    if (!keep_alive) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return do_close();
    }

    // Inform the queue that a write completed
    if (do_write()) {
        // Read another request
        do_read();
    }
}

void http_session::do_close() {
    // Send a TCP shutdown
    beast::error_code ec;
    stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

    // At this point the connection is closed gracefully
}
