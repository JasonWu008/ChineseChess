#include "start_widget.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>

start_widget::start_widget(QWidget *parent) : QWidget(parent)
{
    _start.setParent(this);
    _start.setText("开始游戏");
    _start.move(80,100);
    _start.setFixedSize(QSize(500,60));
    _start.setFont(QFont("Times", 18, QFont::Bold));
    _start.setFlat(1);

    _rule.setParent(this);
    _rule.setText("游戏规则");
    _rule.move(80,250);
    _rule.setFixedSize(QSize(500,60));
    _rule.setFont(QFont("Times", 18, QFont::Bold));
    _rule.setFlat(1);


    _exit.setParent(this);
    _exit.setText("退出游戏");
    _exit.move(80,400);
    _exit.setFixedSize(QSize(500,60));
    _exit.setFont(QFont("Times", 18, QFont::Bold));
    _exit.setFlat(1);

    connect(&_start,&QPushButton::released,this,&start_widget::start_game);
    connect(&_rule,&QPushButton::released,this,&start_widget::rule);
    connect(&_exit,&QPushButton::released,this,&start_widget::exit_game);
}

void start_widget::start_game()
{
    w.show();
    w._game->start_time();
    this->hide();
}

//使用文件的操作将规则读入;
void start_widget::rule()
{
    QFile f("D:\\Qt5.9\\Application\\Chinese_Chess\\Rule.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::about(this,"警告","文件读取失败");
    }
    QTextStream txtInput(&f);
    QString lineStr;
    QString temp;
    while(!txtInput.atEnd())
    {
        temp = txtInput.readLine();
        lineStr+=temp;
        lineStr+="\n";
    }
    f.close();
    QMessageBox::about(this,"走棋规则",lineStr);
}

void start_widget::exit_game()
{
    this->close();
}
