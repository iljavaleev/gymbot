#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include "bot/database/UserStorage.hpp" 

#include <string>

#include "tgbot/types/CallbackQuery.h" 
#include "tgbot/types/Message.h"
#include <tgbot/tgbot.h>


using namespace TgBot;

inline auto storage = std::make_shared<Storage<long, std::string>>();

namespace command_handlers
{
    class start_command{
        const TgBot::Bot& bot;
    public:
        start_command(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };

    class help_command{
        const TgBot::Bot& bot;
    public:
        help_command(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr&);
    };
   
};

namespace handlers
{   
    class inform_handler
    {
        const TgBot::Bot& bot;
    public:
        inform_handler(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class get_training
    {
        const TgBot::Bot& bot;
    public:
        get_training(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr& message);
    };

    class prev_next_training
    {
        const TgBot::Bot& bot;
    public:
        prev_next_training(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    }; 
};

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl);

#endif