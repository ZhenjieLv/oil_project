#ifndef _MANDRELRECORD_H
#define _MANDRELRECORD_H
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

typedef struct mandrelrecord
{
  uint32_t id;
  uint16_t device_id;
  uint16_t wellid;
  uint16_t layer;
  float traffic;
  uint16_t temperature;
  uint16_t internalpressure;
  uint16_t externalpressure;
  uint16_t openingaccuracy;
  uint16_t moisturepercentage;
  uint16_t status;
  uint16_t motorstatus;
  uint16_t motorcurrent;
  uint16_t voltage;
  uint16_t current;
  uint16_t operatingmode;
  uint16_t controlstatus;
  uint16_t yieldcontrol;
  float yieldvalue;
  uint16_t upflowdeviation;
  uint16_t downflowdeviation;
  uint16_t adjustmentminutes;
  uint16_t pressuretestcontrol;
  uint16_t pressuretesthour;
  uint16_t waterabsorptiontest;
  uint16_t faucetflag;
  std::string timestamp;
}mandrelrecord_t;

bool insert_mandrelrecord_info(mandrelrecord_t& mandrelrecord_info,bool is_sync);
std::vector<mandrelrecord_t> search_mandrelrecord_info();
bool update_mandrelrecord_info(mandrelrecord_t& mandrelrecord_info);
bool delete_mandrelrecord_info(uint32_t id);
bool delete_mandrelrecord_info(const char* end_time);
std::vector<mandrelrecord_t> search_mandrelrecord_not_upload_info();
bool update_mandrelrecord_info_upload(uint16_t id);
bool update_mandrelrecord_upload(uint32_t id);
bool insert_all_mandrelrecord_info(mandrelrecord_t& mandrelrecord_info);
#endif