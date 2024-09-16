#pragma once

#include <boost/asio.hpp>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <thread>
#include <utility>

class websocket_session;

using Callback = std::function<void(std::optional<std::string> error)>;

class Core {
  public:
    enum class state { off, on, running, calibration };

    static Core &GetCore();

    void setWebsockerSession(std::shared_ptr<websocket_session> ws);
    void setIOContext(boost::asio::io_context &ioc);

    void powerOff(Callback cb);
    void powerOn(Callback cb);
    void setDispersion(Callback cb, float value);
    void setLambda(Callback cb, float value); // for wave
    void calibration(Callback cb);

    void stop(Callback cb);
    void setMirrorPosition(Callback cb);
    void setGratePosition(Callback cb);

  private:
    Core();

    std::set<std::pair<state, state>> transition_ = {
        {state::off, state::on},     {state::on, state::off},
        {state::on, state::running}, {state::on, state::calibration},
        {state::running, state::on}, {state::calibration, state::on},
    };

    std::string getMonitorJson();

    void loopFunction();

    std::mutex state_mutex_;
    state state_{state::off};

    std::mutex mutex_;
    std::list<std::shared_ptr<websocket_session>> web_sessions_;

    boost::asio::io_context *ioc_;
    std::thread loop_;
};