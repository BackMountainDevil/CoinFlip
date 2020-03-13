#include "playscene.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"
#include <QTimer>
#include <QLabel>
#include <QSound>

PlayScene::PlayScene(int levelNum)
{
    qDebug() << "进入了第 "<<levelNum<<"关";
    this->levelIndex = levelNum;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    //this->setWindowTitle("翻金币场景");
this->setWindowTitle("金币连连看");
    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");
    //创建退出 菜单项
    QAction *  quitAction = startMenu->addAction("退出");
    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png" , ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width() , this->height() - backBtn->height());
    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=]()
    {
        qDebug() << "翻金币场景中：点击了返回按钮playscene";
        backSound->play();
        QTimer::singleShot(500,this,[=]()
        {
            emit this->chooseSceneBack();           
        });
    });

    //显示当前关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("Level: %1").arg(this->levelIndex);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText( str1);
    label->setGeometry(QRect( 30, this->height() - 50,140, 50));

//初始化游戏中的二维数据
    recoin();

    //创建金币的背景图片
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
           //绘制背景图片
            QLabel* bg = new QLabel(this);
            QPixmap pix;
            pix.load(":/res/BoardNode.png");
            bg->setGeometry(0,0,pix.width(),pix.height());
            bg->setPixmap(pix);
            bg->move(57 + i*50,200+j*50);

            //金币对象
            QString str;    //区分金银币
            if(this->gameArray[i][j] == 1)
            {
                str = ":/res/Coin0001.png";
            }
            else
            {
                str = ":/res/Coin0008.png";
            }

            MyCoin * coin = new MyCoin(str);
            coin->posX = i; //记录x坐标
            coin->posY = j; //记录y坐标
            coin->flag = this->gameArray[i][j]; //记录正反标志
            coin->setParent(this);
            coin->move(59 + i*50,204+j*50);
            coinBtn[i][j] = coin;

            connect(coin,&MyCoin::clicked,[=]()
            {
                //禁用所有金币按钮
                for(int i = 0 ; i < 4;i++)
                {
                    for(int j = 0 ; j < 4; j++)
                    {
                        coinBtn[i][j]->isWin = true;
                    }
                }
                //翻被点击的硬币
                flipSound->play();
                coin->changeFlag();
                gameArray[i][j] = gameArray [i][j] == 0?1:0;

                //延时翻zhouwei的硬币
                QTimer::singleShot(300, this,[=]()
                {
                    if(coin->posX+1 <=3)
                    {
                      coinBtn[coin->posX+1][coin->posY]->changeFlag();
                      gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posX-1>=0)
                    {
                      coinBtn[coin->posX-1][coin->posY]->changeFlag();
                      gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY+1<=3)
                    {
                     coinBtn[coin->posX][coin->posY+1]->changeFlag();
                     gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    if(coin->posY-1>=0)
                    {
                     coinBtn[coin->posX][coin->posY-1]->changeFlag();
                     gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                    }
                    this->isWin = true;
                    //释放所有金币按钮
                    for(int i = 0 ; i < 4;i++)
                    {
                        for(int j = 0 ; j < 4; j++)
                        {
                            coinBtn[i][j]->isWin = false;
                        }
                    }

                    //判断是否胜利
                    checkWin();
                });

            });
        }
    }

}

void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);

}

void PlayScene::recoin()
{
    dataConfig data;
    for(int i = 0; i < 4;i++)
    {
        for(int j = 0; j <4;j++)
        {
            this->gameArray[i][j] = data.mData[this->levelIndex][i][j];
        }
        qDebug()<<"";
    }
}



void  PlayScene::checkWin()
{
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++)
        {
            //qDebug() << coinBtn[i][j]->flag ;
            if( coinBtn[i][j]->flag == false)
            {
                this->isWin = false;
                break;
            }
        }
    }
    if(this->isWin)
   {
        //胜利按钮音效
        QSound *winSound =  new QSound(":/res/LevelWinSound.wav",this);
        winSound->play();

        //隐藏胜利图
        QLabel* winLabel = new QLabel;
        QPixmap tmpPix;
        tmpPix.load(":/res/LevelCompletedDialogBg.png");
        winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
        winLabel->setPixmap(tmpPix);
        winLabel->setParent(this);
        winLabel->show();
        winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());

        //显示胜利图
        QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel,"geometry");
        animation1->setDuration(1000);
        animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
        animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
        animation1->setEasingCurve(QEasingCurve::OutBounce);
        animation1->start();

            //前十九关延时自动进入到下一关游戏场景
        if(levelIndex<20)
        {
            QTimer::singleShot(1500,this,[=]()
            {
              this->hide(); //将场景隐藏掉

              play = new PlayScene(this->levelIndex+1); //创建游戏场景
              play->setGeometry(this->geometry());
              play->show();//显示游戏场景

              //返回按钮返回关卡选择
              connect(play,&PlayScene::chooseSceneBack,[=](){
                  this->setGeometry(play->geometry());
                  this->chooseSceneBack();
                  delete play;
                  play = NULL;
                   qDebug("1Back Test from playscene") ;
              });

            });
        }

   }
}
