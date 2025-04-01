#include "bot/handlers/Handlers.hpp" 
#include "bot/database/DB.hpp"   
#include "bot/keyboards/Keyboards.hpp"  
#include "bot/utils/Utils.hpp"
                           
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
#include "spdlog/spdlog.h"


using namespace TgBot;


namespace command_handlers
{
    Message::Ptr start_command::operator()(const Message::Ptr& message)
    {
        try
        {
            return bot.getApi().sendMessage(
                message->chat->id, 
                "<i>Введите дату тренировки в формате дд.мм.гг. Например 20.03.20</i>",
                nullptr,
                nullptr,
                nullptr,
                "HTML");
        }
        catch(const std::exception& e)
        {
            spdlog::error("error: {}", e.what());
            
        }  
        return Message::Ptr(nullptr);
    }

    Message::Ptr help_command::operator()(const Message::Ptr& message)
    {
        try
        {
            return bot.getApi().sendMessage(
                message->chat->id, 
                "<i>Вы можете получить программу тренировки на указанную вами " 
                "дату из блокнота. Бот ожидает сообщение в формате дд.мм.гг. Например 20.03.20" 
                "Дата первой возможной теренировки - 30.09.19, последней - 21.05.21</i>",
                nullptr,
                nullptr,
                nullptr,
                "HTML");
        }
        catch(const std::exception& e)
        {
            spdlog::error("error: {}", e.what());
   
        }
        return Message::Ptr(nullptr);
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
            try
            {
                return bot.getApi().sendMessage(
                    message->chat->id, 
                    "<i>Некорректные данные. Попробуйте снова</i>",
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML");
            }
            catch(const std::exception& e)
            {
                spdlog::error("error: {}", e.what());
                return Message::Ptr(nullptr);
            }  
        }
        
        std::vector<std::string> training;
        try
        {
            auto& pg_connection = DBConnection::getInstance();
            auto fut_training = std::async(std::launch::async, 
                &DBConnection::get, std::ref(pg_connection), std::ref(date));
            training = fut_training.get();
        }
        catch(const std::exception& e)
        {
            spdlog::error("error: {}", e.what());
            try
            {
                return bot.getApi().sendMessage(
                    message->chat->id, 
                    "<i>Похоже, у нас небольшие неполадки. Попробуйте позже</i>",
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML"
                );
            }
            catch(const std::exception& e)
            {
                spdlog::error("error: {}", e.what());
            }  
            return Message::Ptr(nullptr);
        }  

        try
        {
            std::string mess = training.empty() ? \
            "<i>Неверно задана дата. Формат дд.мм.гг. Дата первой возможной теренировки 30.09.19, последней - 21.05.21</i>" :  training.at(0);
            
            return bot.getApi().sendMessage(message->chat->id, 
                std::move(mess), 
                nullptr, 
                nullptr, 
                training.empty() ? nullptr : Keyboards::navigation_kb(
                    std::move(training.at(1)),
                    std::move(training.at(2))
                ),
                "HTML"
            );
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        return Message::Ptr(nullptr); 
    }
 
    Message::Ptr prev_next_training::operator()(const CallbackQuery::Ptr& query)
    {
        std::vector<std::string> training;
        try
        {
            auto& pg_connection = DBConnection::getInstance();
            auto fut_training = std::async(std::launch::async, 
                &DBConnection::get, std::ref(pg_connection), std::ref(query->data));
            training = fut_training.get();
        }
        catch(const std::exception& e)
        {
            spdlog::error("error: {}", e.what());
            try
            {
                return bot.getApi().sendMessage(
                    query->message->chat->id, 
                    "<i>Похоже, у нас небольшие неполадки. Попробуйте позже</i>",
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML"
                );
            }
            catch(const std::exception& e)
            {
                spdlog::error("error: {}", e.what());
            }  
            return Message::Ptr(nullptr);
        }  
       

        try
        {
            std::string mess = training.empty() ? "<i>Неверно задана дата. Формат дд.мм.гг. Дата первой возможной теренировки 30.09.19, последней - 21.05.21</i>" :  training.at(0);
            
            return bot.getApi().sendMessage(query->message->chat->id, 
                std::move(mess), 
                nullptr, 
                nullptr, 
                training.empty() ? nullptr : Keyboards::navigation_kb(
                    std::move(training.at(1)),
                    std::move(training.at(2))
                ),
                "HTML"
            );
        }
        catch(const std::exception& e)
        {
            spdlog::error("error: {}", e.what());
        }

        return Message::Ptr(nullptr); 
    }

};

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl)
{
    try {
        spdlog::info("Bot username: {}", bot.getApi().getMe()->username);
        TgWebhookTcpServer webhookServer(8080, bot);
        spdlog::info("Server starting");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    } 
    catch (std::exception& e) 
    {
        spdlog::error("error: {}", e.what());
    }
}
