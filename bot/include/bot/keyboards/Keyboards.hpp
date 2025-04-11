#ifndef KEYBOARDS_HPP
#define KEYBOARDS_HPP

#include <tgbot/tgbot.h>
#include <string>


using namespace TgBot;


namespace Keyboards
{
    InlineKeyboardMarkup::Ptr choose_program_kb(); 
    InlineKeyboardMarkup::Ptr navigation_kb(
        std::string&& prev, std::string&& next, std::string&& program);   
} 

#endif