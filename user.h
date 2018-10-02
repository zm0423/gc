#ifndef USER_H_
#define USER_H_

#include "userfile.h"

//Logins
;int LoginMenu(void);
bool GetUser(UserFile &us);
bool RegisterAccount(UserFile &us);
bool Visitor(UserFile &us);

//User Services
int UserMenu(void);
void ShowData(UserFile &us);
void ChangePassword(UserFile &us);
void FindPassword(UserFile &us);
void Adminster(UserFile &us);

#endif