
#include <tgbot/tgbot.h>
#include <exception>

#include "bot/utils/Utils.hpp"
#include "bot/handlers/Handlers.hpp"
#include "bot/database/UserStorage.hpp"
#include "bot/database/DB.hpp"

#include <spdlog/spdlog.h>
#include "spdlog/async.h"

using namespace TgBot;


int main()
{
    std::string bot_token = std::string(std::getenv("BOT_TOKEN"));
    std::shared_ptr<spdlog::logger> logger = 
    spdlog::basic_logger_mt<spdlog::async_factory>(
        "handler_logger", "../logs/handler_logs.txt");

    spdlog::set_default_logger(logger);
    logger->set_level(spdlog::level::err);
    spdlog::flush_on(spdlog::level::err);
    // std::string base_webhook_url = std::string(std::getenv("BASE_WEBHOOK_URL"));
    // std::string TEL = "https://api.telegram.org/bot" + bot_token + "/setWebhook?url=";
    // std::string WEBHOOK_URL = base_webhook_url + "/" + bot_token;    
    TgBot::Bot bot(bot_token);
    auto storage = std::make_shared<Storage<long, char>>();
   

    bot.getApi().deleteWebhook();
    
    std::vector<BotCommand::Ptr> commands = create_commands();
    bot.getApi().setMyCommands(commands);

    bot.getEvents().onCommand("start", command_handlers::start_command(bot, logger));
    bot.getEvents().onCommand("help", command_handlers::help_command(bot, logger));
    
    bot.getEvents().onAnyMessage(handlers::get_training(bot, storage, logger));
    bot.getEvents().onCallbackQuery(handlers::prev_next_training(bot, logger));
    bot.getEvents().onCallbackQuery(handlers::inform_handler(bot, storage, logger));
    
    
    try 
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) 
        {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    signal(SIGINT, [](int s) { printf("SIGINT got\n"); exit(0);});
    
    // try 
    // {
    //     TgWebhookTcpServer webhookServer(8080, bot);
    //     bot.getApi().setWebhook(WEBHOOK_URL);
    //     webhookServer.start();
    // } 
    // catch (std::exception& e) 
    // {
    //     logger->error("error: {}", e.what());
    // }
    return 0;
}
