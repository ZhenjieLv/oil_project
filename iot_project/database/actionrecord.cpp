#include "actionrecord.h"
#include <cstdio>
#include <cstring>
#include "database.h"
using namespace std;
#define LOG_FILE_MSG "[database actionrecord.cpp]: "

int insert_actionrecord_info(uint16_t device_id,string action_record_msg)
{
    Database db;
    int record_id;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return -1;
    char query[300] = {0};
    sprintf(query, "insert into ActionRecord(DeviceID, Package) values(%u, '%s')", device_id, action_record_msg);
    char id_query[50] = "select SCOPE_IDENTITY() as RecordID";
    MYSQL_RES *result = db.sql_search(id_query,strlen(id_query));
    if(result == nullptr)
    {
      mysql_free_result(result);
      db.close();
      return -1;
    }
    MYSQL_ROW row;
    while(row = mysql_fetch_row(result))
    {
       record_id = char_to_uint32(row[0]);
    }
    mysql_free_result(result);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向ActionRecord表中插入数据失败！",LOG_FILE_MSG);
      return -1;
    }
    db.close();
    return record_id;
}
bool update_actionrecord_info_succeed(uint32_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[800] = {0};
    sprintf(query,"update ActionRecord set IsSucceed = true where RecordID = %u", id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}向ActionRecord表中插入数据失败！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}
/**
 * @brief 查询表中未上传的数据
 * 
 * @return std::vector<actionrecord_t> 
 */
std::vector<actionrecord_t> search_actionrecord_not_upload_info()
{
    std::vector<actionrecord_t> ret;
    Database db;
    if(!db.connect()) return ret;
    char query[50] = "select * from ActionRecord where isSync = false;";
    MYSQL_RES *result = db.sql_search(query, strlen(query));
    if(result == nullptr) { mysql_free_result(result);
    db.close(); return ret;}
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        actionrecord_t temp = {0};
        temp.id = char_to_uint32(row[0]);
        temp.timestamp = row[1];
        temp.device_id = char_to_uint16(row[2]);
        temp.issucceed = row[3];
        memcpy(temp.package,row[4],strlen(row[4]));
    }
    mysql_free_result(result);
    db.close();
    return ret;
}

bool update_actionrecord_info_upload(uint32_t id)
{
    Database db;
    auto logger = spdlog::get("project_log");
    if(!db.connect()) return false;
    char query[500] = {0};
    sprintf(query,"update ActionRecord set isSync = true where RecordID = %u", id);
    if(!db.sql_query(query, strlen(query))) 
    {
      db.close();
      logger->error("{}将ActionRecord表中isSync更新为true操作失败！但该数据已经成功发送至云端！",LOG_FILE_MSG);
      return false;
    }
    db.close();
    return true;
}

bool delete_actionrecord_info(const char* end_time)
{
    Database db;
    if(!db.connect()) return false;
    char query[200] = {0};
    sprintf(query,"delete from ActionRecord where Timestamp<='%s'", end_time);
    if(!db.sql_query(query, strlen(query))) return false;
    db.close();
    return true;
}