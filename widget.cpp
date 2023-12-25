#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QMenu>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->volumeSlider->setRange(0,100);
    this->setWindowTitle(tr("音乐播放器"));
    this->playList=new QMediaPlaylist(this);
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    this->player=new QMediaPlayer(this);
    player->setPlaylist(playList);
    player->setVolume(50);
    ui->volumeSlider->setValue(50);


    ui->songsList->setStyleSheet("background-color:transparent;color: white;");
    ui->addSongsButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->addSongsButton->setFlat(true);
    ui->clearButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->clearButton->setFlat(true);
    ui->delectButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->delectButton->setFlat(true);
    ui->volumeButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->volumeButton->setText(" ");
    ui->volumeButton->setFlat(true);

    connect(ui->volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(setVol(int)));
    connect(ui->addSongsButton,SIGNAL(clicked()),this,SLOT(addSongs()));
    connect(ui->playPauseButton,SIGNAL(clicked()),this,SLOT(playPause()));
    connect(ui->previousButton,SIGNAL(clicked()),this,SLOT(playPrevious()));
    connect(ui->nextButton,SIGNAL(clicked()),this,SLOT(playNext()));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(durChanged(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(posChanged(qint64)));
    connect(ui->processSlider,SIGNAL(valueChanged(int)),this,SLOT(setPos(int)));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(clearList()));
    connect(ui->delectButton,SIGNAL(clicked()),this,SLOT(delectSong()));
    connect(ui->volumeButton,SIGNAL(clicked()),this,SLOT(volumeSilence()));

    ui->playPauseButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->playPauseButton->setFlat(true);
    ui->playPauseButton->setText(" ");
    ui->previousButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->previousButton->setFlat(true);
    ui->nextButton->setStyleSheet("QPushButton{background: transparent;}");
    ui->nextButton->setFlat(true);

    action_hideShow=new QAction(tr("隐藏/显示窗口部件"),this);
    action_addSongs=new QAction(tr("添加本地歌曲"),this);
    action_playPause=new QAction(tr("播放/暂停"),this);
    action_stop=new QAction(tr("停止"),this);
    action_previous=new QAction(tr("上一曲"),this);
    action_next=new QAction(tr("下一曲"),this);
    action_addVolume=new QAction(tr("音量+(5%)"),this);
    action_redVolume=new QAction(tr("音量-(5%)"),this);
    action_quit=new QAction(tr("退出"),this);
    connect(action_hideShow,SIGNAL(triggered()),this,SLOT(hideShow()));
    connect(action_addSongs,SIGNAL(triggered()),this,SLOT(addSongs()));
    connect(action_playPause,SIGNAL(triggered()),this,SLOT(playPause()));
    connect(action_previous,SIGNAL(triggered()),this,SLOT(playPrevious()));
    connect(action_next,SIGNAL(triggered()),this,SLOT(playNext()));
    connect(action_addVolume,SIGNAL(triggered()),this,SLOT(addVolume()));
    connect(action_redVolume,SIGNAL(triggered()),this,SLOT(redVolume()));
    connect(action_quit,SIGNAL(triggered()),this,SLOT(close()));

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_3->setAlignment(Qt::AlignCenter);
}
Widget::~Widget()
{
    delete ui;
}
//每当音量进度条改变，会触发这个槽函数重新设置音量大小
void Widget::setVol(int vol)
{
    player->setVolume(vol);
}

//选择要添加的所有歌曲，添加到QStringList中，遍历QStringList中的所有.MP3文件
//添加到playList中，并且添加到ui界面的QlistWight中
void Widget::addSongs()
{
    QStringList songsList=QFileDialog::getOpenFileNames(this,"选择文件",QDir::homePath(),"*.mp3");
    for(int i=0;i<songsList.size();i++)
    {
        QString song=songsList.at(i);
        qDebug()<<song;
         //歌词存放路径
        playPath =song.left(song.lastIndexOf("/"));
        qDebug()<<playPath;
        this->playList->addMedia(QUrl::fromLocalFile(song));
        QStringList songL=song.split('/');
        song=songL.last();
        ui->songsList->addItem(song);
    }
}

//播放/暂停
void Widget::playPause()
{
      ui->processSlider->setEnabled(true);
    if(" "==ui->playPauseButton->text())
    {
        int index=ui->songsList->currentRow();

        if(-1!=index)
        {
            //歌曲名字
            QString songname =ui->songsList->item(index)->text().left(ui->songsList->item(index)->text().lastIndexOf("."));
            playList->setCurrentIndex(index);
            player->play();
            ui->playPauseButton->setText("\n");
            ui->label_background->setStyleSheet("background-image: url(:/pic/musicup.jpg);");
            //            读取歌词文件
            if(!lyrics.readLyricsFile(playPath+"/"+songname+".lrc")){
                ui->label->setText("   未检测到歌词文件");
                ui->label_2->setText("请检查歌词文件是否存在");
            }
            lyricsID = 0;

        }

    }
    else
    {
        player->pause();
        ui->playPauseButton->setText(" ");
        ui->label_background->setStyleSheet("background-image: url(:/pic/musicstop.jpg);");
        //    清空歌词
        //        ui->label->clear();
        //        ui->label_2->clear();

        lyrics.listLyricsText.clear();
    }
}
//播放上一曲
void Widget::playPrevious()
{
    player->stop();
    int index=playList->currentIndex();
    if(0==index)
        index=playList->mediaCount()-1;
    else
        index--;
    playList->setCurrentIndex(index);
    ui->songsList->setCurrentRow(index);
    ui->playPauseButton->setText(" ");
    ui->label_background->setStyleSheet("background-image: url(:/pic/musicup.jpg);");
//    清空歌词
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
    lyrics.listLyricsText.clear();
}
//播放下一曲
void Widget::playNext()
{
    player->stop();
    int index=playList->currentIndex();
    if(index==playList->mediaCount()-1)
        index=0;
    else
        index++;
    playList->setCurrentIndex(index);
    ui->songsList->setCurrentRow(index);
    ui->playPauseButton->setText(" ");
    ui->label_background->setStyleSheet("background-image: url(:/pic/musicup.jpg);");
//    清空歌词
    ui->label->clear();
    ui->label_2->clear();
    ui->label_3->clear();
   lyrics.listLyricsText.clear();
}
//设置进度条显示范围，并计算歌曲总时间，显示出来
void Widget::durChanged(qint64 dur)
{
    ui->processSlider->setRange(0,dur);
    totalTime.clear();
    dur/=1000;
    totalTime+=QString::number(dur/60);
    totalTime+=':';
    if(dur%60<10)
        totalTime+='0';
    totalTime+=QString::number(dur%60);
    ui->totalTimeLabel->setText(totalTime);
}
//设置进度条的值，并计算当前时间，显示出来
void Widget::posChanged(qint64 pos)
{
    presentTime.clear();
    ui->processSlider->setValue(pos);
    pos/=1000;
    presentTime+=QString::number(pos/60);
    presentTime+=':';
    if(pos%60<10)
        presentTime+='0';
    presentTime+=QString::number(pos%60);
    ui->presentTimeLabel->setText(presentTime);
    if(presentTime==totalTime)
    {
        presentTime.clear();
        player->stop();
        ui->label->clear();
        ui->label_2->clear();
        ui->label_3->clear();

    }

}
void Widget::setPos(int pos)
{
    setPlayTime();

}

//清空播放列表
void Widget::clearList()
{
    playList->clear();
    ui->songsList->clear();
}
//在播放列表中删除某首歌曲
void Widget::delectSong()
{
    int index=ui->songsList->currentRow();
    if(-1!=index)
    {
        ui->songsList->takeItem(index);
        playList->removeMedia(index);
    }
}
//设置有音量和静音
void Widget::volumeSilence()
{
    if(ui->volumeButton->text()==" ")
    {
        ui->volumeButton->setText("\n");
        ui->volumeSlider->setValue(0);
        ui->label_volume->setStyleSheet("background-image: url(:/pic/_09.png);");

    }
    else
    {
        ui->volumeButton->setText(" ");
        ui->label_volume->setStyleSheet("background-image: url(:/pic/music_04.png);");
        ui->volumeSlider->setValue(50);
    }

}
/*鼠标右击事件
 *每次鼠标右击都会显示菜单*/
void Widget::contextMenuEvent(QContextMenuEvent*)
{
    QMenu* menu=new QMenu(this);

    menu->addAction(action_hideShow);
    menu->addAction(action_addSongs);
    menu->addSeparator();
    menu->addAction(action_playPause);
    menu->addAction(action_stop);
    menu->addAction(action_previous);
    menu->addAction(action_next);
    menu->addSeparator();
    menu->addAction(action_addVolume);
    menu->addAction(action_redVolume);
    menu->addSeparator();
    menu->addAction(action_quit);

    menu->move(cursor().pos());
    menu->show();
}
//控件隐藏/显示
void Widget::hideShow()
{
    if(ui->songsList->isVisible())
    {
        ui->addSongsButton->setVisible(false);
        ui->clearButton->setVisible(false);
        ui->delectButton->setVisible(false);
        ui->songsList->setVisible(false);
        ui->processSlider->setVisible(false);
        ui->presentTimeLabel->setVisible(false);
        ui->totalTimeLabel->setVisible(false);
        ui->volumeButton->setVisible(false);
        ui->volumeSlider->setVisible(false);

    }
    else
    {
        ui->addSongsButton->setVisible(true);
        ui->clearButton->setVisible(true);
        ui->delectButton->setVisible(true);
        ui->songsList->setVisible(true);
        ui->processSlider->setVisible(true);
        ui->presentTimeLabel->setVisible(true);
        ui->totalTimeLabel->setVisible(true);
        ui->volumeButton->setVisible(true);
        ui->volumeSlider->setVisible(true);
    }

}
//每触发一次，音量+5%，设置到音量进度条上。
void Widget::addVolume()
{
    int vol=ui->volumeSlider->value();
    vol+=5;
    if(vol>100)
        vol=100;
    ui->volumeSlider->setValue(vol);
}

//每触发一次，音量-5%，设置到音量进度条上。
void Widget::redVolume()
{
    int vol=ui->volumeSlider->value();
    vol-=5;
    if(vol<0)
        vol=0;
    ui->volumeSlider->setValue(vol);
}
void Widget::setPlayTime()
{
    if(!lyrics.getListLyricsTime().empty()&&player->position()>=lyrics.getListLyricsTime().at(lyricsID)&&lyricsID<lyrics.getListLyricsTime().size()-1){
        ui->label->setText(lyrics.getListLyricsText().at(lyricsID));
        ui->label_2->setText(lyrics.getListLyricsText().at(lyricsID+1));
        ui->label_3->setText(lyrics.getListLyricsText().at(lyricsID+2));
        lyricsID++;
    }
}
