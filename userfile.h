#ifndef USERFILE_H_
#define USERFILE_H_

#include<vector>
#include<fxlib.h>

const int NAME_SIZE = 8;

struct User
{
    char name[NAME_SIZE + 1];
    char password[NAME_SIZE + 1];
    short data[11];
    /*   data                     *
     *   1,2 bing all & win       *
     *   3 miner all              *
     *   4,5 normal all & win     *
     *   6,7 easy all & win       *
     *   8,9 hard all & win       *
     *   10,11 challenge 1&2 win  */
    User & operator=(const User &us);
};

const int USER_SIZE = 2 * NAME_SIZE + 2 + 11 * 2;

class UserFile
{
 public:
    enum{MAX_USER = 8};
    
    UserFile(const char *s);
    ~UserFile(){Bfile_CloseFile(f_handle);}
    
    enum{UNEXISTED = 50, VISITOR = 60};
    int SearchUser(const char *uname);
    bool AddUser(const User &us);
	void flush(void);
    User &now(void){return d_users[d_num];}
    void set(int num){d_num = num;}
    int ShowNum(void){return d_num;}
    
    void show(int num, int upp);
    void ad_show(void);
    int size(void){return d_users.size();}
 private:
    std::vector<User> d_users;
    int f_handle;
    FONTCHARACTER fname[32];
    int d_num;
    
    FONTCHARACTER * CharToFont(const char *cFileName,FONTCHARACTER *fFileName);
}

#endif
