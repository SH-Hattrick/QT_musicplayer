#include "musicfile.h"

MusicFile::MusicFile()
{

}

MusicFile::MusicFile(QString usr)
{

}

void MusicFile::LoadALLMusic(QMediaPlaylist* list,QStringList &ql,QString dir)
{
    QString curpath;
    QStringList allfile;
    if(dir.isEmpty())
    {
        curpath=MusicPath;
        allfile= MusicDir->entryList(QDir::Files);
    }
    else//导入用户文件，目前默认为Mylist
    {
        curpath = dir;
        QDir* newdir = new QDir(dir);
        allfile= newdir->entryList((QDir::Files));
    }
    //qDebug()<<allfile.size();
    for(int i=0;i<allfile.size();++i)
    {
        QString tmp =curpath + "/" + allfile[i];
        ql.push_back(allfile[i]);
        list->addMedia(QUrl::fromLocalFile(tmp));
    }
    //qDebug()<<list->isEmpty();
}

//此函数负责导入所有文件名以供查阅
void MusicFile::LoadLocalMusic(QStringList&ql)
{
    QDir* dir = new QDir(MusicPath);
    ql = dir->entryList(QDir::Files);
}

//此函数负责增加一个音乐文件
void MusicFile::LoadSingleMusic(QMediaPlaylist* list,QString usr,QString str)
{
    QString tmp = MusicPath + "/" + str;
    str = usr + "/" + str;
    list->addMedia(QUrl::fromLocalFile(tmp));
    //写入USER目录
    QFile::copy(tmp,str);
}

//此函数将在用户歌单中删除一个音乐文件
void MusicFile::RemoveSingleMusic(QString user,QString tar)
{
    //qDebug()<<"file deleted";
    QDir *dir = new QDir(user);
    dir->remove(tar);
}
