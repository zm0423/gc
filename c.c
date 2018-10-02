int AddIn_main(int isAppli, unsigned short OptionNum)
{
    extern int main(void);
    return main();
}

#pragma section _BR_Size
unsigned long BR_Size;
#pragma section

#pragma section _TOP
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}
#pragma section

