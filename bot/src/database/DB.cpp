#include <stdexcept>
#include <sstream>
#include <mutex>
#include <sstream>
#include <string>

#include "spdlog/spdlog.h"
#include "bot/database/DB.hpp"


using namespace TgBot;

extern const std::shared_ptr<spdlog::logger> logger;

std::vector<std::string> DBConnection::get(const std::string& id, 
    const std::string& program)
{
    std::string query;
    if (program == "S")
        query = str_query;
    else
        query = end_query;
    query += id;

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
        logger->error("error: {}", e.what());
    }

    if (res.empty())  return {};

    std::stringstream result;

    int count = 1;
    int work_id = res.begin().at(2).as<int>();
    bool endu_program = (program == "E");

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
