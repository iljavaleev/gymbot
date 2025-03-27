#include "bot/handlers/Handlers.hpp" 
#include "bot/database/DB.hpp"   
#include "bot/database/RedisConn.hpp" 
#include "bot/keyboards/Keyboards.hpp"  
#include "bot/utils/Utils.hpp"                        
#include <functional>                              
#include <memory>                                   
#include <string>                                                                     
#include <vector> 
#include <future> 
#include <exception>      
                          

#include "tgbot/Api.h"                              
#include "tgbot/Bot.h"                              
#include "tgbot/net/TgWebhookTcpServer.h"           
#include "tgbot/tools/StringTools.h"                
#include "tgbot/types/Chat.h"                       
#include "tgbot/types/InlineKeyboardMarkup.h"       
#include "tgbot/types/User.h"                       

using namespace TgBot;


namespace command_handlers
{
    Message::Ptr start_command::operator()(const Message::Ptr& message)
    {
        return bot.getApi().sendMessage(message->chat->id, "Введите дату \
            тренировки в формате дд.мм.гг. Например 09.03.21");
    }

    Message::Ptr help_command::operator()(const Message::Ptr& message)
    {
        return bot.getApi().sendMessage(message->chat->id, "Вы можете получить программу тренировки на указанную вами \дату. Бот ожидает сообщение в формате дд.мм.гг. Например 09.03.21. Дату нужно взять из коричневого блокнота. \Если вы недавно обращались к боту, то возможно он запомнил последнюю дату вашего поиска и сможет найти данные \на предыдущий или следующий тренировочный день. Но память у него короткая. Нажимайте на кнопки след/пред");
    }
};


namespace handlers
{
    Message::Ptr get_training::operator()(const Message::Ptr& message)
    {
        if (StringTools::startsWith(message->text, "/start") || 
            StringTools::startsWith(message->text, "/help")) 
        {
            return Message::Ptr(nullptr);
        }
        std::string date = parse_date(message->text);
        if (date.empty())
        {
            return bot.getApi().sendMessage(message->chat->id, "Некорректные данные. Попробуйте снова");
        }
        auto& redis_connection = RedisConnection::getInstance();
        auto set_date = std::async(std::launch::deferred, 
           &RedisConnection::set, std::ref(redis_connection), message->chat->id, date); 

        auto& pg_connection = DBConnection::getInstance();
        auto fut_training = std::async(std::launch::async, 
           &DBConnection::get, std::ref(pg_connection), std::ref(date));
        
        try
        {
            std::string training = fut_training.get();
            return bot.getApi().sendMessage(message->chat->id, 
                std::move(training), nullptr, nullptr, Keyboards::navigation_kb());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        set_date.wait();
        return Message::Ptr(nullptr); 
    }
 
    Message::Ptr next_training::operator()(const CallbackQuery::Ptr& query)
    {
        
        return Message::Ptr(nullptr); 
    }

    Message::Ptr prev_training::operator()(const CallbackQuery::Ptr& query)
    {
        
        return Message::Ptr(nullptr); 
    };

};

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl)
{
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgWebhookTcpServer webhookServer(8080, bot);
        printf("Server starting\n");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    } 
    catch (std::exception& e) 
    {
        printf("error: %s\n", e.what());
    }
}
