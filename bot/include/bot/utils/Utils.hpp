#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <tgbot/tgbot.h>
#include "spdlog/sinks/basic_file_sink.h"
#include <spdlog/spdlog.h>
#include "spdlog/async.h"

using namespace TgBot;


std::string parse_date(const std::string&);
std::vector<BotCommand::Ptr> create_commands();

inline const std::shared_ptr<spdlog::logger> logger = 
    spdlog::basic_logger_mt<spdlog::async_factory>(
        "async_file_logger", "../logs/logs.txt");

#endif