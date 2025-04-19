#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <tgbot/tgbot.h>
#include <string_view>

#include "spdlog/sinks/basic_file_sink.h"


using namespace TgBot;


bool is_valid_date(const std::string&, char program);
std::vector<BotCommand::Ptr> create_commands();

#endif