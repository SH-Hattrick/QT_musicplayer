#ifndef MUSICCTRL_H
#define MUSICCTRL_H

#include"musicfile.h"
#include<QWidget>
#include<QMediaPlayer>
#include<QMediaControl>
#include<QMediaPlaylist>
#include<QDebug>
#include<QStringList>
#include<QMessageBox>

class MusicCtrl
{
private:
    QMediaPlaylist *curList;
    MusicFile *file;
    int vol=30;
    QString user = "Mymusic";
public:
    QMediaPlayer *player;
    QStringList alllist;
    QStringList mylist;

    MusicCtrl();
    int getVol();
    QString getCurSongName();
    void loadAllFile();//加载音乐名
    void loadChosenFile();//加载用户歌单
    int addToList(QString);//添加音乐
    int removeFromList(QString);//移除
    void playList();//开始播放
    void pauseList();//暂停播放
    void changePlayMode();//改变播放模式
    void volumnCtrl(int);//音量调节
    void nextsong();
    void lastsong();
    void playSpecificSong(QString);
};

#endif // MUSICCTRL_H
