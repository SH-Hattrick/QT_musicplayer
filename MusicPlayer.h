#ifndef WIDGET
#define WIDGET

#include <QWidget>
#include<QIcon>
#include<QListWidget>
#include<QMessageBox>
#include<QMouseEvent>
#include<QTimer>
#include<QPaintEvent>
#include<QPainter>
#include<QStyle>
#include"musicctrl.h"

enum LISTTYPE{MYLIST=0,ALLLIST=1};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void displayList();
    void styleSheetConstruction();

private slots:

    void on_pushButton_play_clicked();

    void on_pushButton_nextsong_clicked();

    void on_pushButton_lastsong_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_playmode_clicked();

    void on_pushButton_mylist_clicked();

    void on_pushButton_createlist_clicked();

    void volumnBarChanged();

    void on_pushButton_displayallmusic_clicked();

    void listwidget_double_clicked(QListWidgetItem*);

    void on_pushButton_deletesong_clicked();

    void onDurationChanged(qint64);

    void onPositionChanged(qint64);

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Widget *ui;
    MusicCtrl *ctrl;
    LISTTYPE curlistwidget=MYLIST;
    QString durationTime;
    QString positionTime;
    QTimer* timer1;

    QImage bkgimage;
};
#endif // WIDGET
