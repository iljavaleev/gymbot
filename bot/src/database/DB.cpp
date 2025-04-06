#include <stdexcept>
#include <sstream>
#include <mutex>
#include <sstream>
#include <string>

#include "spdlog/spdlog.h"
#include "bot/database/DB.hpp"


using namespace TgBot;

extern const std::shared_ptr<spdlog::logger> logger;

std::vector<std::string> DBConnection::get(const std::string& date)
{
    logger->set_level(spdlog::level::err);
    std::string query = get_query + ("'" + date + "'");
    spdlog::info(query);
    pqxx::result res;
    try
    {
        if (!connection->is_open())
            reconnect();

        pqxx::nontransaction non_trans(*connection);
        res = non_trans.exec(query);
    }   
    catch(const std::exception& e)
    {
        logger->error("error: {}", e.what());
    }

    if (res.empty())  
    {
        return {};
    };
    std::stringstream result;

    int count = 1;
    result << "<u><b>" << res.begin().at(2).as<std::string>() << "</b></u>" << "\n";
    for (auto i{res.begin()}; i != res.end(); ++i)
    {
        result << count++ << ". " << i.at(0).as<std::string>() << '\t' << 
            (i.at(1).is_null() ? "" : ("<b>/" + i.at(1).as<std::string>() + "</b>")) << '\n';
    }
    return {
        result.str(), 
        res.begin().at(3).is_null() ? res.begin().at(5).as<std::string>() : res.begin().at(3).as<std::string>(),
        res.begin().at(4).is_null() ? res.begin().at(5).as<std::string>() : res.begin().at(4).as<std::string>()
    };
}   
void DBConnection::reconnect()
{  
    try
    {
        connection = std::make_shared<pqxx::connection>(uri);
    }
    catch(const pqxx::broken_connection & e){}

    if (!connection->is_open())
        throw std::runtime_error("DB reconnection error");
};
