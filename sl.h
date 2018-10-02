#ifndef SL_H_
#define SL_H_

#include "userfile.h"

;;;int MainMenu(void);
void Instruction(void);
void About(void);
void Difficulty(void);
void PrintAll(void);
void CreateTable(void);
int OperateKey(int key);
int Lose(UserFile &us);
int Win(UserFile &us);
int Pause(void);

#endif