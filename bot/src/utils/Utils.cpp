#include "bot/utils/Utils.hpp"

#include <regex>                        
#include <vector>
#include <sstream>

#include "tgbot/Api.h"                       
#include "tgbot/Bot.h"

using namespace TgBot;

std::string parse_date(const std::string& date)
{
    std::regex self_regex("^[0-9.]{8}$",
        std::regex_constants::ECMAScript | std::regex_constants::icase);
    if (!std::regex_search(date, self_regex))
        return "";
    std::vector<std::string> dmy;
    std::stringstream ss (date);
    std::string item;

    while (getline (ss, item, '.')) 
    {
        dmy.push_back (item);
    }

    if (std::stoi(dmy.at(0)) < 1 && std::stoi(dmy.at(0)) > 31)
        return "";
    if (std::stoi(dmy.at(1)) < 1 && std::stoi(dmy.at(1)) > 12)
        return "";
    if (std::stoi(dmy.at(2)) < 20 && std::stoi(dmy.at(2)) > 40)
        return "";

    std::stringstream res;
    res << dmy.at(2) << "-" << dmy.at(1) << "-" << dmy.at(0);
    return res.str();
}