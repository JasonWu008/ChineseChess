#ifndef START_WIDGET_H
#define START_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include "mygamewindow.h"

class start_widget : public QWidget
{
    Q_OBJECT
public:
    explicit start_widget(QWidget *parent = nullptr);

public slots:
    void start_game();
    void rule();
    void exit_game();

private:
    QPushButton _start;          //开始按钮;
    QPushButton _exit;           //退出按钮;
    QPushButton _rule;           //规则按钮;
    MyGameWindow w;

};

#endif // START_WIDGET_H
