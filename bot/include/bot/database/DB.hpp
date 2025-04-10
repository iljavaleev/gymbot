#ifndef DB_HPP
#define DB_HPP

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <tgbot/tgbot.h>


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
                throw;
            }
        }
            
        return *instance;
    }
    std::vector<std::string> get(const std::string&, const std::string&);
    DBConnection(const DBConnection&) = delete;
    DBConnection& operator=(const DBConnection&) = delete;
    
private:
    DBConnection()
    {
        connection = std::make_shared<pqxx::connection>(uri);
        if (!connection->is_open())
            throw std::runtime_error("error open db"); 
    }

    void reconnect();
    ~DBConnection()
    { 
        delete instance;
    }
    
    static DBConnection* instance;
    std::shared_ptr<pqxx::connection> connection{nullptr};
    
    const static std::string uri; 
    static constexpr char get_women_query[] = "select exercise, reps, TO_CHAR"
    "(date::DATE,'dd.mm.yyyy'), prev, next, date from womens_workout" 
    " where date=";
    static constexpr char get_men_query[] = "select exercise, reps, TO_CHAR"
    "(date::DATE,'dd.mm.yyyy'), prev, next, date from mens_workout where date=";
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
