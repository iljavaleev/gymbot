
#include <tgbot/tgbot.h>
#include <exception>

#include "bot/utils/Utils.hpp"
#include "bot/handlers/Handlers.hpp"
#include "bot/database/DB.hpp"


using namespace TgBot;
extern const std::shared_ptr<spdlog::logger> logger;

int main()
{
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::err);

    std::string bot_token = std::string(std::getenv("BOT_TOKEN"));
    // std::string base_webhook_url = std::string(std::getenv("BASE_WEBHOOK_URL"));
    // std::string TEL = "https://api.telegram.org/bot" + bot_token + "/setWebhook?url=";
    // std::string WEBHOOK_URL = base_webhook_url + "/" + bot_token;    
    TgBot::Bot bot(bot_token);
    bot.getApi().deleteWebhook();
    
    std::vector<BotCommand::Ptr> commands = create_commands();
    bot.getApi().setMyCommands(commands);

    bot.getEvents().onCommand("start", command_handlers::start_command(bot));
    bot.getEvents().onCommand("help", command_handlers::help_command(bot));
    
    bot.getEvents().onAnyMessage(handlers::get_training(bot));
    bot.getEvents().onCallbackQuery(handlers::prev_next_training(bot));
    bot.getEvents().onCallbackQuery(handlers::inform_handler(bot));
    
    
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
    // startWebhook(bot, WEBHOOK_URL);
    return 0;
}
