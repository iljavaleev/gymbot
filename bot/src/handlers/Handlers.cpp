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
#include <string_view>
#include <regex>

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
        if (!(query->data == "0" || query->data == "1"))
        {
            return Message::Ptr(nullptr);
        }
            
        try
        {
            storage->set(query->message->chat->id, *query->data.c_str());
            return bot.getApi().sendMessage(
                query->message->chat->id, 
                std::string(choose_work_string),
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
        if ((message->text == "/start" || message->text == "/help"))
            return Message::Ptr(nullptr);
        if (!std::regex_search(message->text, 
            std::regex("^[0-9]+$", std::regex_constants::ECMAScript))) 
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
        
        auto program = storage->get(message->chat->id);
        if (!program)
        {
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
                return Message::Ptr(nullptr);
            }
            
        }
        
        if (!is_valid_date(message->text, *program))
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
        try
        {
            auto& pg_connection = DBConnection::getInstance();
            auto fut_training = std::async(std::launch::async, 
                &DBConnection::get<char>, std::ref(pg_connection), 
                std::ref(message->text), 
                program.value());
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
            std::string(error_work_string) : training.at(0);
            
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
        
        std::vector<std::string> training;
        try
        {
            auto& pg_connection = DBConnection::getInstance();
            auto fut_training = std::async(std::launch::async, 
                &DBConnection::get<char>, std::ref(pg_connection), 
                data.at(0), *data.at(1).c_str());
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
            std::string(error_work_string) : training.at(0);
            
            char program = *data.at(1).c_str();
            return bot.getApi().sendMessage(query->message->chat->id, 
                std::move(mess), 
                nullptr, 
                nullptr, 
                training.empty() ? nullptr : Keyboards::navigation_kb(
                    std::move(training.at(1)),
                    std::move(training.at(2)),
                    std::move(program)
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
