#ifndef MYGAMEWINDOW_H
#define MYGAMEWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include "game.h"

class MyGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyGameWindow(QWidget *parent = 0);
    ~MyGameWindow();

    Game * _game;
    QMenuBar * _pMenuBar;
    QMenu * _gameMenu;
    QMenu * _helpMenu;

public slots:
    void slotBack();
    void slotChooseLevel1();
    void slotChooseLevel2();
    void slotChooseLevel3();
    void slotRule();
    void slotAbout();
    void close_widget();
};

#endif // MYGAMEWINDOW_H

