#include<fxlib.h>
#include<cstring>
#include "user.h"
#include "pict.h"
#include "errorr.h"
using std::vector;

User & User::operator=(const User &us)
{
    strcpy(name, us.name);
    strcpy(password, us.password);
    for(int i = 0;i !=11;++i)
        data[i] = us.data[i];
    return *this;
}

FONTCHARACTER * UserFile::CharToFont(const char *cFileName,FONTCHARACTER *fFileName)
{
   int i,len = strlen(cFileName);
   for(i=0; i<len ;++i)
       fFileName[i] = cFileName[i];
   fFileName[i]=0;
   return fFileName;
}

UserFile::UserFile(const char *s)
{
    User us;
    int size;
    CharToFont(s, fname);
    f_handle = Bfile_OpenFile(fname, _OPENMODE_READWRITE);
    if(!f_handle)
    {
        Bfile_CreateFile(fname, 0);
        f_handle = Bfile_OpenFile(fname, _OPENMODE_READWRITE);
        d_num = 0;
    }
    else
    {
        size = Bfile_GetFileSize(f_handle) / USER_SIZE;
        for(int i = 0;i != size; ++i)
        {
            Bfile_ReadFile(f_handle, &us, USER_SIZE, i * USER_SIZE);
            d_users.push_back(us);
        }
        d_num = 0;
    }
}

int UserFile::SearchUser(const char *uname)
{
    for(d_num = 0;d_num != d_users.size();++d_num)
        if(!strcmp(d_users[d_num].name, uname))
            break;
    if(d_num == d_users.size())
        d_num = UNEXISTED;
    return d_num;
}

bool UserFile::AddUser(const User &us)
{
    if(d_users.size() >= MAX_USER)
        return false;
    d_users.push_back(us);
    flush();
    return true;
}

void UserFile::flush(void)
{
    if(d_num <= 0 || !d_users.size())
        return ;
    Bfile_DeleteFile(fname);
    Bfile_CreateFile(fname, USER_SIZE * d_users.size());
    f_handle = Bfile_OpenFile(fname, _OPENMODE_READWRITE);
    for(int i = 0;i != d_users.size();++i)
    {
        Bfile_SeekFile(f_handle, i * USER_SIZE);
        Bfile_WriteFile(f_handle, &d_users[i], USER_SIZE);
    }
}

void UserFile::show(int num, int upp)
{
    Bdisp_AllClr_DDVRAM();
	--num;
	if(upp)
	{
		fxprintf(1, 1, d_users[num].name);
		fxprintf(10, 1, "All:%d", d_users[num].data[2]);
		fxprintf(1, 2, "SL:  All  Win  rate");
		fxprintf(1, 3, "Easy                %");
		fxprintf(1, 4, "Norm                %");
		fxprintf(1, 5, "Hard                %");
		fxprintf(1, 6, "Challenge1 win:");
		fxprintf(1, 7, "Challenge2 win:");
		fxprintf(6, 3, "%d", d_users[num].data[3]); 
		fxprintf(11, 3, "%d", d_users[num].data[4]); 
		fxprintf(6, 4, "%d", d_users[num].data[5]); 
		fxprintf(11, 4, "%d", d_users[num].data[6]); 
		fxprintf(6, 5, "%d", d_users[num].data[7]); 
		fxprintf(11, 5, "%d", d_users[num].data[8]); 
		fxprintf(16, 6, "%d", d_users[num].data[9]); 
		fxprintf(16, 7, "%d", d_users[num].data[10]); 
		fxprintf(16, 3, "%.2f", (static_cast<float>(d_users[num].data[4]))/d_users[num].data[3]);
		fxprintf(16, 4, "%.2f", (static_cast<float>(d_users[num].data[6]))/d_users[num].data[5]);
		fxprintf(16, 5, "%.2f", (static_cast<float>(d_users[num].data[8]))/d_users[num].data[7]); 
		fxprintf(19, 1, upp ? "\xE6\x9D":"\xE6\x9C");
		if(num != 0)fxprintf(20, 1, "\xE6\x9A");
		if(num != d_users.size() - 1)fxprintf(21, 1, "\xE6\x9B");
	}
	else
	{
		fxprintf(1, 1, d_users[num].name);
		fxprintf(9, 1, "BING");
		fxprintf(1, 2, "All:%d", d_users[num].data[0]);
		fxprintf(1, 3, "Win:%d", d_users[num].data[1]);
		fxprintf(1, 4, "rate:     %");
		fxprintf(6, 4, "%.2f", (static_cast<float>(d_users[num].data[1]))/d_users[num].data[0]);
		fxprintf(19, 1, upp ? "\xE6\x9D":"\xE6\x9C");
		if(num != 0)fxprintf(20, 1, "\xE6\x9A");
		if(num != d_users.size() - 1)fxprintf(21, 1, "\xE6\x9B");
	}
}

void UserFile::ad_show(void)
{
	Bdisp_AllClr_DDVRAM();
    for(int i = 0;i != d_users.size();++i)
    {
    	fxprintf(1, i + 1, "%d", i + 1);
    	fxprintf(2, i + 1, ".");
		fxprintf(4, i + 1, d_users[i].name);
		fxprintf(13, i + 1, d_users[i].password);
    }		 
}
