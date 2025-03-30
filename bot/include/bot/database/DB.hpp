#ifndef DB_HPP
#define DB_HPP

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <tgbot/tgbot.h>
#include "spdlog/spdlog.h"

class DBConnection 
{
public:
    static DBConnection& getInstance()
    {
        if (!instance)
        {
            try
            {
                instance = new DBConnection();
            }
            catch(const std::exception& e)
            {
                spdlog::error("error: {}", e.what());
                throw;
            }
        }
            
        return *instance;
    }
    std::vector<std::string> get(const std::string&);
    DBConnection(const DBConnection&) = delete;
    DBConnection& operator=(const DBConnection&) = delete;
    
private:
    DBConnection()
    {
        connection = pqxx::connection(uri);
        if (!connection.is_open())
            throw std::runtime_error("error open db"); 
    }

    void reconnect();
    ~DBConnection()
    { 
        connection.close(); 
        delete instance;
    }
    
    static DBConnection* instance;
    pqxx::connection connection;
    
    const static std::string uri; 

    static constexpr char get_query[] = "select exercise, reps, TO_CHAR(date::DATE,'dd/mm/yyyy'), prev, next from workout where date=";
};

inline DBConnection* DBConnection::instance = nullptr;

inline const std::string DBConnection::uri =  
    std::string("dbname=") + (std::getenv("POSTGRES_DB") ? 
        std::getenv("POSTGRES_DB") : "postgres") +
    std::string(" user=") + (std::getenv("POSTGRES_USER") ? 
        std::getenv("POSTGRES_USER") : "postgres") +
    std::string(" password=") + (std::getenv("POSTGRES_PASSWORD") ? 
        std::getenv("POSTGRES_PASSWORD") : "postgres") + 
    std::string(" host=") + (std::getenv("POSTGRES_HOST") ? 
        std::getenv("POSTGRES_HOST") : "localhost") +
    std::string(" port=") +  (std::getenv("POSTGRES_PORT") ? 
        std::getenv("POSTGRES_PORT") : "5432");

#endif 
