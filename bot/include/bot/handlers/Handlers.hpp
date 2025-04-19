#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include "bot/database/UserStorage.hpp"

#include <string>

#include "tgbot/types/CallbackQuery.h" 
#include "tgbot/types/Message.h"
#include <tgbot/tgbot.h>
#include <spdlog/spdlog.h>
#include "spdlog/async.h"

using namespace TgBot;


namespace command_handlers
{
    class start_command{
        const TgBot::Bot& bot;
        std::shared_ptr<spdlog::logger>& logger;
    public:
        start_command(const TgBot::Bot& _bot, 
            std::shared_ptr<spdlog::logger>& l):
            bot(_bot), 
            logger(l)
            {}
        Message::Ptr operator()(const Message::Ptr&);
    };

    class help_command{
        const TgBot::Bot& bot;
        std::shared_ptr<spdlog::logger>& logger;
    public:
        help_command(const TgBot::Bot& _bot, 
            std::shared_ptr<spdlog::logger>& l):
            bot(_bot), 
            logger(l)
            {}
        Message::Ptr operator()(const Message::Ptr&);
    };
   
};

namespace handlers
{   
    class inform_handler
    {
        const TgBot::Bot& bot;
        std::shared_ptr<Storage<long, char>>& storage;
        std::shared_ptr<spdlog::logger>& logger;
    public:
        inform_handler(const TgBot::Bot& _bot, 
            std::shared_ptr<Storage<long, char>>& s,
            std::shared_ptr<spdlog::logger>& l):
            bot(_bot), 
            storage(s),
            logger(l)
            {}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class get_training
    {
        const TgBot::Bot& bot;
        std::shared_ptr<Storage<long, char>>& storage;
        std::shared_ptr<spdlog::logger>& logger;
    public:
        get_training(const TgBot::Bot& _bot,
            std::shared_ptr<Storage<long, char>>& s,
            std::shared_ptr<spdlog::logger>& l):
            bot(_bot), 
            storage(s),
            logger(l)
            {}
        Message::Ptr operator()(const Message::Ptr& message);
    };

    class prev_next_training
    {
        const TgBot::Bot& bot;
        std::shared_ptr<spdlog::logger>& logger;
    public:
        prev_next_training(const TgBot::Bot& _bot, 
            std::shared_ptr<spdlog::logger>& l):
            bot(_bot), 
            logger(l)
            {}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    }; 
};


#endif