#include "core.hpp"

#include "boost/asio/post.hpp"
#include "nlohmann/json.hpp"
#include "websocket_session.hpp"
#include <chrono>
#include <iostream>
#include <vector>

using json = nlohmann::json;

std::string to_str(enum class Core::state state) {
    switch (state) {
    case Core::state::on:
        return "on";
    case Core::state::off:
        return "off";
    case Core::state::running:
        return "running";
    case Core::state::calibration:
        return "calibration";
    default:
        return "wrong";
    }
}

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
                            session->write(getMonitorJson());
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

void Core::powerOff(Callback cb) {
    std::lock_guard lock(state_mutex_);
    if (transition_.contains({state_, state::off})) {
        state_ = state::off;
        std::cout << "[state] set off" << std::endl;
        cb(std::nullopt);
    } else {
        std::cout << "[state] cant set off from " << to_str(state_)
                  << std::endl;
        cb("cant set off");
    }
}
void Core::powerOn(Callback cb) {
    std::lock_guard lock(state_mutex_);
    if (transition_.contains({state_, state::on})) {
        state_ = state::on;
        std::cout << "[state] set on" << std::endl;
        cb(std::nullopt);
    } else {
        std::cout << "[state] cant set on from " << to_str(state_) << std::endl;
        cb("cant set on");
    }
}
void Core::setDispersion(Callback cb, float value) {
    std::lock_guard lock(state_mutex_);
    if (transition_.contains({state_, state::running})) {
        state_ = state::calibration;
        std::cout << "[state] set calibration" << std::endl;
        cb(std::nullopt);
    } else {
        std::cout << "[state] cant set calibration from " << to_str(state_)
                  << std::endl;
        cb("cant set calibration");
    }
}
void Core::setLambda(Callback cb, float value) {}

void Core::calibration(Callback cb) {
    std::lock_guard lock(state_mutex_);
    if (transition_.contains({state_, state::calibration})) {
        state_ = state::calibration;
        std::cout << "[state] set calibration" << std::endl;
        cb(std::nullopt);
    } else {
        std::cout << "[state] cant set calibration from " << to_str(state_)
                  << std::endl;
        cb("cant set calibration");
    }
}

void Core::stop(Callback cb) {
    std::lock_guard lock(state_mutex_);
    if (state_ == state::off) {
        cb("state is already off");
        return;
    }
    cb(std::nullopt);
    // todo stop
}

void Core::setMirrorPosition(Callback cb) {}
void Core::setGratePosition(Callback cb) {}
