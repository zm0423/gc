#include<cstdlib>
#include<cstdarg>
#include<cstdio>
#include "fxlib.h"
#include "table.h"
#include "pict.h"

extern "C"{
#include "clock.h"
}

const int COL_MAX = 16;    
const int ROW_MAX = 8;


void PrintFontc(int x, int y, int frontc)
{
    unsigned char n0[]={0xE5, 0xE0,0};
    unsigned char n1[]={0xE5, 0xD1,0};
    unsigned char n2[]={0xE5, 0xD2,0};
    unsigned char n3[]={0xE5, 0xD3,0};
    unsigned char n4[]={0xE5, 0xD4,0};
    unsigned char n5[]={0xE5, 0xD5,0};
    unsigned char n6[]={0xE5, 0xD6,0};
    unsigned char n7[]={0xE5, 0xD7,0};
    unsigned char n8[]={0xE5, 0xD8,0};
    unsigned char block[]={0xE6, 0xA6,0};
    unsigned char flag[]={0xE6, 0x92,0};
    unsigned char un[]={0x3F, 0};
    unsigned char mine[]={0xE5, 0x9B,0};
    unsigned char cur[]={0xE5, 0xA6,0};
    
    unsigned char *re;
    switch(frontc)
    {
        case 0:re = n0;break;
        case 1:re = n1;break;
        case 2:re = n2;break;
        case 3:re = n3;break;
        case 4:re = n4;break;
        case 5:re = n5;break;
        case 6:re = n6;break;
        case 7:re = n7;break;
        case 8:re = n8;break;
        case C_BLOCK  :re = block;break;
        case C_FLAG   :re = flag ;break;
        case C_UNKNOWN:re = un   ;break;
        case C_MINE   :re = mine;break;
        case C_CURSOR :re = cur;break;
        default:re = block;
    }
    locate(x+1, y+1);
    PrintC(re);
    Bdisp_PutDisp_DD();
}

Table::Table()
{
    InitializeTable();
}

void Table::InitializeTable(void)
{
    for(int i = 0;i != COL_MAX;++i)
        for(int j = 0;j != ROW_MAX;++j)
        {
            d_all[i][j].b_status = S_UNSEEN;
            d_all[i][j].b_num = 0;
        }
    d_x = 7;
    d_y = 3;
    d_flag = 0;
    d_seen = 0;
    d_randomed = false;
}

void Table::RandomTable(int diff)
{
    int x,y;
    int tim;
    TEmps t;
    // get rand seed from RTC
    RTC_GetDateTime(&t);
  tim=((t.mins<<6)|t.secs)|((t.hour&0x0F)<<12);
  srand(tim);
    d_mine = diff;
    for(int i = 1;i <= d_mine;i++)
    {
        while(1)
        {
            x=(rand() % COL_MAX);
            y=(rand() % ROW_MAX);
            if(x == d_x && y == d_y)
                         continue;
            if(d_all[x][y].b_num != -1)break;
        }
        d_all[x][y].b_num = -1;
        for(int j = -1;j <= 1;j++)
        {
            for(int k = -1;k <= 1;k++)
            {
                if((!j) && (!k))continue;
                if(x+j == -1||x+j == COL_MAX||y+k == -1||y+k == ROW_MAX)continue;
                if(d_all[x+j][y+k].b_num == -1)continue;
                d_all[x+j][y+k].b_num++;
            }
        }
    }
    d_randomed = true;
}

void Table::PrintBlock(int x, int y)
{
    int i,j,st,nu;
    int ch;
    if(x == AUTO && y == AUTO)
    {
        i = d_x;
        j = d_y;
    }
    else
    {
        i = x;
        j = y;
    }
    st = d_all[i][j].b_status;
    nu = d_all[i][j].b_num;
    switch(st)
    {
       case S_SEEN   :ch = nu       ;break;
       case S_UNSEEN :ch = C_BLOCK  ;break;
       case S_FLAG   :ch = C_FLAG   ;break;
       case S_UNKNOWN:ch = C_UNKNOWN;break;
    }
    PrintFontc(i, j, ch);
}

void Table::TransLocate(int x,int y)
{
    PrintBlock(AUTO, AUTO);
    d_x = x;
    d_y = y;
    PrintFontc(x, y, C_CURSOR);
}

bool Table::UseEnter(void)
{
    int x = d_x;
    int y = d_y;
    if(d_all[x][y].b_status == S_SEEN || 
                 d_all[x][y].b_status == S_FLAG)
       return true;
    if(!d_randomed)RandomTable(d_mine);
    if(d_all[x][y].b_num == -1)
       return false;
    if(d_all[x][y].b_num == 0)
       Auto(x, y);
    else
    {  
         d_all[x][y].b_status = S_SEEN;
         d_seen++;
         PrintBlock(AUTO, AUTO);
         Bdisp_PutDisp_DD();
         Sleep(250);   
     }
    PrintFontc(x, y, C_CURSOR);
    Bdisp_PutDisp_DD();
    return true;
}
    
void Table::UseFlag(void)
{
    int x = d_x;
    int y = d_y;
    if(d_all[x][y].b_status==S_SEEN)
       return ;
    if(d_all[x][y].b_status==S_FLAG)
        {
           d_all[x][y].b_status=S_UNSEEN;
           d_flag--;
        }
    else
        {
           d_all[x][y].b_status=S_FLAG;
           d_flag++;
        }
    PrintBlock(AUTO, AUTO);
    Bdisp_PutDisp_DD();
    Sleep(250);
    PrintFontc(x,y,C_CURSOR);
    Bdisp_PutDisp_DD();
}

void Table::UseUnknown(void)
{
    int x=d_x;
    int y=d_y;
    if(d_all[x][y].b_status==S_SEEN||
                     d_all[x][y].b_status==S_FLAG)
       return ;
    if(d_all[x][y].b_status==S_UNSEEN)
       d_all[x][y].b_status=S_UNKNOWN;
    else
       d_all[x][y].b_status=S_UNSEEN;
    PrintBlock(AUTO,AUTO);
     Bdisp_PutDisp_DD();
    Sleep(250);
    PrintFontc(x,y,C_CURSOR);
    Bdisp_PutDisp_DD();
}

bool Table::UseAuto(void)
{
    int x=d_x;
    int y=d_y;
    int number=d_all[x][y].b_num;
    int count=0;
    int other=0;
    int list[8][2];
    int len=0;
    int i,k,j;
    bool cck=true;
    if(d_all[x][y].b_status!=S_SEEN)
        return true;
    for(j=-1;j<=1;j++)
    {
        for(k=-1;k<=1;k++)
        {
            if((!j)&&(!k))continue;
            if(x+j==-1||x+j==COL_MAX||y+k==-1||y+k==ROW_MAX)continue;
            if(d_all[x+j][y+k].b_status==S_FLAG)
            {
                 count++;
                 if(d_all[x+j][y+k].b_num!=-1)
                       other++;
                 
            }

        }
    }
    if(count==number&&other>0)
       return false;
    if(count<number||count>number)
       return true;
    for(j=-1;j<=1;j++)
    {
        for(k=-1;k<=1;k++)
        {
            if(d_all[x+j][y+k].b_status!=S_UNSEEN)continue;
            if((!j)&&(!k))continue;
            if(x+j==-1||x+j==COL_MAX||y+k==-1||y+k==ROW_MAX)continue;
            if(d_all[x+j][y+k].b_num!=-1)
            {
                if(d_all[x+j][y+k].b_num==0)
                {
                    list[len][0]=x+j;
                    list[len++][1]=y+k;
                    fxprintf(x+j+1,y+k+1," ");
                    Bdisp_PutDisp_DD();
                }
                else
                {
                    d_all[x+j][y+k].b_status=S_SEEN;
                    d_seen++;
                    PrintBlock(x+j,y+k);
                }
            }

        }
    }
    if(len==0)
      return true;
    if(len==1)
    {
        Auto(list[0][0],list[0][1]);
        return true;
    }
    else
    {
        Auto(list[0][0],list[0][1]);
        for(i=0;i<len;i++)
        {
            if(d_all[list[i][0]][list[i][1]].b_status!=S_SEEN)
            {
                cck=false;
                list[7][0]=list[i][0];
                list[7][1]=list[i][1];
            }
        }
        if(cck==false)
            Auto(list[7][0],list[7][1]);
    }
    return true;
}


void Table::Auto(int x,int y)
{
    int lista[128]={0};
    int listb[128]={0};
    int length=1;
    int pres=0;
    int j=0,k=0;
    int a=0,b=0;

    d_all[x][y].b_status=S_SEEN;
    PrintBlock(x,y);
    d_seen++;
    lista[0]=x;
    listb[0]=y;
    while(pres<length)
    {
         a = lista[pres];
        b = listb[pres];
        for(j=-1;j<=1;j++)
        {
         for(k=-1;k<=1;k++)
         {
            if((!j)&&(!k))continue;
            if(a+j == -1||a+j == COL_MAX||b+k == -1||b+k == ROW_MAX)continue;
            if((d_all[a+j][b+k]).b_status != S_UNSEEN) 
               continue;
            if((d_all[a+j][b+k]).b_num == 0)
                {
                    lista[length]=a+j;
                    listb[length]=b+k;
                    length++;
                }
                (d_all[a+j][b+k]).b_status=S_SEEN;
                d_seen++;
                PrintBlock(a+j,b+k);
         }
        }
     pres++;
     
    }

}

void Table::PrintMines(void)
{
	for(int i = 0;i < COL_MAX;i++)
    {
        for(int j = 0;j < ROW_MAX;j++)
        {
            if(d_all[i][j].b_num == -1)
                PrintFontc(i, j, C_MINE);
        }
    }
}

