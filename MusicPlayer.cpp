#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //界面初始化以及自定义界面设计
    ui->setupUi(this);
    setWindowTitle("Hattrick Music");
    const QIcon mainicon("images/Mainicon.jfif");
    setWindowIcon(mainicon);
    setWindowFlags(windowFlags() &~ Qt::WindowMaximizeButtonHint);//禁止最大化)
    styleSheetConstruction();

    //QSS
    //应用样式 apply the qss style
    QFile file("myqss.qss");
    file.open(QFile::ReadOnly);
    if(!file.isOpen())
        qDebug()<<"QSS application failure!";
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    //资源初始化
    timer1 = new QTimer;
    ctrl = new MusicCtrl;
    displayList();
    bkgimage.load("images/background_1.jpg");


    //同步音量
    ui->horizontalSlider_volumn->setRange(0,100);
    ui->horizontalSlider_volumn->setValue(ctrl->getVol());
    ui->horizontalSlider_volumn->valueChanged(ui->horizontalSlider_volumn->value());
    connect(ui->horizontalSlider_volumn,SIGNAL(valueChanged(int)),this,
            SLOT(volumnBarChanged()));


    //增加/删减曲目
    connect(ui->listWidget_songlist,&QListWidget::itemClicked,
            [&](QListWidgetItem*qwi)
    {
        ui->listWidget_songlist->setCurrentItem(qwi);
    });//单击
    connect(ui->listWidget_songlist,&QListWidget::itemDoubleClicked,
            this,&Widget::listwidget_double_clicked);//双击
    //connect()
    ui->listWidget_songlist->setCurrentItem(ui->listWidget_songlist->currentItem());


    //进度条与时间
    connect(ctrl->player,SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));//切歌
    connect(ctrl->player,SIGNAL(positionChanged(qint64)),this,SLOT(onPositionChanged(qint64)));//进度
}

Widget::~Widget()
{
    delete ui;
    delete timer1;
}


void Widget::on_pushButton_play_clicked()
{

    ctrl->playList();

}

void Widget::on_pushButton_nextsong_clicked()
{
    ctrl->nextsong();
}

void Widget::on_pushButton_lastsong_clicked()
{
    ctrl->lastsong();
}

void Widget::on_pushButton_pause_clicked()
{

    ctrl->pauseList();
}

void Widget::on_pushButton_playmode_clicked()
{
    if(ui->pushButton_playmode->text() == "顺序播放")
        ui->pushButton_playmode->setText("随机播放");
    else if(ui->pushButton_playmode->text() == "随机播放")
        ui->pushButton_playmode->setText("单曲循环");
    else ui->pushButton_playmode->setText("顺序播放");
    ctrl->changePlayMode();
}

void Widget::on_pushButton_mylist_clicked()
{
    curlistwidget = MYLIST;
    ui->listWidget_songlist->clear();
    ui->listWidget_songlist->addItems(ctrl->mylist);
}

void Widget::on_pushButton_createlist_clicked()
{
    //no operation
}

void Widget::volumnBarChanged()
{
    ctrl->volumnCtrl(ui->horizontalSlider_volumn->value());
}

void Widget::on_pushButton_displayallmusic_clicked()
{
    curlistwidget = ALLLIST;
    ui->listWidget_songlist->clear();
    ui->listWidget_songlist->addItems(ctrl->alllist);
}

void Widget::listwidget_double_clicked(QListWidgetItem*ql)
{
    if(curlistwidget == ALLLIST)
    {
        QMessageBox::StandardButton btm;
        btm = QMessageBox::question(this,"pardon","是否将 "+ql->text()+" 加入歌单");
        if(btm == QMessageBox::Yes)
        {
            ctrl->addToList(ql->text());
            //on_pushButton_mylist_clicked();
        }
    }
    else if(curlistwidget == MYLIST)
    {
        //QListWidget
        ctrl->playSpecificSong(ql->text());
        ui->listWidget_songlist->setCurrentItem(ql);
    }
}

void Widget::on_pushButton_deletesong_clicked()
{
    if(curlistwidget == ALLLIST)
        return;
    if(ui->listWidget_songlist->currentItem() == nullptr)
        return;
    QMessageBox::StandardButton btm;
    btm = QMessageBox::question(this,"pardon","确定要将 "+ ui->listWidget_songlist->currentItem()->text()+" 永久移除吗？");
    if(btm == QMessageBox::Yes)
    //删除
    {

        ctrl->removeFromList(ui->listWidget_songlist->currentItem()->text());
        QListWidgetItem* tmp = (ui->listWidget_songlist->currentItem());
        ui->listWidget_songlist->removeItemWidget(ui->listWidget_songlist->currentItem());
        delete tmp;
        displayList();
    }
}

void Widget::displayList()
{
    ui->listWidget_songlist->clear();
    for(int i=0;i<ctrl->mylist.size();++i)
        ui->listWidget_songlist->insertItem(i,ctrl->mylist[i]);
}

//切歌时
void Widget::onDurationChanged(qint64 dura)
{
    //毫秒！
    ui->horizontalSlider_timebar->setMaximum(dura);
    int secs = dura/1000;
    int mins = secs/60;
    secs=secs%60;
    if(secs>=10)
        durationTime = QString::asprintf("%d:%d",mins,secs);
    else{
        QString str1=QString::asprintf("%d:0",mins);
        QString str2=QString::asprintf("%d",secs);
        durationTime = str1+str2;
    }
    ui->label_songtime->setText(positionTime+"/"+durationTime);
    //显示当前歌名
    ui->label_curpiece->setText(ctrl->getCurSongName());
}

//拖动进度条时
void Widget::onPositionChanged(qint64 pos)
{
    //QSlider;
    //正在调整，不处理
    if(ui->horizontalSlider_timebar->isSliderDown())
        return;
    ui->horizontalSlider_timebar->setValue(pos);
    int secs = pos/1000;
    int mins = secs/60;
    secs=secs%60;
    if(secs>=10)
        positionTime = QString::asprintf("%d:%d",mins,secs);
    else{
        QString str1=QString::asprintf("%d:0",mins);
        QString str2=QString::asprintf("%d",secs);
        positionTime = str1+str2;
    }
    ui->label_songtime->setText(positionTime+"/"+durationTime);
}

void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    //qDebug()<<"123";
    int x=ev->x();
    int y=ev->y();
    int posx = ui->horizontalSlider_timebar->x();
    int posy = ui->horizontalSlider_timebar->y();
    int w=ui->horizontalSlider_timebar->width();
    int h=ui->horizontalSlider_timebar->height();
    if(x>posx&&x<posx+w&&y>posy&&y<posy+h)
    {
        int val = ui->horizontalSlider_timebar->maximum()* (x-posx)/w;
        ui->horizontalSlider_timebar->setValue(val);
        ctrl->player->setPosition(val);
    }
}

//QSS演示设计
void Widget::styleSheetConstruction()
{
    ui->label_curpiece->setStyleSheet("color:rgb(136, 51, 195);");

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    brush.setTextureImage(bkgimage); //背景图片
    painter.setBrush(brush);
    painter.setPen(Qt::black);  //边框色
    painter.drawRoundedRect(this->rect(), 5, 5); //圆角5像素

}


