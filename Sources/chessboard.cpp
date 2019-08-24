#include "ChessBoard.h"
#include "Chessman.h"
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QString>
#include <QTimer>

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent)
{
    this->radius=30;
    init(true);

    _sum_time=new QLabel("局时: 00:00:00",this);
    _sum_time->setGeometry(600,300,150,40);
    QFont ft;
    ft.setPointSize(12);
    _sum_time->setFont(ft);
    timer =new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(time_out_slot()));
}

void ChessBoard::init(bool k)
{
    for(int i=0;i<32;i++)
    {
        chessman[i].initialize(i);
    }

    if(!k)
    {
        for(int i=0;i<32;i++)
        {
            chessman[i].rotate();
        }
    }

    select_id=-1;
    which_turn=1;
    be_side=k;
    sum_time=0;
}

QSize ChessBoard::sizeHint() const      //设置窗口默认大小;
{
    return QSize(28*this->radius,22*this->radius);
}

//绘图函数;
void ChessBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int length=2*this->radius;

    QPen pen(Qt::black,2);
    painter.setPen(pen);

    //绘制横线;
    for(int i=1;i<=10;i++)
    {
        painter.drawLine(QPoint(length,length*i),QPoint(length*9,length*i));
    }

    //绘制竖线,中间需要空出“楚河汉界”四个字的位置;
    for(int i=1;i<=9;i++)
    {
        if(i==1||i==9)
        {
            painter.drawLine(QPoint(length*i,length),QPoint(length*i,length*10));
        }
        else
        {
            painter.drawLine(QPoint(length*i,length),QPoint(length*i,length*5));
            painter.drawLine(QPoint(length*i,length*6),QPoint(length*i,length*10));
        }
    }

    //绘制将所在范围的斜线;
    painter.drawLine(QPoint(length*4,length),QPoint(length*6,length*3));
    painter.drawLine(QPoint(length*4,length*3),QPoint(length*6,length));
    painter.drawLine(QPoint(length*4,length*8),QPoint(length*6,length*10));
    painter.drawLine(QPoint(length*4,length*10),QPoint(length*6,length*8));

    //“楚河汉界”;
    QFont font("华文隶书",26);                       //设置字体和大小;
    painter.setFont(font);
    painter.drawText(2*length,5.7*length,"   楚河         汉界");

    //绘制棋子;
    for(int i=0;i<32;i++)
    {
        draw_chessman(painter,i);
    }

    if(game_win())
    {
        QMessageBox::information(this,"将军","你赢了!  ");
        this->init(true);
        return;
    }

    //判断游戏是否结束;
    if(game_lose())
    {
        QMessageBox::information(this,"将军","你输了!  " );
        this->init(true);
        return;
    }
}

//将棋子在棋盘中的坐标转换成窗口的实际坐标;
QPoint ChessBoard::center(int x,int y)
{
    QPoint temp;
    temp.rx()=(x+1)*2*this->get_radius();                     //QPoint类rx()和ry()函数返回x,y的引用;
    temp.ry()=(y+1)*2*this->get_radius();
    return temp;
}

//根据棋子id将棋盘中的坐标转换成窗口实际坐标;
QPoint ChessBoard::center(int a)
{
    return ChessBoard::center(chessman[a].get_x(),chessman[a].get_y());
}

//将窗口中的坐标转换成棋盘的坐标;
QPoint ChessBoard::windows_center(int x,int y)
{
    QPoint temp;
    temp.rx()=(x-1)*2*get_radius();
    temp.ry()=(y-1)*2*get_radius();
    return temp;
}

//根据id绘制棋子;
void ChessBoard::draw_chessman(QPainter& painter,int id)
{
    if(chessman[id].if_dead())                    //判断棋子是否已被吃;
        return;

    QPoint temp=center(chessman[id].get_x(),chessman[id].get_y());
    QRect rect=QRect(temp.x()-get_radius(),temp.y()-get_radius(),2*get_radius(),2*get_radius());

    if(id==get_select_id())
    {
        painter.setBrush(Qt::gray);               //被选中的棋子设为灰色;
    }
    else
    {
        painter.setBrush(QColor(255,215,0));      //设置底色为黄色;
    }

    if(chessman[id].get_color()==1)               //棋子为红色;
    {
        painter.setPen(QPen(Qt::red,2));
        painter.drawEllipse(center(id),get_radius(),get_radius());                  //绘制圆形;
        painter.drawText(rect,chessman[id].get_text(),QTextOption(Qt::AlignCenter));//居中;
        painter.setPen(QPen(Qt::black,2));
    }
    else                                          //棋子为黑色;
    {
        painter.setPen(QPen(Qt::black,2));
        painter.drawEllipse(center(id),get_radius(),get_radius());                  //绘制圆形;
        painter.drawText(rect,chessman[id].get_text(),QTextOption(Qt::AlignCenter));//居中;
    }
}

//鼠标释放函数;
void ChessBoard::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint point=event->pos();

    click(point);
}

bool ChessBoard::get_click_xy(QPoint point,int& x,int& y)
{
    for(int i=0;i<=8;i++)
    {
        for(int j=0;j<=9;j++)
        {
            QPoint temp=center(i,j);
            int distance=(point.x()-temp.x())*(point.x()-temp.x())+(point.y()-temp.y())*(point.y()-temp.y());
            if(distance<(this->get_radius()*this->get_radius()))
            {
                x=i;
                y=j;
                return true;
            }
        }
    }
    return false;
}

//根据坐标得到相应的id;
int ChessBoard::get_chessman_id(int x,int y)
{
    for(int i=0;i<32;i++)
    {
        if(chessman[i].get_x()==x&&chessman[i].get_y()==y&&!chessman[i].if_dead())
            return i;
    }
    return -1;
}

//获取两点间直线上的棋子数;
int ChessBoard::get_chessman_count(int x1, int y1, int x2, int y2)
{
    if((x1!=x2&&y1!=y2)||(x1==x2&&y1==y2))
        return -1;

    int count=0;
    if(x1==x2)
    {
        int left=y1<y2?y1:y2;
        int right=y1>y2?y1:y2;
        for(int i=left+1;i<right;i++)
        {
            if(get_chessman_id(x1,i)>=0)
                count++;
        }
    }
    else
    {
        int left=x1<x2?x1:x2;
        int right=x1>x2?x1:x2;
        for(int i=left+1;i<right;i++)
        {
            if(get_chessman_id(i,y1)>=0)
                count++;
        }
    }

    return count;
}

void ChessBoard::click(QPoint point)
{
    int x,y;

    if(!get_click_xy(point,x,y))
        return;

    int id=get_chessman_id(x,y);
    click(id,x,y);
}

void ChessBoard::click(int click_id,int x,int y)
{
    if(this->get_select_id()==-1)             //若之前没有棋子被选中;
    {
        try_select_chessman(click_id);
    }
    else
    {
        try_move_chessman(click_id,x,y);
    }
}

//选择棋子;
void ChessBoard::try_select_chessman(int id)
{
    if(id==-1)
    {
        return;
    }

    if(this->get_which_turn()==chessman[id].get_color())
    {
        select_id=id;
        update();
    }

}

//移动棋子;
void ChessBoard::try_move_chessman(int kill_id,int x,int y)
{
    if(can_move(get_select_id(),x,y,kill_id))
    {
        step* step_temp=new step;
        step_temp->change_x_from(chessman[select_id].get_x());
        step_temp->change_x_to(x);
        step_temp->change_y_from(chessman[select_id].get_y());
        step_temp->change_y_to(y);
        step_temp->change_move_id(select_id
                                  );
        step_temp->change_kill_id(kill_id);
        retract.push_back(step_temp);

        chessman[select_id].change_x(x);
        chessman[select_id].change_y(y);

        if(kill_id>=0)
        {
            chessman[kill_id].turn_dead_to_true();
        }
        else
        {
            try_select_chessman(kill_id);
        }

        update();

        select_id=-1;

        //交换下棋方;
        if(which_turn==1) which_turn=2;
        else  which_turn=1;

    }
}

//保存走棋步骤;
void ChessBoard::save_step(int move_id,int kill_id,int x,int y,QVector<step*>& step2)
{
    int x_temp=chessman[move_id].get_x();
    int y_temp=chessman[move_id].get_y();

    //记录棋盘走棋的步骤;
    step* step_temp=new step;
    step_temp->change_x_from(x_temp);
    step_temp->change_x_to(x);
    step_temp->change_y_from(y_temp);
    step_temp->change_y_to(y);
    step_temp->change_move_id(move_id);
    step_temp->change_kill_id(kill_id);

    step2.append(step_temp);
}

//棋子被吃;
void ChessBoard::kill_chessman(int id)
{
    if(id==-1)
    {
        return;
    }
    chessman[id].turn_dead_to_true();
}

//棋子复活;
void ChessBoard::release_chessman(int id)
{
    if(id==-1)
    {
        return;
    }
    chessman[id].turn_dead_to_false();
}

//移动棋子;
void ChessBoard::move_chessman(int move_id,int x,int y)
{
    chessman[move_id].change_x(x);
    chessman[move_id].change_y(y);

    change_turn();
}

//移动棋子;
void ChessBoard::move_chessman(int move_id,int kill_id,int x,int y)
{
    save_step(move_id,kill_id,x,y,steps);
    kill_chessman(kill_id);
    move_chessman(move_id,x,y);
}

void ChessBoard::back(step *step)
{
    if(step->get_kill_id()==-1)
    {
        return;
    }

    chessman[step->get_move_id()].change_x(step->get_x_from());
    chessman[step->get_move_id()].change_y(step->get_y_from());

    change_turn();
}

void ChessBoard::back_one()
{
    if(this->steps.size()==0)
    {
        return;
    }

    step*step_temp=this->steps.last();
    this->steps.removeLast();
    back(step_temp);

    update();
    delete step_temp;
}

void ChessBoard::back()
{
    back_one();
}

void ChessBoard::change_turn()
{
    if(this->which_turn==1) this->which_turn=2;
    else  this->which_turn=1;
}

bool ChessBoard::can_move(int move_id,int x,int y,int kill_id)
{
    if(chessman[move_id].get_color()==chessman[kill_id].get_color())     //如果两次都选择相同方棋子，则更新被选对象;
    {
        select_id=kill_id;
        update();
        return false;
    }

    switch(chessman[move_id].type)
    {
    case Chessman::KING:
        return can_move_king(move_id,x,y,kill_id);
        break;

    case Chessman::MANDARIN:
        return can_move_mandarin(move_id,x,y,kill_id);
        break;

    case Chessman::ELEPHANT:
        return can_move_elephant(move_id,x,y,kill_id);
        break;

    case Chessman::ROOK:
        return can_move_rook(move_id,x,y,kill_id);
        break;

    case Chessman::KNIGHT:
        return can_move_knight(move_id,x,y,kill_id);
        break;

    case Chessman::CANNON:
        return can_move_cannon(move_id,x,y,kill_id);
        break;

    case Chessman::PAWN:
        return can_move_pawn(move_id,x,y,kill_id);
        break;
    }
    return true;
}

bool ChessBoard::can_move_king(int move_id,int x,int y,int kill_id)
{
    if(chessman[kill_id].type==Chessman::KING)                                 //两将在一条直线且中间没有棋子则可将军;
    {
        if(get_chessman_count(chessman[move_id].get_x(),chessman[move_id].get_y(),chessman[kill_id].get_x(),chessman[kill_id].get_y())==0)
        {
            return true;
        }
    }

    if(x<3||x>5)
    {
        return false;
    }

    if(chessman[move_id].get_color()==1)
    {
        if(y<7)
        {
            return false;
        }
    }
    else
    {
        if(y>2)
        {
            return false;
        }
    }

    int k=abs(x-chessman[move_id].get_x())+abs(y-chessman[move_id].get_y());
    if(k==1)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool ChessBoard::can_move_mandarin(int move_id,int x,int y,int)
{
    if(x<3||x>5)
    {
        return false;
    }

    if(chessman[move_id].get_color()==1)
    {
        if(y<7)
        {
            return false;
        }
    }
    else
    {
        if(y>2)
        {
            return false;
        }
    }

    int k=abs(x-chessman[move_id].get_x())*10+abs(y-chessman[move_id].get_y());

    if(k==11)                       //横纵坐标各移动一格;
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool ChessBoard::can_move_elephant(int move_id,int x,int y,int)
{
    if(x<0||x>8)
    {
        return false;
    }

    if(chessman[move_id].get_color()==1)
    {
        if(y<5)
        {
            return false;
        }
    }
    else
    {
        if(y>4)
        {
            return false;
        }
    }

    int k=abs(x-chessman[move_id].get_x())*10+abs(y-chessman[move_id].get_y());

    if(k==22)                     //横纵坐标各移动两格;
    {
        if(get_chessman_id((chessman[move_id].get_x()+x)/2,(chessman[move_id].get_y()+y)/2)>=0)
        {
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool ChessBoard::can_move_rook(int move_id,int x,int y,int)
{
    if(get_chessman_count(chessman[move_id].get_x(),chessman[move_id].get_y(),x,y)==0)
        return true;
    else
        return false;
}

bool ChessBoard::can_move_knight(int move_id,int x,int y,int)
{
    int k=abs(chessman[move_id].get_x()-x)*10+abs(chessman[move_id].get_y()-y);

    if(k!=21&&k!=12)
        return false;

    if(k==12)
    {
        if(get_chessman_id(chessman[move_id].get_x(),(chessman[move_id].get_y()+y)/2)>=0)
            return false;
    }
    else
    {
        if(get_chessman_id((chessman[move_id].get_x()+x)/2,chessman[move_id].get_y())>=0)
            return false;
    }
    return true;
}

bool ChessBoard::can_move_cannon(int move_id,int x,int y,int kill_id)
{
    int count=get_chessman_count(chessman[move_id].get_x(),chessman[move_id].get_y(),x,y);
    if(kill_id>=0)
    {
        if(count==1)
            return true;
    }
    else
    {
        if(count==0)
            return true;
    }
    return false;
}

bool ChessBoard::can_move_pawn(int move_id,int x,int y,int)
{
    int k=abs(chessman[move_id].get_x()-x)+abs(chessman[move_id].get_y()-y);
    if(k!=1)
        return false;

    if(chessman[move_id].get_color()==1)         //红方棋子;
    {
        if(chessman[move_id].get_y()>4)          //未过界;
        {
            if(chessman[move_id].get_y()<=y)
            {
                return false;
            }
        }
        else                                     //过界;
        {
            if(chessman[move_id].get_y()<y)
            {
                return false;
            }
        }
    }
    else                                         //黑方棋子;
    {
        if(chessman[move_id].get_y()<5)          //未过界;
        {
            if(chessman[move_id].get_y()>=y)
            {
                return false;
            }
        }
        else                                     //过界;
        {
            if(chessman[move_id].get_y()>y)
            {
                return false;
            }
        }
    }

    return true;
}

bool ChessBoard::game_win()
{
    if(chessman[4].if_dead())
    {
        return true;
    }
    return false;
}

bool ChessBoard::game_lose()
{
    if(chessman[20].if_dead())
    {
        return true;
    }
    return false;
}\

void ChessBoard::start_time()
{
    timer->start(1000);
}


void ChessBoard::slot_back()
{
    back();
}

void ChessBoard::time_out_slot()
{
    sum_time++;
    QString sumTime="局时："+QString("%1").arg(sum_time/3600%60, 2, 10,QChar('0'))
            +":"+QString("%2").arg(sum_time/60%60, 2, 10, QChar('0'))
            +":"+QString("%3").arg(sum_time%60, 2, 10, QChar('0'));
    _sum_time->setText(sumTime);
}


