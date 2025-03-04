#ifndef _DATABASE_H
#define _DATABASE_H
#include <mysql/mysql.h>
#include <spdlog/spdlog.h>
#include <cstdint>

class Database
{
private:
    MYSQL mysql;
    std::shared_ptr<spdlog::logger> logger;
public:
    Database(/* args */);
    ~Database();
    bool connect();
    void close();
    bool sql_query(const char *query, size_t len);
    MYSQL_RES *sql_search(const char *query, size_t len);
    int check_search_not_info(const char *query, size_t len);
};

uint16_t char_to_uint16(const char* str);
uint32_t char_to_uint32(const char* str);
#endif
