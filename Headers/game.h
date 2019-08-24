#ifndef GAME_H
#define GAME_H

#include "chessboard.h"
#include "step.h"
#include <QVector>
#include <QMap>

class Game:public ChessBoard
{
    Q_OBJECT
public:
    Game();
    ~Game();
    enum TYPE{KING,MANDARIN,ELEPHANT,ROOK,KNIGHT,CANNON,PAWN};

    virtual void click(int,int,int);

    void get_all_move(QVector<step*>&);       //获取所有可能的走法;
    step* get_best_move();                    //获取最佳的走法;

    void try_one_move(step* step);            //走一步;
    void back_one_move(step*step);            //后退一步;
    int evaluate_red();                       //评估红方;
    int evaluate_black();                     //评估黑方;
    int evaluate();                           //评估局势;
    int get_min_value(int,int);               //获取当前局势最小值;
    int get_max_value(int,int);               //获取当前局势最大值;
    void back();

    void retract_game();                      //悔棋函数;

    void init_level(int);                      //初始化搜索层数(控制难度);
    int get_max_depth(){return this->max_depth;}

public slots:
    void computer_move();                     //电脑移动;

private:
    int max_depth=4;
    int level;                              //搜索层数;
    QMap<Chessman::TYPE,int> map1;          //给不同的棋子分配价值权重;
    QMap<Chessman::TYPE,int> map2;          //棋子被对方控制时对方所能获得的价值;
};

#endif // GAME_H
