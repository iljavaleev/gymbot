#ifndef Handlers_hpp
#define Handlers_hpp

#include <string>

#include "tgbot/types/CallbackQuery.h" 
#include "tgbot/types/Message.h"

namespace TgBot { class Bot; }

using namespace TgBot;


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

    class get_training
    {
        const TgBot::Bot& bot;
    public:
        get_training(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const Message::Ptr& message);
    };

    class next_training
    {
        const TgBot::Bot& bot;
    public:
        next_training(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };

    class prev_training
    {
        const TgBot::Bot& bot;
    public:
        prev_training(const TgBot::Bot& _bot):bot(_bot){}
        Message::Ptr operator()(const CallbackQuery::Ptr&);
    };
    
   
};

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl);

#endif