#ifndef _MANDREL_H
#define _MANDREL_H
#include <cstdint>
#include <string>
#include <vector>
#include "mandrelrecord.h"
using namespace std;

typedef struct mandrel
{
  uint16_t id;
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
}mandrel_t;

bool insert_mandrel_info(mandrel_t& mandrel_info);
bool insert_mrecord_info(mandrel_t& mandrel_info,bool is_sync);
std::vector<mandrel_t> search_mandrel_info();
bool search_mandrel_not_info(uint16_t id);
bool update_mandrel_info(mandrelrecord_t& mandrel_info,uint16_t id);
bool delete_mandrel_info(uint32_t id);
std::vector<mandrel_t> search_mandrel_not_upload_info();
bool update_mandrel_info_upload(uint32_t id);
bool insert_all_mandrel_info(mandrel_t& mandrel_info);
#endif