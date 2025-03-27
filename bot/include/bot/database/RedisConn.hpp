#pragma once

#include <sw/redis++/redis++.h>
#include <exception>
#include <iostream>
#include <string>
#include <memory>

using namespace sw::redis;

class RedisConnection 
{
public:
    static RedisConnection& getInstance()
    {
        if (!instance)
            instance = new RedisConnection();
        return *instance;
    }
    std::string get(const std::string& user_id);
    void set(int user_id, const std::string& date);
    RedisConnection(const DBConnection&) = delete;
    RedisConnection& operator=(const RedisConnection&) = delete;
   
private:
    RedisConnection()
    {
        try
        {
            connection = Redis(uri);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void reconnect();
    ~RedisConnection()
    {
        delete instance;
    }
    
    static RedisConnection* instance;
    sw::redis::Redis connection = Redis("");

    const static std::string uri;
};

RedisConnection* RedisConnection::instance = nullptr;
const std::string RedisConnection::uri = std::getenv("REDIS") ? 
        std::getenv("REDIS") : "tcp://127.0.0.1:6379/2?keep_alive=true";