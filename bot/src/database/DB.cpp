#include <stdexcept>


#include "bot/database/DB.hpp"


using namespace TgBot;


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
