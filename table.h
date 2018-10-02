#ifndef TABLE_H_
#define TABLE_H_

// fontc code 
// small 0~8:0~8 
const int C_BLOCK = 9;
const int C_FLAG = 10;
const int C_CURSOR = 11;
const int C_UNKNOWN = 12;
const int C_MINE = 13;

class Table
{
 public:
    enum{AUTO = 50};
    // difficulties
    enum{D_NORMAL = 19,
         D_EASY = 14,
         D_HARD = 24,
         D_C_ONE = 29,
         D_C_TWO = 34};
    
    Table();
    ~Table(){}
    
    void InitializeTable(void);
    void RandomTable(int diff);
    
    // following x, y range 0-15, 0-7.
    // PrintBlock: x = AUTO read data from the table.
    void PrintBlock(int x, int y = AUTO);
    void PrintMines(void);
    void TransLocate(int x, int y);
    
    bool UseEnter(void);
    void UseFlag(void);
    void UseUnknown(void);
    bool UseAuto(void);
    
    int mine(void){return d_mine;}
    int flag(void){return d_flag;}
    int x(void){return d_x;}
    int y(void){return d_y;}
    int seen(void){return d_seen;}
    
    void setdiff(int diff){d_mine = diff;}
    
 private:
    struct Block
    {
        int b_status;
        int b_num;
    };
    
    // block status
    enum{S_UNSEEN,
         S_SEEN,
         S_FLAG,
         S_UNKNOWN};
    
    Block d_all[16][8];
    int d_mine;
    int d_x;
    int d_y;
    int d_flag;
    int d_seen;
    bool d_randomed;
    
    void Auto(int x, int y);
};

// x,y as table 
void PrintFontc(int x, int y, int frontc);

#endif