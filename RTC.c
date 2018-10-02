#include "clock.h"

int BCD2int(unsigned char *BCD, int bytes)
{
  int i,n;
  for(i=0,n=0;i<bytes;i++)
  {
      n += (*(BCD+i)&240)>>4;
      n *= 10;
      n += *(BCD+i) & 15;
      if(i != bytes-1) n*=10;
  }
  return n;
}

void int2BCD(int val, unsigned char *BCD, int bytes)
{
  int i;
  unsigned char n;
  for(i=0;i<bytes;i++)
  {
    n = (((val/10)%10)<<4) + (val%10);
    *(BCD+bytes-i-1) = n;
    val /= 100;
  }
}

void RTC2Time(unsigned char *rtc, struct Temps *ptemps)
{
  ptemps->year = BCD2int(rtc,2);
  ptemps->mons = BCD2int(rtc+2,1);
  ptemps->days = BCD2int(rtc+3,1);
  ptemps->hour = BCD2int(rtc+4,1);
  ptemps->mins = BCD2int(rtc+5,1);
  ptemps->secs = BCD2int(rtc+6,1);
}

void Time2RTC(struct Temps *ptemps, unsigned char *rtc)
{
  int2BCD(ptemps->year,rtc,2);
  int2BCD(ptemps->mons,rtc+2,1);
  int2BCD(ptemps->days,rtc+3,1);
  int2BCD(ptemps->hour,rtc+4,1);
  int2BCD(ptemps->mins,rtc+5,1);
  int2BCD(ptemps->secs,rtc+6,1);
}

void RTC_SetDateTime(struct Temps *ptemps)
{
  unsigned char rtc[8];
  volatile TRTC *pRTC;

  switch(GetMPU()) {
    case 3: case 4: pRTC = (TRTC*)0xA413FEC0; break;
    case 2: case 1: case 5: pRTC = (TRTC*)0xFFFFFEC0; break; // mpu7705 ?
    default : pRTC = 0; break; }
    
  if(pRTC)
  {
    Time2RTC(ptemps,rtc);
    (*pRTC).RYEARCNT = rtc[0]<<8 | rtc[1];
    (*pRTC).RMONCNT = rtc[2];
    (*pRTC).RDAYCNT = rtc[3];
    (*pRTC).RHOURCNT = rtc[4];
    (*pRTC).RMINCNT = rtc[5];
    (*pRTC).RSECCNT = rtc[6];
  }
}

void RTC_GetDateTime(struct Temps *time)
{
  unsigned char rtc[8];
  volatile TRTC *pRTC;
  int i;

  switch(GetMPU()) {
    case 3: case 4: pRTC = (TRTC*)0xA413FEC0; break;
    case 2: case 1: case 5: pRTC = (TRTC*)0xFFFFFEC0; break; // mpu7705 ?
    default : pRTC = 0; break; }
  
  if(pRTC)
  {
    rtc[0] = (*pRTC).RYEARCNT >> 8;
    rtc[1] = (*pRTC).RYEARCNT & 0x00FF;
    rtc[2] = (*pRTC).RMONCNT;
    rtc[3] = (*pRTC).RDAYCNT;
    rtc[4] = (*pRTC).RHOURCNT;
    rtc[5] = (*pRTC).RMINCNT;
    rtc[6] = (*pRTC).RSECCNT;
    rtc[7] = 0;
    RTC2Time(rtc,time);
  }
  else for(i=0;i<8;i++) rtc[i] = 0;
}

int GetMPU()
{
  int mpu;
  unsigned char savevalue;
  mpu = 0;
  savevalue = *(unsigned short*)0xA4000114;
  *(unsigned short*)0xA4000114 = 0xFFFF;

  switch (*(unsigned short*)0xA4000114)
  {
    case 0x0FFF: mpu = 1; break;
    case 0x00FF: mpu = 2; break;
    default:
      switch (*(unsigned int*)0xFF000030 & 0xFFFFFF00) {
        case 0x10300B00 :
          switch (*(unsigned int*)0xFF000044 & 0xFFFFFFF0) {
            case 0x00002C00: mpu = 3; break;
            case 0x00002200: mpu = 4; break; }
        break; }
      break;
  };    
  *(unsigned short*)0xA4000114 = savevalue;

  return mpu;
}