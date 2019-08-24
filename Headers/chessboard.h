#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include "Chessman.h"
#include "step.h"
#include <QPoint>
#include <QVector>
#include <QMessageBox>
#include <QLabel>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);

    QVector<step*>steps;                           //保存走棋步骤;
    Chessman chessman[32];                         //保存所有棋子;
    QVector <step*> retract;                         //保存悔棋步骤;

    void init(bool);                               //初始化函数;
    QSize sizeHint() const;                        //设置窗口默认大小;
    void paintEvent(QPaintEvent*);                 //绘图函数;
    void draw_chessman(QPainter&,int);             //绘制棋子;
    void mouseReleaseEvent(QMouseEvent*);          //鼠标点击事件;

    QPoint center(int,int);                        //将棋子在棋盘中的坐标转换成窗口的实际坐标;(传入x,y值)
    QPoint center(int);                            //将棋子在棋盘中的坐标转换成窗口的实际坐标;(传入棋子id值)
    QPoint windows_center(int,int);                //将窗口中的坐标转换成棋盘的坐标;

    bool get_click_xy(QPoint,int&,int&);           //根据点击获取坐标值;
    void click(QPoint);                            //点击函数;
    int get_chessman_id(int,int);                  //根据坐标返回棋子的id值;
    int get_chessman_count(int, int, int, int);    //返回两点间直线上的棋子数，若不在同一直线则返回-1;                         
    virtual void click(int,int,int);                 //点击函数;

    void try_select_chessman(int);                   //选择棋子;
    void try_move_chessman(int,int,int);             //移动棋子;
    void save_step(int,int,int,int,QVector<step*>&); //保存走棋步骤;
    void kill_chessman(int);                         //吃子;
    void release_chessman(int);                      //棋子复活;
    void move_chessman(int,int,int);                 //移动棋子(根据移动棋子的id，和坐标轴)
    void move_chessman(int,int,int,int);             //移动棋子(根据移动棋子的id,坐标轴和被吃棋子的id)

    void back(step*step);                            //后退一步;
    void back_one();
    virtual void back();

    int get_radius(){return this->radius;}
    int get_select_id(){return this->select_id;}
    int get_which_turn(){return this->which_turn;}

    void change_turn();                             //交换顺序;

    bool game_win();                                //判断是否赢了;
    bool game_lose();                               //判断是否输了;

    void start_time();                        //开始计时;

    //走棋的规则;
    bool can_move(int,int,int,int);
    bool can_move_king(int,int,int,int);
    bool can_move_mandarin(int,int,int,int);
    bool can_move_elephant(int,int,int,int);
    bool can_move_rook(int,int,int,int);
    bool can_move_knight(int,int,int,int);
    bool can_move_cannon(int,int,int,int);
    bool can_move_pawn(int,int,int,int);

public slots:
    void slot_back();
    void time_out_slot();

private:
    int radius;        //棋子半径;
    int select_id;     //被选中的棋子序号;
    int which_turn;    //1代表轮到红方，2代表轮到黑方;
    bool be_side;
    QLabel *_sum_time;
    QTimer *timer;
    int sum_time;
};

#endif // CHESSBOARD_H
