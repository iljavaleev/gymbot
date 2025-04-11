#include "bot/handlers/Handlers.hpp" 
#include "bot/database/DB.hpp"  
#include "bot/keyboards/Keyboards.hpp"  
#include "bot/utils/Utils.hpp"
#include "bot/Vocabular.hpp"
                           
#include <memory>                                   
#include <string>                                                                     
#include <vector> 
#include <future> 
#include <exception>      
                            
#include "spdlog/spdlog.h"

using namespace TgBot;
extern const std::shared_ptr<spdlog::logger> logger;


namespace command_handlers
{
    Message::Ptr start_command::operator()(const Message::Ptr& message)
    {
        logger->set_level(spdlog::level::err);
        try
        {
            return bot.getApi().sendMessage(
                message->chat->id, 
                std::string(choose_program_string),
                nullptr,
                nullptr,
                Keyboards::choose_program_kb(),
                "HTML");
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
            
        }  
        return Message::Ptr(nullptr);
    }


    Message::Ptr help_command::operator()(const Message::Ptr& message)
    {
        logger->set_level(spdlog::level::err);
        try
        {
            return bot.getApi().sendMessage(
                message->chat->id, 
                std::string(help_string),
                nullptr,
                nullptr,
                nullptr,
                "HTML");
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
   
        }
        return Message::Ptr(nullptr);
    }
};


namespace handlers
{
    Message::Ptr inform_handler::operator()(const CallbackQuery::Ptr& query)
    {
        logger->set_level(spdlog::level::err);
        if (!(query->data == "M" || query->data == "W"))
        {
            return Message::Ptr(nullptr);
        }
            
        try
        {
            storage->set(query->message->chat->id, 
                std::move(query->data));
            return bot.getApi().sendMessage(
                query->message->chat->id, 
                std::string(choose_date_string),
                nullptr,
                nullptr,
                nullptr,
                "HTML");
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
            
        }  
        return Message::Ptr(nullptr);
    }


    Message::Ptr get_training::operator()(const Message::Ptr& message)
    {
        logger->set_level(spdlog::level::err);
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
                    std::string(error_string),
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML");
            }
            catch(const std::exception& e)
            {
                logger->error("error: {}", e.what());
                return Message::Ptr(nullptr);
            }  
        }
        
        std::vector<std::string> training;
        auto program = storage->get(message->chat->id);
        try
        {
            if (!program)
            {
                return bot.getApi().sendMessage(
                    message->chat->id, 
                    std::string(choose_program_string),
                    nullptr,
                    nullptr,
                    Keyboards::choose_program_kb(),
                    "HTML");
            }
            auto& pg_connection = DBConnection::getInstance();
            auto fut_training = std::async(std::launch::async, 
                &DBConnection::get, std::ref(pg_connection), 
                std::ref(date), 
                *program);
            training = fut_training.get();
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
            try
            {
                return bot.getApi().sendMessage(
                    message->chat->id, 
                    std::string(problems_string),
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML"
                );
            }
            catch(const std::exception& e)
            {
                logger->error("error: {}", e.what());
            }  
            return Message::Ptr(nullptr);
        }  

        try
        {
            std::string mess = training.empty() ? \
            std::string(error_date_string) : training.at(0);
            
            return bot.getApi().sendMessage(message->chat->id, 
                std::move(mess), 
                nullptr, 
                nullptr, 
                training.empty() ? nullptr : Keyboards::navigation_kb(
                    std::move(training.at(1)),
                    std::move(training.at(2)),
                    std::move(*program)
                ),
                "HTML"
            );
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
        }

        return Message::Ptr(nullptr); 
    }
 
    Message::Ptr prev_next_training::operator()(const CallbackQuery::Ptr& query)
    {
        auto data = StringTools::split(query->data, ',');
        if (data.size() != 2)
        {
            return Message::Ptr(nullptr);
        }
        logger->set_level(spdlog::level::err);
        std::vector<std::string> training;
        try
        {
            auto& pg_connection = DBConnection::getInstance();
            auto fut_training = std::async(std::launch::async, 
                &DBConnection::get, std::ref(pg_connection), 
                data.at(0), data.at(1));
            training = fut_training.get();
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
            try
            {
                return bot.getApi().sendMessage(
                    query->message->chat->id, 
                    std::string(problems_string),
                    nullptr,
                    nullptr,
                    nullptr,
                    "HTML"
                );
            }
            catch(const std::exception& e)
            {
                logger->error("error: {}", e.what());
            }  
            return Message::Ptr(nullptr);
        }  
       

        try
        {
            std::string mess = training.empty() ? \
            std::string(error_date_string) : training.at(0);
            
            return bot.getApi().sendMessage(query->message->chat->id, 
                std::move(mess), 
                nullptr, 
                nullptr, 
                training.empty() ? nullptr : Keyboards::navigation_kb(
                    std::move(training.at(1)),
                    std::move(training.at(2)),
                    std::move(data.at(1))
                ),
                "HTML"
            );
        }
        catch(const std::exception& e)
        {
            logger->error("error: {}", e.what());
        }

        return Message::Ptr(nullptr); 
    }

};

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl)
{
    logger->set_level(spdlog::level::err);
    try {
        spdlog::info("Bot username: {}", bot.getApi().getMe()->username);
        TgWebhookTcpServer webhookServer(8080, bot);
        spdlog::info("Server starting");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    } 
    catch (std::exception& e) 
    {
        logger->error("error: {}", e.what());
    }
}
