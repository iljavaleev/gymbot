#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <tgbot/tgbot.h>

using namespace TgBot;

std::string parse_date(const std::string&);
std::vector<BotCommand::Ptr> create_commands();

#endif