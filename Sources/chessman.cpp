#include "Chessman.h"
#include <QString>

Chessman::Chessman()
{

}

void Chessman::initialize(int id)
{
    this->ID=id;
    this->dead=false;
    this->color=(id>15)?1:2;

    if(id<16)
    {
        x=chess_piece[id].x;
        y=chess_piece[id].y;
        type=chess_piece[id].type;
    }
    else
    {
        x = 8-chess_piece[id-16].x;
        y = 9-chess_piece[id-16].y;
        type = chess_piece[id-16].type;
    }
}

void Chessman::rotate()
{
    x=8-x;
    y=9-y;
}

QString Chessman::get_text()
{
    switch (this->type)
    {
        case KING:
            return color==1?"帅":"将";

        case MANDARIN:
            return "士";
        case ELEPHANT:
            return color==1?"相":"象";
        case ROOK:
            return "车";
        case KNIGHT:
            return "马";
        case CANNON:
            return "炮";
        case PAWN:
            return color==1?"兵":"卒";
    }
    return "出现错误";
}

int Chessman::get_color()
{
    return this->color;
}

int Chessman::get_x()
{
    return this->x;
}

int Chessman::get_y()
{
    return this->y;
}

 bool Chessman::if_dead()
 {
     return this->dead;
 }

 void Chessman::turn_dead_to_true()
 {
     this->dead=true;
 }

 void Chessman::turn_dead_to_false()
 {
     this->dead=false;
 }

 void Chessman::change_x(int temp)
 {
     this->x=temp;
 }

 void Chessman::change_y(int temp)
 {
     this->y=temp;
 }

 int Chessman::get_id()
 {
     return this->ID;
 }
