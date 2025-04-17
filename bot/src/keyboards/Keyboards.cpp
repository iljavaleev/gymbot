#include "bot/keyboards/Keyboards.hpp"
#include "bot/utils/Utils.hpp"

#include <tgbot/tgbot.h>
#include <vector>
#include <utility>
#include <unordered_set>

using namespace TgBot;


namespace Keyboards
{
    InlineKeyboardMarkup::Ptr navigation_kb(std::string&& prev, 
        std::string&& next, std::string&& program) 
    { 
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr prev_btn(new InlineKeyboardButton);
        prev_btn->text = "Предыдущая";
        prev_btn->callbackData = std::move(prev) + "," + std::move(program);
        row.push_back(prev_btn);

        InlineKeyboardButton::Ptr next_btn(new InlineKeyboardButton);
        next_btn->text = "Следующая";
        next_btn->callbackData = std::move(next) + "," + std::move(program);
        row.push_back(next_btn);

        keyboard->inlineKeyboard.push_back(row); 

        return keyboard;
    }
    
    InlineKeyboardMarkup::Ptr choose_program_kb()
    {
        InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
        std::vector<InlineKeyboardButton::Ptr> row;

        InlineKeyboardButton::Ptr next_btn(new InlineKeyboardButton);
        next_btn->text = "Коричневый";
        next_btn->callbackData = "S";
        row.push_back(next_btn);

        InlineKeyboardButton::Ptr prev_btn(new InlineKeyboardButton);
        prev_btn->text = "Синий";
        prev_btn->callbackData = "E";
        row.push_back(prev_btn);
        keyboard->inlineKeyboard.push_back(row); 

        return keyboard;
    }

}
