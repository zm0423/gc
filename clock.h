#ifndef CLOCK_H_
#define CLOCK_H_

// Type definitions.
  typedef unsigned char uchar;

// Structure definitions.
  typedef struct
  {
    unsigned char  R128CNT;
    unsigned char  gap1;
    unsigned char  RSECCNT;
    unsigned char  gap2;
    unsigned char  RMINCNT;
    unsigned char  gap3;
    unsigned char  RHOURCNT;
    unsigned char  gap4;
    unsigned char  RWEEKCNT;
    unsigned char  gap5;
    unsigned char  RDAYCNT;
    unsigned char  gap6;
    unsigned char  RMONCNT;
    unsigned char  gap7;
    unsigned short RYEARCNT;
  } TRTC;

  typedef struct Temps
  {
    int year;
    int mons;
    int days;
    int hour;
    int mins;
    int secs;
  }TEmps;

// Display functions.
  void Disp_Bitmap(const unsigned char *,int,int,int,int,int);
  void Disp_Clear(void);
  void Disp_Init(void);
  void Disp_Pixel(int,int,int);

// Main functions.
  void DrawTime(struct Temps *);
  void Set(struct Temps *);

// RTC functions.
  int  BCD2int(unsigned char *,int);
  void int2BCD(int,unsigned char *,int);
  void RTC2Time(unsigned char *, struct Temps *);
  void RTC_SetDateTime(struct Temps *);
  void RTC_GetDateTime(struct Temps *);
  int  GetMPU();

// Additional functions.
  int  GetKeyW(int *,int *,int,int,int,unsigned short *);
  int  GetMPU();

#endif