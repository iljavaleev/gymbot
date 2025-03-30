#include "bot/keyboards/Keyboards.hpp"

#include <tgbot/tgbot.h>
#include <vector>
#include <utility>

#include <unordered_set>


using namespace TgBot;


namespace Keyboards
{
    InlineKeyboardMarkup::Ptr navigation_kb(std::string&& prev, std::string&& next) 
    { 
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr next_btn(new InlineKeyboardButton);
        next_btn->text = "Предыдущая";
        next_btn->callbackData = std::move(prev);
        row.push_back(next_btn);

        InlineKeyboardButton::Ptr prev_btn(new InlineKeyboardButton);
        prev_btn->text = "Следующая";
        prev_btn->callbackData = std::move(next);
        row.push_back(prev_btn);

        keyboard->inlineKeyboard.push_back(row); 

        return keyboard;
    } 

}
