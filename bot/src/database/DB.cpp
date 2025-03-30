#include <stdexcept>
#include <sstream>
#include <mutex>
#include <sstream>
#include <string>

#include "bot/database/DB.hpp"

using namespace TgBot;

std::vector<std::string> DBConnection::get(const std::string& date)
{
    std::string query = get_query + date;
    pqxx::result res;
    try
    {
        if (!connection.is_open())
            reconnect();

        pqxx::nontransaction non_trans(connection);
        res = non_trans.exec(query);
    }   
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    if (res.empty()) return {};
    std::stringstream result;

    int count = 1;
    result << res.begin().at(2).as<std::string>() << "\n";
    for (auto i{res.begin()}; i != res.end(); ++i)
    {
        result << count++ << ". " << i.at(0).as<std::string>() << '\t' << 
            i.at(1).as<int>() << '\n';
    }
    return {result.str(), res.begin().at(3).as<std::string>(), res.begin().at(4).as<std::string>()};
}   

void DBConnection::reconnect()
{  
    try
    {
        connection = pqxx::connection(uri);
    }
    catch(const pqxx::broken_connection & e){}

    if (!connection.is_open())
        throw std::runtime_error("DB reconnection error");
};
