#include "bot/database/RedisConn.hpp"
#include <optional>


std::string RedisConnection::get(const std::string& user_id)
{
    if (!connection.ping())
        reconnect();
    if (!connection.ping())
        throw std::runtime_error("Redis connection failed");
        
    Optional<std::string> val = connection.get(user_id);
    return val.value("");
}

void RedisConnection::set(int user_id, 
    const std::string& date)
{
    if (!connection.ping()) reconnect();
    if (!connection.ping())
        throw std::runtime_error("Redis connection failed");
    redis.set(user_id, date);
}
   
void RedisConnection::reconnect()
{
    connection = Redis(uri);
}
