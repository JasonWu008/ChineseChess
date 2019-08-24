#include "game.h"
#include <QTimer>
#include <QHBoxLayout>
#include <QMessageBox>

Game::Game()
{
    this->level=2;

    //给不同的棋子分配价值权重;
    map1.insert(Chessman::KING,10000);
    map1.insert(Chessman::MANDARIN,20);
    map1.insert(Chessman::ELEPHANT,20);
    map1.insert(Chessman::ROOK,90);
    map1.insert(Chessman::KNIGHT,40);
    map1.insert(Chessman::CANNON,45);
    map1.insert(Chessman::PAWN,10);

    //棋子被对方控制时对方所能获得的价值;
    map2.insert(Chessman::KING,10);
    map2.insert(Chessman::MANDARIN,5);
    map2.insert(Chessman::ELEPHANT,5);
    map2.insert(Chessman::ROOK,25);
    map2.insert(Chessman::KNIGHT,15);
    map2.insert(Chessman::CANNON,20);
    map2.insert(Chessman::PAWN,30);

}

Game::~Game(){}

void Game::click(int id,int x,int y)
{
    if(get_which_turn()==2)
    {
        return;
    }

    ChessBoard::click(id,x,y);

    if(get_which_turn()==2)
    {
        //延迟1秒后调用computer_move()函数来使电脑走棋;
        QTimer::singleShot(100,this,SLOT(computer_move()));
    }
}

//获取所有可能的走法;
void Game::get_all_move(QVector<step*>& temp_step)
{
    int left,right;
    if(this->get_which_turn()==1)
    {
        left=16;
        right=32;
    }
    else
    {
        left=0;
        right=16;
    }

    for(int i=left;i<right;++i)
    {
        if(!chessman[i].if_dead())
        {
            for(int j=0;j<9;++j)
            {
                for(int k=0;k<10;++k)
                {
                    int temp_id=get_chessman_id(j,k);
                    if(chessman[i].get_color()!=chessman[temp_id].get_color())
                    {
                        if(can_move(i,j,k,temp_id))
                        {
                            save_step(i,temp_id,j,k,temp_step);
                        }
                    }
                }
            }
        }
    }

}

//获取最佳的走法;
step* Game::get_best_move()
{
    QVector<step*> temp_step;
    get_all_move(temp_step);
    int max_of_min_score=-999999;
    step* result=NULL;

    while(temp_step.count())
    {
        step* step1=temp_step.last();
        temp_step.removeLast();
        try_one_move(step1);

        int min_score=get_min_value(level-1,max_of_min_score);
        back_one_move(step1);

        if(min_score>max_of_min_score)
        {
            max_of_min_score=min_score;
            if(result)
            {
                delete result;
            }
            result=step1;
        }
        else
        {
            delete step1;
        }
    }
    return result;
}

void Game::try_one_move(step* step)
{
    kill_chessman(step->get_kill_id());
    move_chessman(step->get_move_id(),step->get_x_to(),step->get_y_to());
}

void Game::back_one_move(step*step)
{
    release_chessman(step->get_kill_id());
    move_chessman(step->get_move_id(),step->get_x_from(),step->get_y_from());
}

int Game::evaluate_red()
{
    int value=0;
    //1.棋子价值权重;
    for(int i=0;i<32;++i)
    {
        if(chessman[i].get_color()==1&&!chessman[i].if_dead())
        {
            value+=map1[chessman[i].type];
        }
    }

    //2.棋子所能的控制敌方棋子;
    for(int i=16;i<32;++i)
    {
        if(!chessman[i].if_dead())
        {
            for(int j=0;j<16;++j)
            {
                if(!chessman[j].if_dead())
                {
                    if(can_move(chessman[i].get_id(),chessman[j].get_x(),chessman[j].get_y(),chessman[j].get_id()))
                    {
                        value+=map2[chessman[j].type];
                    }
                }
            }
        }
    }

    //3.己方受保护的程度;
    for(int i=16;i<32;++i)
    {
        for(int j=16;j<32;++j)
        {
            if(i!=j)
            {
                if(can_move(i,chessman[j].get_x(),chessman[j].get_y(),0))
                {
                    value+=map2[chessman[j].type];

                }
            }
        }
    }

    return value;
}

int Game::evaluate_black()
{
    int value=0;
    //1.棋子价值权重;
    for(int i=0;i<32;++i)
    {
        if(chessman[i].get_color()==2&&!chessman[i].if_dead())
        {
            value+=map1[chessman[i].type];
        }
    }

    //2.棋子所能的控制敌方棋子;
    for(int i=0;i<16;++i)
    {
        if(!chessman[i].if_dead())
        {
            for(int j=16;j<32;++j)
            {
                if(!chessman[j].if_dead())
                {
                    if(can_move(chessman[i].get_id(),chessman[j].get_x(),chessman[j].get_y(),chessman[j].get_id()))
                    {
                        value+=map2[chessman[j].type];
                    }
                }
            }
        }
    }

    //3.己方受保护的程度;
    for(int i=0;i<16;++i)
    {
        for(int j=0;j<16;++j)
        {
            if(i!=j)
            {
                if(can_move(i,chessman[j].get_x(),chessman[j].get_y(),16))
                {
                    value+=map2[chessman[j].type];

                }
            }
        }
    }

    return value;
}

int Game::evaluate()
{
    return evaluate_black()-evaluate_red();
}

int Game::get_min_value(int temp_level,int current_min)
{
    if(temp_level==0)
    {
        return evaluate();
    }
    QVector<step*> temp_step;
    get_all_move(temp_step);
    int min_of_max=999999;

    while(temp_step.size()>0)
    {
        step*step1=temp_step.last();
        temp_step.removeLast();
        try_one_move(step1);
        int max_value=get_max_value(temp_level-1,min_of_max);
        back_one_move(step1);
        delete step1;

        if(max_value<=current_min)            //找到最小value;
        {
            while(temp_step.size()>0)         //释放内存;
            {
                step*step1=temp_step.last();
                temp_step.removeLast();
                delete step1;
            }

            return max_value;
        }

        if(max_value<min_of_max)
        {
            min_of_max=max_value;
        }
    }
    return min_of_max;
}

int Game::get_max_value(int temp_level,int current_max)
{
    if(temp_level==0)
    {
        return evaluate();
    }
    QVector<step*> temp_step;
    get_all_move(temp_step);
    int max_of_min=-999999;

    while(temp_step.size()>0)
    {
        step*step1=temp_step.last();
        temp_step.removeLast();
        try_one_move(step1);
        int min_value=get_min_value(temp_level-1,max_of_min);
        back_one_move(step1);
        delete step1;

        if(min_value>=current_max)            //找到最大value;
        {
            while(temp_step.size()>0)         //释放内存;
            {
                step*step1=temp_step.last();
                temp_step.removeLast();
                delete step1;
            }

            return min_value;
        }

        if(min_value>max_of_min)
        {
            max_of_min=min_value;
        }
    }
    return max_of_min;
}

void Game::back()
{
    if(get_which_turn()==1)
    {
        back_one();
        back_one();
    }
}

void Game::retract_game()
{
    if(retract.size()==0)
    {
        QMessageBox::about(this,"提示","你还没有走棋");
        return;
    }
    step* step_temp1=retract.last();
    retract.removeLast();
    release_chessman(step_temp1->get_kill_id());
    chessman[step_temp1->get_move_id()].change_x(step_temp1->get_x_from());
    chessman[step_temp1->get_move_id()].change_y(step_temp1->get_y_from());
    delete step_temp1;
    step* step_temp2=retract.last();
    retract.removeLast();
    release_chessman(step_temp2->get_kill_id());
    chessman[step_temp2->get_move_id()].change_x(step_temp2->get_x_from());
    chessman[step_temp2->get_move_id()].change_y(step_temp2->get_y_from());
    delete step_temp2;
    update();
}

void Game::init_level(int a)
{
    this->level=a;
}

void Game::computer_move()
{
    step* step=get_best_move();
    retract.push_back(step);
    move_chessman(step->get_move_id(),step->get_kill_id(),step->get_x_to(),step->get_y_to());
    update();
}

