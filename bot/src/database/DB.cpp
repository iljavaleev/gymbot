#include <stdexcept>
#include <sstream>
#include <mutex>
#include <sstream>
#include <string>

#include "spdlog/spdlog.h"
#include "bot/database/DB.hpp"

using namespace TgBot;

std::vector<std::string> DBConnection::get(const std::string& date)
{
    
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
        spdlog::error(e.what());
    }

    if (res.empty())  
    {
        return {};
    };
    std::stringstream result;

    int count = 1;
    result << "<b>" << res.begin().at(2).as<std::string>() << "</b>" << "\n";
    for (auto i{res.begin()}; i != res.end(); ++i)
    {
        result << count++ << ". " << i.at(0).as<std::string>() << '\t' << 
            (i.at(1).is_null() ? "" : ("<b>/" + i.at(1).as<std::string>() + "</b>")) << '\n';
    }
    return {
        result.str(), 
        res.at(3).is_null() ? "" ? res.begin().at(3).as<std::string>(),
        res.at(4).is_null() ? "" ? res.begin().at(4).as<std::string>()
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
