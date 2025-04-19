#include "bot/utils/Utils.hpp"

#include <regex>                        
#include <vector>
#include <sstream>

#include "tgbot/Api.h"                       
#include "tgbot/Bot.h"


using namespace TgBot;


bool is_valid_date(const std::string& date, unsigned char program)
{
    
    int int_date = std::stoi(date);
    try
    {
        if (int_date < 1)
            return 0;
        if ((program == '0') && (int_date>263))
            return 0;
        if ((program == '1') && (int_date>52))
            return 0;
    }
    catch(const std::exception& e)
    {
        return 0;
    }
    return 1;
}

std::vector<BotCommand::Ptr> create_commands()
{
    std::vector<BotCommand::Ptr> commands;

    BotCommand::Ptr start_command{new BotCommand};
    start_command->command = "start";
    start_command->description = "запуск бота/выбор программы";
    commands.push_back(start_command);

    BotCommand::Ptr cancel_command{new BotCommand};
    cancel_command->command = "help";
    cancel_command->description = "справка";
    commands.push_back(cancel_command);

    return commands;
}