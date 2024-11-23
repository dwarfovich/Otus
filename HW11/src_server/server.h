#pragma once

#include "session.h"
#include "request_processor.h"

#include "common_aux/boost_aliases.h"
#include "common_aux/port_number.h"

#include <unordered_set>

class Server
{
public:
    Server(basio::io_context& ioContext, PortNumber port, std::size_t maxSessions = 10)
        : acceptor_ { ioContext, BoostTcp::endpoint { BoostTcp::v4(), port } }, maxSessions_ { maxSessions }
    {
        if (port == 0) {
            throw std::runtime_error("Port cann't be zero");
        }

        startAccepting();
    }

    void setRequestProcessor(std::unique_ptr<RequestProcessor> processor){
        requestProcessor_ = std::move(processor);
    }

    std::string handleInput(std::string input) const{
        std::cout << "Handling input: " << input << '\n';
        return requestProcessor_->process(input).value_or("");
    }

    void removeSession(Session* session)
    {
        const auto iter = sessions_.find(session);
        sessions_.erase(iter);
    }

private: // methods
    void startAccepting()
    {
        acceptor_.async_accept([this](const BoostErrorCode& ec, BoostTcp::socket socket) {
            if (!ec) {
                if (sessions_.size() < maxSessions_) {
                    const auto [iter, success] = sessions_.insert(std::make_unique<Session>(std::move(socket), this));
                    if (success) {
                        (*iter)->startAsyncReadData();
                    }
                } else {
                    g_debugOut << "Refusing to establish incomming connection due maximum sessions already reached.\n";
                }
            } else {
                g_debugOut << "Failed to accept connection. Error: " + ec.message();
            }

            startAccepting();
        });
    }

private: // data
    BoostTcp::acceptor acceptor_;
    const std::size_t  maxSessions_;
    using SessionsUset = std::unordered_set<std::unique_ptr<Session>, SessionHasher, SessionComparator>;
    SessionsUset sessions_;
    std::unique_ptr<RequestProcessor> requestProcessor_;
};