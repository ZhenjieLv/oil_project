#include <regex>
#include <string>
#include "util.h"
#include "device_status.h"
#include "error_no.h"

using namespace std;

/**
 * @brief 获取目前板子的读写速率
 * 
 * @param info 将数据存入此JSON变量
 */
int get_IO_speed_now(Json::Value *info)
{
    FILE * fp;
    char buffer[100], *ptr;
    string data_str;
    float data[3]={0};
    regex reg("-?(([1-9]\\d*\\.\\d*)|(0\\.\\d*[1-9]\\d*))");
    // 执行命令
    if((fp = popen("iostat -m","r")) == NULL)
        return NO_SUCH_FILE;

    while (fgets(buffer,sizeof(buffer),fp) != NULL)
    {
        ptr = strstr(buffer, "Device");
        if(ptr != NULL) {
            while (fgets(buffer,sizeof(buffer),fp) != NULL)
            {
                data_str = buffer;
                const sregex_iterator iter_end;
                int i = 0;
                for (sregex_iterator iter(data_str.begin(), data_str.end(), reg); iter != iter_end;++iter, i++) {
                    data[i] += atof(iter->str().c_str());
                }
            }
            break;
        }
    }
    pclose(fp);

    (*info)["readFrequency"] = data[1];
    (*info)["writeFrequency"] = data[2];
    return EXC_SUCCESS;
}

/**
 * @brief 获取目前板子的CPU利用率等
 * @details 此函数获取CPU利用率、单核CPU利用率、剩余硬盘数据、内存数据、剩余内存数据。此函数依赖Jetson NX自带的jtop工具，主要是通过调用Python3.6执行scripts文件夹下的calculation.py来获取信息。
 * @param info 将数据存入此JSON变量
 */
int get_jtop_info(Json::Value *info)
{
    auto logger = spdlog::get("project_log");
    
    int ret = system("python3 ./scripts/calculation.py");
    if(ret != 0) {
        logger->error("系统执行calculation.py出错，返回码{}", ret);
        return EXECUT_SCRIPT_ERROR;
    }
    
    Json::Value tmp = read_json_file("./jtop_info.json");
    if(tmp.isNull()) {
        logger->error("系统读取jtop_info.json出错，未读出任何数据");
        return EXECUT_SCRIPT_ERROR;
    }
    (*info)["CPUUtil"] = tmp["CPU_utilization"];
    (*info)["Disk"] = tmp["hard_disk"];
    (*info)["memory"] = tmp["remain_memory"];
    return EXC_SUCCESS;
}

/**
 * @brief 获得边缘一体机的状态信息
 * 
 * @param info 将数据存入此JSON变量
 */
void get_calculation_info(Json::Value *info)
{
    get_jtop_info(info);
    get_IO_speed_now(info);
}
