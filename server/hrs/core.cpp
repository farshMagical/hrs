#include "core.hpp"

#include "boost/asio/post.hpp"
#include "nlohmann/json.hpp"
#include "websocket_session.hpp"
#include <chrono>
#include <iostream>
#include <vector>

using json = nlohmann::json;

Core &Core::GetCore() {
    static Core core;
    return core;
}

Core::Core() : loop_(&Core::loopFunction, this) {}

void Core::loopFunction() {
    while (1) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            for (auto session : web_sessions_) {
                if (session->is_open()) {
                    boost::asio::post(*ioc_, [this, &session]() {
                        try {
                            // Here make packets with usefull data
                            std::cout << session->write(getMonitorJson())
                                      << std::endl;
                        } catch (const boost::system::system_error &e) {
                            std::cerr << "Error during write: " << e.what()
                                      << " (error mes: " << e.code() << ")\n";
                            session->close();
                        }
                    });
                }
            }
        }
        web_sessions_.remove_if(
            [](auto &session) { return !session->is_open(); });
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Core::setWebsockerSession(std::shared_ptr<websocket_session> ws) {
    std::lock_guard<std::mutex> lock(mutex_);
    web_sessions_.push_back(ws);
}

void Core::setIOContext(boost::asio::io_context &ioc) {
    if (ioc_ == nullptr) {
        ioc_ = &ioc;
    }
}

std::string Core::getMonitorJson() {
    json j;
    j["mirror"]["connection"] = true;
    j["mirror"]["position"] = 12.34;
    j["mirror"]["limitSwitch"] = false;
    j["mirror"]["status"] = "waiting";
    j["grate"]["connection"] = false;
    j["grate"]["position"] = 43.21;
    j["grate"]["limitSwitch"] = false;
    j["grate"]["status"] = "busy";
    return j.dump();
}