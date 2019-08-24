#ifndef STEP_H
#define STEP_H

#include <QObject>

class step : public QObject
{
    Q_OBJECT
public:
    explicit step(QObject *parent = nullptr);
    ~step();

    int get_move_id(){return this->move_id;}
    int get_kill_id(){return this->kill_id;}
    int get_x_from(){return this->x_from;}
    int get_y_from(){return this->y_from;}
    int get_x_to(){return this->x_to;}
    int get_y_to(){return this->y_to;}

    void change_x_from(int);
    void change_x_to(int);
    void change_y_from(int);
    void change_y_to(int);
    void change_move_id(int);
    void change_kill_id(int);

private:
    int move_id;           //移动棋子id;
    int kill_id;           //被吃棋子id;
    int x_from;            //初始位置x坐标;
    int y_from;            //初始位置y坐标;
    int x_to;              //移动位置x坐标;
    int y_to;              //移动位置y坐标;
};

#endif // STEP_H
