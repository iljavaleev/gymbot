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
           instance = new DBConnection();
        }
            
        return *instance;
    }
    template<typename T>
    std::vector<std::string> get(const std::string& id, const T& program)
    {
        std::string query;
        if (program == '0')
            query = str_query;
        else
            query = end_query;
        query += id;
        pqxx::result res;
      
        if (!connection->is_open())
            reconnect();

        pqxx::nontransaction non_trans(*connection);
        res = non_trans.exec(query);


        if (res.empty())
            throw std::runtime_error("empty result: " + query);

        std::stringstream result;

        int count = 1;
        int work_id = res.begin().at(2).as<int>();
        bool endu_program = (program == '1');

        result << "<u><b>" << "Тренировка №" << work_id 
        << " Неделя " << res.begin().at(3).as<int>() << "</b></u>" << "\n";

        for (auto i{res.begin()}; i != res.end(); ++i)
        {
            result << count++ << ". " << i.at(0).as<std::string>() << '\t' << 
                (i.at(1).is_null() ? "" : (
                    "<b>/" + i.at(1).as<std::string>() + "</b>"
                )); 
            
            if (endu_program && (!i.at(4).is_null()))
                result << " суперсет " <<  i.at(4).as<int>();       
            
            result << '\n';
        }

        bool start = (work_id == 1);
        
        bool end = ((work_id==263) && !endu_program) || 
            ((work_id==52) && endu_program);
        
        return {
            result.str(), 
            std::to_string(start ? 1 : work_id - 1),
            std::to_string(end ? work_id : work_id + 1)
        };
    }
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
    static constexpr char end_query[] = "select exercise, reps,\
    work_id, week_id, superset from endurance where work_id=";
    static constexpr char str_query[] = "select exercise, reps,\
    work_id, week_id from strenght where work_id=";
    
    
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
