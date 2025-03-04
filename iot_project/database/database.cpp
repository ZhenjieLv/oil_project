#include <cstring>
#include "database.h"


// TODO 检查数据库连接信息，错误的地方更改
#define DB_HOST "localhost"
#define DB_USER "nvidia"
#define DB_PASSWORD "nvidia"
#define DB_NAME "oil_edge"
#define DB_PORT 3306

#define LOG_FILE_MSG "[database database.cpp]: "

Database::Database(/* args */)
{
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(&mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8");
    logger = spdlog::get("project_log");
}

Database::~Database()
{

}

bool Database::connect()
{
    if(!mysql_real_connect(&mysql, DB_HOST, DB_USER, DB_PASSWORD, DB_NAME, DB_PORT, NULL, 0)) {
        logger->error("{}连接数据库失败，错误号：{}，错误：{}",LOG_FILE_MSG, mysql_error(&mysql), mysql_errno(&mysql));
        return false;
    }
    return true;
}

void Database::close()
{
    mysql_close(&mysql);
}

bool Database::sql_query(const char *query, size_t len)
{
    return mysql_real_query(&mysql, query, len)==0;
}

MYSQL_RES* Database::sql_search(const char *query, size_t len)
{
    mysql_real_query(&mysql, query, len);
    //if(mysql_real_query(&mysql, query, len)!=0)
       // return nullptr;
    return mysql_store_result(&mysql);
}

/**
 * @brief 将数据库中查询到的字符数字转换为uint16表示
 * 
 * @param str 需要转换的字符
 * @return uint16_t 转换后的数据
 */
uint16_t char_to_uint16(const char* str)
{
    uint16_t ans = 0;
    size_t len = strlen(str);
    for(size_t i=0; i<len; i++) {
        ans *= 10;
        ans += str[i]-'0';
    }
    return ans;
}

/**
 * @brief 将数据库中查询到的字符数字转换为uint32表示
 * 
 * @param str 需要转换的字符
 * @return uint32_t 转换后的数据
 */
uint32_t char_to_uint32(const char* str)
{
    uint32_t ans = 0;
    size_t len = strlen(str);
    for(size_t i=0; i<len; i++) {
        ans *= 10;
        ans += str[i]-'0';
    }
    return ans;
}

int Database::check_search_not_info(const char *query, size_t len)
{
    if(mysql_real_query(&mysql, query, len)) return -1;
    MYSQL_RES *res;
    res = mysql_store_result(&mysql);
    if(res == NULL )   return -1;
    int rows = mysql_num_rows(res);
    if(rows == 0){mysql_free_result(res);return -1;}
}
