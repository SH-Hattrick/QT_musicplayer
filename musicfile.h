#ifndef MUSICFILE_H
#define MUSICFILE_H

#include<QMediaPlaylist>
#include<QDir>
#include<QFile>
#include<QStringList>
#include<QDebug>

class MusicFile
{
private:
    const QString ImagePath = "images";
    const QString MusicPath = "Music";
    const QDir *MusicDir = new QDir("Music");
    const QDir *ImageDir = new QDir("images");
public:
    MusicFile();
    MusicFile(QString usr);
    void LoadALLMusic(QMediaPlaylist* list,QStringList&,QString dir="");
    void LoadLocalMusic(QStringList&);
    void LoadSingleMusic(QMediaPlaylist*,QString usr,QString);
    void RemoveSingleMusic(QString,QString);
};

#endif // MUSICFILE_H
