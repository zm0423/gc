#include<fxlib.h>
#include "pict.h"
#include "user.h"
#include "gc.h"
#include "sl.h"

void UserLogin(UserFile &uf)
{
	int i;
	bool b = false;
	while(!b)
         {
          i = LoginMenu();
		switch(i)
	    {
		    case 1:b = GetUser(uf);        break;
		    case 2:b = RegisterAccount(uf);break;
	    	case 3:b = Visitor(uf);        break;
    	}
         }
}
    	
int Menu(void)
{
	int i = 1;
        unsigned int key; 
	pMenu();
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

void Sl(UserFile &uf)
{
    int mai, check, end;
    unsigned int key;
	while(1)
    {
        mai = MainMenu();
        switch(mai)
        {
        	case 2:Instruction();continue;break;
        	case 3:About();continue;break;
        	case 4:return ;
	    } 
        Difficulty();
        while(1)
        {
            Bkey_Set_RepeatTime(20,6);
            CreateTable();
            PrintAll();
            GetKey(&key);
            while(!(check = OperateKey(key)))
                GetKey(&key);
            switch(check)
            {
                case 1:end = Win(uf);break;
                case 2:end = Lose(uf);break;
            }
            if(check == 3)break;
            if(end == 2)
                Difficulty();
            else if(end == 3)break;
        }
    }
}

//not implemented
void Bing(UserFile &uf)
{
	PopUpWin(1);
	fxprintf(4, 4, "Not implemented");
	Sleep(2000);
}

void Users(UserFile &uf)
{
	int uss;
	while(1)
	{
		uss = UserMenu();
		switch(uss)
		{
			case 1:ShowData(uf);break;
			case 2:ChangePassword(uf);break;
			case 3:Adminster(uf);break;
			case 4:return ;
		}
	}
		
}



    	
    	
    	
    	
	