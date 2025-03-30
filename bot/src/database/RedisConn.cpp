#include "bot/database/RedisConn.hpp"
#include <optional>


std::string RedisConnection::get(const std::string& user_id)
{
    if (connection.ping() != "PONG")
        reconnect();
    if (connection.ping() != "PONG")
        throw std::runtime_error("Redis connection failed");
        
    Optional<std::string> val = connection.get(user_id);
    
    return val.value_or("");
}

void RedisConnection::set(const std::string& user_id, 
    const std::string& date)
{
    if (connection.ping() != "PONG") reconnect();
    if (connection.ping() != "PONG")
        throw std::runtime_error("Redis connection failed");
        connection.set(user_id, date);
}
   
void RedisConnection::reconnect()
{
    connection = Redis(uri);
}
