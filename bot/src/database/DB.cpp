#include <stdexcept>
#include <sstream>
#include <mutex>
#include <sstream>

#include "bot/database/DB.hpp"

using namespace TgBot;

std::string DBConnection::get(const std::string& date)
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
    for (auto i{res.begin()}; i != res.end(); ++i)
    {

        result << i.at(0).as<std::string>() << '\t' << 
            i.at(1).as<std::string>() << '\t' 
            << i.at(2).as<std::string>() << '\n';
    }
    return result.str();
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
