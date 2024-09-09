#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <list>


class websocket_session;

class Core {
  public:
    static Core &GetCore();

    void setWebsockerSession(std::shared_ptr<websocket_session> ws);
    void setIOContext(boost::asio::io_context &ioc);

  private:
    Core();

    std::string getMonitorJson();

    void loopFunction();

    std::mutex mutex_;
    std::list<std::shared_ptr<websocket_session>> web_sessions_;

    boost::asio::io_context *ioc_;
    std::thread loop_;
};