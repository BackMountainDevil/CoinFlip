#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"


class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    PlayScene(int levelNum);
    //重写paintEvent事件
    void paintEvent(QPaintEvent *);
    //重置金币正反面到原始状态
    void recoin();
    void checkWin();


    int levelIndex ; //内部成员属性 记录所选的关卡
    bool isWin = true; //是否胜利
    int gameArray[4][4]; //二维数组数据
    MyCoin * coinBtn[4][4];
    PlayScene * play = NULL;


signals:
    void chooseSceneBack();

};

#endif // PLAYSCENE_H
