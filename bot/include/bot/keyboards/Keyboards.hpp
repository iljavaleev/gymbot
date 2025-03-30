#ifndef KEYBOARDS_HPP
#define KEYBOARDS_HPP

#include <tgbot/tgbot.h>
#include <string>


using namespace TgBot;


namespace Keyboards
{
    InlineKeyboardMarkup::Ptr navigation_kb(std::string&& prev, std::string&& next);   
} 

#endif