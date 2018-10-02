#include "userfile.h"
#include "gc.h"

int main(void)
{
	int choose;
    UserFile us("\\\\crd0\\GC.sav");
	UserLogin(us);
	while(1)
	{
		choose = Menu();
		switch(choose)
		{
			case 1:Sl(us);break;
			case 2:Bing(us);break;
			case 3:Users(us);break;
			case 4:return 0;
		}
	}
	return 0;
}
