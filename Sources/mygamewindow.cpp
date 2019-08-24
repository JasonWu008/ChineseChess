#include "mygamewindow.h"
#include <QMessageBox>

MyGameWindow::MyGameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 菜单栏
        _pMenuBar = menuBar();
        _gameMenu = _pMenuBar->addMenu("&游戏");

        //悔棋
        //QMenu * backAction = _gameMenu->addMenu("悔棋");
        _gameMenu->addAction("&悔棋", this, SLOT(slotBack()));
        //_gameMenu->addMenu(backAction);

        //选择难度
        QMenu * chooseLevelMenu = _gameMenu->addMenu("选择难度");
        chooseLevelMenu->addAction("&简单", this, SLOT(slotChooseLevel1()));
        chooseLevelMenu->addAction("&一般", this, SLOT(slotChooseLevel2()));
        chooseLevelMenu->addAction("&困难", this, SLOT(slotChooseLevel3()));
        _gameMenu->addMenu(chooseLevelMenu);

        //退出游戏
        _gameMenu->addSeparator();
        _gameMenu->addAction("&退出", this, SLOT(close_widget()));

        _helpMenu = _pMenuBar->addMenu("&帮助");
        _helpMenu->addAction("&游戏说明", this, SLOT(slotRule()));
        _helpMenu->addAction("&关于", this, SLOT(slotAbout()));

        /* 游戏界面 */
        _game = new Game;
    this->setCentralWidget(_game);
}

MyGameWindow::~MyGameWindow()
{

}

void MyGameWindow::slotBack()
{
    _game->retract_game();
}

void MyGameWindow::slotChooseLevel1()
{
    _game->init_level(_game->get_max_depth()-2);
}

void MyGameWindow::slotChooseLevel2()
{
    _game->init_level(_game->get_max_depth()-1);
}

void MyGameWindow::slotChooseLevel3()
{
    _game->init_level(_game->get_max_depth()-1);
}

void MyGameWindow::slotRule()
{
    QMessageBox::about(this,"走棋规则","将军不离九宫内，士止相随不出官。\n"
                                      "象飞四方营四角，马行一步一尖冲。\n"
                                      "炮须隔子打一子，车行直路任西东。\n"
                                      "唯卒只能行一步，过河横进退无踪。\n");
}

void MyGameWindow::slotAbout()
{
    QMessageBox::about(this, "关于", "中国象棋 软件1班 吴宇鹏");
}

void MyGameWindow::close_widget()
{
    if(QMessageBox::Yes==QMessageBox::question(this,"提示：","你想要离开游戏吗?",QMessageBox::Yes|QMessageBox::No))
    {
            exit(0);
    }
}
