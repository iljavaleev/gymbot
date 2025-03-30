
#include <tgbot/tgbot.h>
#include <sw/redis++/redis++.h>
#include <exception>
#include "bot/handlers/Handlers.hpp"
#include "bot/database/DB.hpp"

using namespace TgBot;
using namespace sw::redis; 

void startWebhook(TgBot::Bot& bot, std::string& webhookUrl);

int main()
{
    std::string bot_token = std::string(std::getenv("BOT_TOKEN"));
    // std::string base_webhook_url = std::string(std::getenv("BASE_WEBHOOK_URL"));
    // std::string TEL = "https://api.telegram.org/bot" + bot_token + "/setWebhook?url=";
    // std::string WEBHOOK_URL = base_webhook_url + "/" + bot_token;    
    TgBot::Bot bot(bot_token);
    bot.getEvents().onCommand("start", command_handlers::start_command(bot));
    bot.getEvents().onCommand("help", command_handlers::help_command(bot));
    bot.getEvents().onAnyMessage(handlers::get_training(bot));
    bot.getEvents().onCallbackQuery(handlers::prev_next_training(bot));
    
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

   

    return 0;
}

void startWebhook(TgBot::Bot& bot, const std::string& webhookUrl)
{
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());

        TgWebhookTcpServer webhookServer(8080, bot);

        printf("Server starting\n");
        bot.getApi().setWebhook(webhookUrl);
        webhookServer.start();
    } 
    catch (std::exception& e) 
    {
        printf("error: %s\n", e.what());
    }
}