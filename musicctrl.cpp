#include "musicctrl.h"

MusicCtrl::MusicCtrl()
{
    //初始化
    file = new MusicFile;
    player = new QMediaPlayer;
    curList = new QMediaPlaylist;
    //onesong = new QMediaPlaylist;
    curList->setPlaybackMode(QMediaPlaylist::Loop);//默认播放模式：顺序
    player->setPlaylist(curList);
    player->setVolume(30);
    vol=player->volume();

    //资源加载
    file->LoadLocalMusic(alllist);//加载本地所以歌曲
    loadChosenFile();//加载歌单


}

void MusicCtrl::loadAllFile()
{
    file->LoadALLMusic(curList,mylist);
}


void MusicCtrl::loadChosenFile()
{

    file->LoadALLMusic(curList,mylist,user);
}

void MusicCtrl::playList()//开始播放
{
    if(curList->isEmpty() == true)
        return;
    if(player->state()!=QMediaPlayer::PlayingState)
        player->play();

}

void MusicCtrl::pauseList()//暂停播放
{
    if(curList->isEmpty() == true)
        return;
    if(player->state()!=QMediaPlayer::PausedState)
    {
        player->pause();
    }
}

//未解决的问题
void MusicCtrl::changePlayMode()//改变播放模式
{
    //随机播放
    if(curList->playbackMode() == QMediaPlaylist::Loop)
        curList->setPlaybackMode(QMediaPlaylist::Random);
    //单曲循环
    else if(curList->playbackMode() == QMediaPlaylist::Random)
    {
        curList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    }
    else
    {
        curList->setPlaybackMode(QMediaPlaylist::Loop);
    }
}

void MusicCtrl::volumnCtrl(int vol)//音量调节
{
    this->vol=vol;
    player->setVolume(vol);
}

void MusicCtrl::nextsong()
{
    if(curList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
    {
        curList->setPlaybackMode(QMediaPlaylist::Loop);
        curList->next();
        curList->previous();
        playList();
        curList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        return;
    }
    else if(curList->isEmpty() == true&&
            curList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
        return;
    curList->next();
    playList();
}

void MusicCtrl::lastsong()
{

    if(curList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
    {
        curList->setPlaybackMode(QMediaPlaylist::Loop);
        curList->next();
        curList->previous();
        playList();
        curList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        return;
    }
    else if(curList->isEmpty() == true)
        return;
    curList->previous();
    playList();
}

int MusicCtrl::addToList(QString str)
{
    if(mylist.contains(str))
    {
        QMessageBox newbox(QMessageBox::Information,"Info","歌曲已在歌单中");
        newbox.exec();
        return -1;
    }
    file->LoadSingleMusic(curList,user,str);
    mylist.push_back(str);
    return 0;
}

//使curlist与mylist同步
//未解决：移除歌曲会导致当前播放的曲目中断
int MusicCtrl::removeFromList(QString str)
{
    player->pause();
    qDebug()<<"ctrl called";
    //file->RemoveSingleMusic(user,str);
    int index=mylist.indexOf(str);
    mylist.removeOne(str);
    curList->removeMedia(index);
    player->setPlaylist(curList);
    player->play();
    return 0;
}

void MusicCtrl::playSpecificSong(QString str)
{
    int index = mylist.indexOf(str);
    curList->setCurrentIndex(index);
    playList();
}


int MusicCtrl::getVol()
{
    return vol;
}

QString MusicCtrl::getCurSongName()
{
    return mylist[curList->currentIndex()];
}
