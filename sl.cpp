#include<fxlib.h>
#include"sl.h"
#include"pict.h"
#include"table.h"

static Table ta;

//print main menu
int MainMenu(void)
{
    int a = 1;
    unsigned int key;
    Bdisp_AllClr_DDVRAM();
    pMainMenu();
    Bdisp_AreaReverseVRAM(25, 21, 91, 29);
    while(1)
    {
       GetKey(&key);
       if(key == KEY_CTRL_DOWN && a != 4)
       {
           Bdisp_AreaReverseVRAM(25, 10 * a + 11, 91, 10 * a + 19);
           a++;
           Bdisp_AreaReverseVRAM(25, 10 * a + 11, 91, 10 * a + 19);
       }
       if(key == KEY_CTRL_UP && a != 1)
       {
           Bdisp_AreaReverseVRAM(25, 10 * a + 11, 91, 10 * a + 19);
           a--;
           Bdisp_AreaReverseVRAM(25, 10 * a + 11, 91, 10 * a + 19);
       }
       if(key == KEY_CTRL_EXE) break;
       if(key == KEY_CTRL_EXIT)
       {
           a = 4;
           break;
       }
    }
    Bdisp_AllClr_DDVRAM();
    return a;
}

// print an instruction
void Instruction(void)
{
    int a = 1;
    int kcode1,kcode2;
    short unused;
    Bdisp_AllClr_DDVRAM();
    pInstruction1();
    while(kcode1 != 3 || kcode2 != 2)
    {
        if(a==3)a=1;
        fxprintf(1,7, a==1?"PRESS:[EXE]":"            ");
        Bdisp_PutDisp_DD();
        Bkey_GetKeyWait(&kcode1,&kcode2,
                 KEYWAIT_HALTON_TIMERON,1,1,&unused);
        a++;
    }
    Bdisp_AllClr_DDVRAM();
    pInstruction2();
    kcode1=1;
    a=1;
    while(kcode1!=3||kcode2!=2)
    {
        if(a==3)a=1;
        fxprintf(1,7, a==1?"PRESS:[EXE]":"            ");
        Bdisp_PutDisp_DD();
        Bkey_GetKeyWait(&kcode1,&kcode2,
                 KEYWAIT_HALTON_TIMERON,1,1,&unused);
        a++;
    }
    Bdisp_AllClr_DDVRAM();
}

// print an About 
void About(void)
{
    int a=1;
    int kcode1,kcode2;
    short unused;
    Bdisp_AllClr_DDVRAM();
    pAbout();
    while(kcode1!=3||kcode2!=2)
    {
        if(a==3)a=1;
        fxprintf(1,7, a==1?"PRESS:[EXE]":"            ");
        Bdisp_PutDisp_DD();
        Bkey_GetKeyWait(&kcode1,&kcode2,
                 KEYWAIT_HALTON_TIMERON,1,1,&unused);
        a++;
    }
    Bdisp_AllClr_DDVRAM();
}

// print difficulty choice 
void Difficulty(void)
{
   int a=1,di;
   unsigned int key;
   pDifficulty();
   Bdisp_AreaReverseVRAM(30,8*a+12,92,8*a+20);
   while(1)
   {
        GetKey(&key);
        if(key==KEY_CTRL_DOWN&&a!=5)
        {
            Bdisp_AreaReverseVRAM(30,8*a+12,92,8*a+20);
            a++;
            Bdisp_AreaReverseVRAM(30,8*a+12,92,8*a+20);
        }
        if(key==KEY_CTRL_UP&&a!=1)
        {
            Bdisp_AreaReverseVRAM(30,8*a+12,92,8*a+20);
            a--;
            Bdisp_AreaReverseVRAM(30,8*a+12,92,8*a+20);
        }
        if(key==KEY_CTRL_EXE) break;
  }
 switch(a)
 {
  case 1:di=Table::D_NORMAL;
             break;
  case 2:di= Table::D_EASY;
             break;
  case 3:di= Table::D_HARD;
             break;
  case 4:di= Table::D_C_ONE;
             break;
  case 5:di= Table::D_C_TWO;
             break;
 default:di= Table::D_NORMAL;
             break;
 }
 ta.setdiff(di);
}

void CreateTable(void)
{
    ta.InitializeTable();
}

void PrintAll(void)
{
    Bdisp_AllClr_DDVRAM();
    for(int i = 0;i < 16;i++)
    {
        for(int j = 0;j < 8;j++)
        {
            ta.PrintBlock(i, j);
        }
    }			
    Bdisp_DrawLineVRAM(98, 0, 98, 63);
    Bdisp_DrawLineVRAM(99, 0, 99, 63);
    Bdisp_DrawLineVRAM(100, 0, 100, 63);
    fxprintf(18, 2, "ALL:");
    fxprintf(18, 3, "%d",ta.mine());
    fxprintf(18, 5, "NOW:");
    fxprintf(18, 6, "%d",ta.mine() - ta.flag());
    PrintFontc(ta.x(), ta.y(), C_CURSOR);
    Bdisp_PutDisp_DD();
}

/* react to the key pressed */
/* return:0 continue 1 win 2 lose 3 exit to mainmenu */
int OperateKey(int key)
{
    bool ck = true;
    int pau = 0;
    int xx, yy;
    xx = ta.x();
    yy = ta.y();
    fxprintf(18, 6, "%d",ta.mine() - ta.flag());
    Bdisp_PutDisp_DD();
    switch(key)
    {
        case KEY_CHAR_COS:ck = ta.UseEnter();break;
        case KEY_CTRL_F6:ta.UseFlag()       ;break;
        case KEY_CTRL_F5:ta.UseUnknown()    ;break;
        case KEY_CHAR_TAN:ck = ta.UseAuto() ;break;
        case KEY_CTRL_EXIT:pau = Pause()    ;break;
        case KEY_CHAR_LPAR: ta.TransLocate(0, 7) ;break;
        case KEY_CHAR_LN:   ta.TransLocate(0, 0) ;break;
        case KEY_CHAR_SIN:  ta.TransLocate(15, 0);break;
        case KEY_CHAR_RPAR: ta.TransLocate(15, 7);break;
        case KEY_CTRL_LEFT: ta.TransLocate((xx + 15) % 16, yy);break;
        case KEY_CTRL_RIGHT:ta.TransLocate(++xx % 16, yy)     ;break;
        case KEY_CTRL_DOWN: ta.TransLocate(xx, ++yy % 8)      ;break;
        case KEY_CTRL_UP:   ta.TransLocate(xx, (yy + 7) % 8)  ;break;
    }
    fxprintf(18, 6, "%d", ta.mine() - ta.flag());
    if(pau == 1) return 3;
    if(ta.mine() - ta.flag() < 10 && ta.mine() - ta.flag() >= 0)
          fxprintf(19, 6, " ");
    if(ck == false)return 2;
    if(ta.seen() == 16 * 8 - ta.mine())
       return 1;
    return 0;
}

int Lose(UserFile &us)
{
    int a = 1;
    int kcode1, kcode2;
    short unused;
    unsigned int key;
    ta.PrintMines();
    Bkey_GetKeyWait(&kcode1, &kcode2, 
                 KEYWAIT_HALTON_TIMERON, 3, 1, &unused);
    Bdisp_AllClr_DDVRAM();
    if(us.ShowNum() != UserFile::VISITOR)
    {
    	++(us.now()).data[2];
        switch(ta.mine())
        {
            case Table::D_NORMAL: ++(us.now()).data[5];break;
            case Table::D_EASY  : ++(us.now()).data[3];break;
            case Table::D_HARD  : ++(us.now()).data[7];break;
        }
        us.flush();
    }
    pLose();
    Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
    while(1)
    {
        GetKey(&key);
        if(key == KEY_CTRL_DOWN && a != 3)
        {
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
            a++;
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
        }
        if(key == KEY_CTRL_UP && a != 1)
        {
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
            a--;
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
        }
        if(key == KEY_CTRL_EXE) break;
    }
    Bdisp_AllClr_DDVRAM();
    return a;
}

int Win(UserFile &us)
{
    int a = 1;
    int kcode1, kcode2;
    short unused;
    unsigned int key;
    ta.PrintMines();
    Bkey_GetKeyWait(&kcode1,&kcode2,
                 KEYWAIT_HALTON_TIMERON,3,1,&unused);
    Bdisp_AllClr_DDVRAM();
    if(us.ShowNum() != UserFile::VISITOR)
    {
    	++(us.now()).data[2];
        switch(ta.mine())
        {
            case Table::D_EASY: ++(us.now()).data[3];
			                      ++(us.now()).data[4];break;
            case Table::D_NORMAL : ++(us.now()).data[5];
			                      ++(us.now()).data[6];break;
            case Table::D_HARD  : ++(us.now()).data[7];
			                      ++(us.now()).data[8];break;
            case Table::D_C_ONE   : ++(us.now()).data[9];break;
            case Table::D_C_TWO   : ++(us.now()).data[10];break;
        }
        us.flush();
    }
    pWin();
    Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
    while(1)
    {
        GetKey(&key);
        if(key == KEY_CTRL_DOWN && a != 3)
        {
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
            a++;
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
        }
        if(key == KEY_CTRL_UP && a != 1)
        {
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
            a--;
            Bdisp_AreaReverseVRAM(3, 10 * a + 21, 75, 10 * a + 29);
        }
        if(key == KEY_CTRL_EXE) break;
    }
    Bdisp_AllClr_DDVRAM();
    return a;
}

int Pause(void)
{
    unsigned int key;
    PopUpWin(3);
    fxprintf(4, 3, "Pause.");
    fxprintf(4, 4, "[F1]:EXIT");
    fxprintf(4, 5, "[EXE]:continue");
    while(1)
    {
        GetKey(&key);
        if(key == KEY_CTRL_EXE)
        {
            PrintAll();
            return 0;
        }
        if(key == KEY_CTRL_F1)
            return 1;
    }
}

