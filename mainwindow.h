#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QDesktopWidget>

#include <windows.h>
#include <lmcons.h>

#include "systemkeyboardreadwrite.h"

#include <QSystemTrayIcon>

#include <QPixmap>
#include <QResizeEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *statuslabel0, *statuslabel1, *statuslabel2, *statuslabel3;
    QTimer *blinktimer;
    QDateTime endtime, missiontime;
    QTime lastmousepress;
    QPoint mousepos;
    int screenoffsetx, screenoffsety;

public slots:
    void timer_blink();
    void keyPressed(byte *keysDepressed, DWORD keyPressed);
private slots:
    void on_btn_start_clicked();
    void on_btn_lastshot_clicked();
};

#endif // MAINWINDOW_H
