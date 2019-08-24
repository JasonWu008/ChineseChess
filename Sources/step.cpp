#include "step.h"

step::step(QObject *parent) : QObject(parent)
{
    move_id=-1;
    kill_id=-1;
    x_from=-1;
    y_from=-1;
    x_to=-1;
    y_to=-1;
}

step::~step()
{

}

void step::change_x_from(int temp)
{
    this->x_from=temp;
}

void step::change_x_to(int temp)
{
    this->x_to=temp;
}

void step::change_y_from(int temp)
{
    this->y_from=temp;
}

void step::change_y_to(int temp)
{
    this->y_to=temp;
}

void step::change_move_id(int temp)
{
    this->move_id=temp;
}

void step::change_kill_id(int temp)
{
    this->kill_id=temp;
}

