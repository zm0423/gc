#include<fxlib.h>
#include<cstring>
#include "user.h"
#include "pict.h"
#include "userfile.h"

static void GetLine(char *string, int line);
static char getkey(unsigned int key);
static bool alpha = true;

int LoginMenu(void)
{
    int i = 1;
    unsigned int key;
    pLoginMenu();
    Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
    while(1)
    {
        GetKey(&key);
        if(key == KEY_CTRL_EXE)break;
        if(key == KEY_CTRL_EXIT)return 4;
        if(key == KEY_CTRL_DOWN && i != 3)
        {
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
            ++i;
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
        }
        if(key == KEY_CTRL_UP && i != 1)
        {
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
            --i;
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
        }
    }
    return i;
}

bool GetUser(UserFile &us)
{
    char name[9], pass[9];
    int num;
    pGetUser();
    GetLine(name, 4);
    if(name[0] == '+')return false;
    GetLine(pass, 6);
    if(pass[0] == '+')return false;
    num = us.SearchUser(name);
    if(num == UserFile::UNEXISTED)
    {
        PopUpWin(1);
        fxprintf(4, 4, "User Unexisted!");
        Sleep(1500);
        return false;
    }
    if(!strcmp(pass, us.now().password))
    {
        PopUpWin(1);
        fxprintf(4, 4, "Wrong Password");
        Sleep(1500);
        return false;
    }
    else
    {
        PopUpWin(2);
        fxprintf(4, 3, "Welcome Back");
        fxprintf(4, 4, us.now().name);
        Sleep(1500);
    }
    return true;
}

bool RegisterAccount(UserFile &us)
{
    if(us.size() >= UserFile::MAX_USER)
    {
    	PopUpWin(1);
        fxprintf(4, 4, "User Full");
        Sleep(1500);
        return false;
	}
	char name[9], pass1[9], pass2[9];
    User u = {"        ", "        ", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    pResisterAccount1();
    GetLine(name, 4);
    while(us.SearchUser(name) != UserFile::UNEXISTED)
        GetLine(name, 4);
    pResisterAccount2();
    GetLine(pass1, 4);
    GetLine(pass2, 6);
    if(!strcmp(pass1, pass2))
    {
        PopUpWin(1);
        fxprintf(4, 4, "Wrong PassWord");
        Sleep(1500);
        return false;
    }
    strcpy(u.name, name);
    strcpy(u.password, pass1);
    us.AddUser(u);
    PopUpWin(1);
    fxprintf(4, 4, "Success!");
    Sleep(1500);
    return false;
}

bool Visitor(UserFile &us)
{
    us.set(UserFile::VISITOR);
    return true;
}

int UserMenu(void)
{
    int i = 1;
    unsigned int key;
    pUserMenu();
    Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
    while(1)
    {
        GetKey(&key);
        if(key == KEY_CTRL_EXE)break;
        if(key == KEY_CTRL_EXIT)return 4;
        if(key == KEY_CTRL_DOWN && i != 3)
        {
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
            ++i;
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
        }
        if(key == KEY_CTRL_UP && i != 1)
        {
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
            --i;
            Bdisp_AreaReverseVRAM(78, 15 + 8 * i, 127, 22 + 8 * i);
        }
    }
    return i;
}

void ShowData(UserFile &us)
{
    int num = 1;
    unsigned int key;
    bool up = true;
    us.show(num, up);
    while(1)
    {
        GetKey(&key);
        if(key == KEY_CTRL_EXIT)return ;
        if((up && key == KEY_CTRL_DOWN) || (!up && key == KEY_CTRL_UP))up = !up;
        if(num != 1 && key == KEY_CTRL_LEFT) --num;
        if(num != us.size() && key == KEY_CTRL_RIGHT)++num;
        us.show(num, up);
    }
}

void ChangePassword(UserFile &us)
{
    char pass1[9], pass2[9];
    pChangePassword();
    GetLine(pass1, 4);
    GetLine(pass2, 6);
    if(!strcmp(pass1, pass2))
    {
        PopUpWin(1);
        fxprintf(4, 4, "Wrong PassWord");
        Sleep(1500);
        return ;
    }
    strcpy(us.now().password, pass1);
    us.flush();
    PopUpWin(1);
    fxprintf(4, 4, "Success!");
    Sleep(1500); 
}

void Adminster(UserFile &us)
{
    Bdisp_AllClr_DDVRAM();
    fxprintf(1, 1, "Adminster Password");
    us.ad_show();
}

static void GetLine(char *string, int line)
{
    strcpy(string, "\0\0\0\0\0\0\0\0\0");
	unsigned int key;
    int lo = 1, size = 0;
    char c;
    while(1)
    {
        fxprintf(21, 8, "%c", alpha ? 'A' : 'a');
        GetKey(&key);
        if(key == KEY_CTRL_EXE && *string)
        {
        	if(strlen(string) <= 8)
        	    for(int a = strlen(string);a != 9;++a)
        	        string[a] = '\0';
	        return ;
        }
        if(key == KEY_CTRL_F2)
        {
            alpha = !alpha;
            continue;
        }
        if(key == KEY_CTRL_DEL && lo != 1)
        {
            if(size < lo)
            {
                fxprintf(--lo + 13, line, "  ");
                string[(--size) - 2] = '\0';
            }
        }
        c = getkey(key);
        if(c == '+')
        {
            string[0] = '+';
            return ;
        }
        if(c == 0 || size >= 8)
            continue;
            string[size++] = c;
            fxprintf((lo++)+13, line, "%c", c);
    }
}

static char getkey(unsigned int key)
{
    if(key >= KEY_CHAR_0 && key <= KEY_CHAR_9)
        return key;
    if(key >= KEY_CHAR_A && key <= KEY_CHAR_Z)
        return (alpha ? key : key + 32);
    if(key == KEY_CHAR_MINUS)
        return '-';
    if(key == KEY_CHAR_COMMA)
        return ',';
    if(key == KEY_CTRL_EXIT)
        return '+';
    return 0;
}



