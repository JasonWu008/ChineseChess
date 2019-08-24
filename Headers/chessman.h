#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <QString>

class Chessman
{
public:
    Chessman();

    enum TYPE{KING,MANDARIN,ELEPHANT,ROOK,KNIGHT,CANNON,PAWN};

    TYPE type;    //棋子类型；

    void initialize(int);     //初始化函数;
    void rotate();            //旋转函数;
    QString get_text();       //获取棋子种类;
    int get_color();          //棋子颜色;
    int get_x();              //棋子横坐标;
    int get_y();              //棋子纵坐标;
    bool if_dead();           //棋子是否阵亡;
    void turn_dead_to_true(); //棋子阵亡;
    void turn_dead_to_false();//棋子复活;
    void change_x(int);       //改变x值;
    void change_y(int);       //改变y值;
    int get_id();             //返回id值;

private:
    int x,y;      //坐标；
    int ID;       //棋子序号；
    bool dead;    //是否已阵亡；
    int color;    //1代表红方，2代表黑方；
    struct
    {
        int x,y;
        Chessman::TYPE type;
    }chess_piece[16]={
    {0,0,ROOK},
    {1,0,KNIGHT},
    {2,0,ELEPHANT},
    {3,0,MANDARIN},
    {4,0,KING},
    {5,0,MANDARIN},
    {6,0,ELEPHANT},
    {7,0,KNIGHT},
    {8,0,ROOK},

    {1,2,CANNON},
    {7,2,CANNON},

    {0,3,PAWN},
    {2,3,PAWN},
    {4,3,PAWN},
    {6,3,PAWN},
    {8,3,PAWN},
    };
};

#endif // CHESSMAN_H
