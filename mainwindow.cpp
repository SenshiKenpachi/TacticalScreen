#include "mainwindow.h"
#include "ui_mainwindow.h"


//========== KONSTRUKTOR ============================================================================================================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    if(SystemKeyboardReadWrite::instance()->setConnected(true))
        connect(SystemKeyboardReadWrite::instance(), SIGNAL(keyPressed(byte*,DWORD)), this, SLOT(keyPressed(byte*,DWORD)));
    else
        qDebug("Could not connect Keyboard Hook!");


    ui->setupUi(this);


//---------- Setup Screen Offset -----------------------------------------------------------------------------------------------------------------------------------------------------------
    QList<QRect> screensizes;
    for(int i=0; i<QDesktopWidget().screenCount(); i++)
    {
        screensizes << QDesktopWidget().screenGeometry(i);
    }
    screenoffsetx = 0;
    screenoffsety = 0;
    for(int i=0; i<screensizes.count(); i++)
    {
        if(screensizes[i].width() == 1024 && screensizes[i].height() == 600)
        {
            screenoffsetx = screensizes[i].left();
            screenoffsety = screensizes[i].top();
            break;
        }
    }
    qDebug( "Screen Offset X: %u Y: %u", screenoffsetx , screenoffsety);

    if(screenoffsetx > 0)
    {
        this->move(screenoffsetx, screenoffsety);
        this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->showMaximized();
        this->setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_QuitOnClose, true);
    }
    else
        this->show();

//---------- Setup Blinker -------------------------------------------------------------------------------------------------------------------------------------------------------------
    blinktimer = new QTimer(this);
    connect(blinktimer, SIGNAL(timeout()), this, SLOT(timer_blink()));
    blinktimer->start(1000);
    timer_blink();  // do it immediately to show time, etc.

//---------- Setup Menu -------------------------------------------------------------------------------------------------------------------------------------------------------------
    QMenu *stimenu = new QMenu(this);
    stimenu->addAction("Exit", this, SLOT(close()));

//---------- Setup SystemTrayIcon -----------------------------------------------------------------------------------------------------------------------------------------------------------
    QSystemTrayIcon  *sti = new QSystemTrayIcon(this);
    sti->setToolTip("TacScreen");
    sti->toolTip();
    sti->setIcon(QIcon(":/icon"));
    sti->setContextMenu(stimenu);
    sti->show();

//---------- Seutp QPixmaps -------------------------------------------------------------------------------------------------------------------------------------------------------------
    map_1 = QPixmap(":/1.bmp");
    map_2 = QPixmap(":/2.bmp");
    map_3 = QPixmap(":/3.bmp");
    map_4 = QPixmap(":/4.bmp");
    map_5 = QPixmap(":/5.bmp");
    map_6 = QPixmap(":/6.bmp");
    map_7 = QPixmap(":/7.bmp");
    map_8 = QPixmap(":/8.bmp");
    map_9 = QPixmap(":/9.bmp");
    map_k = QPixmap(":/map_keys");
    map_m = QPixmap(":/map_comp");
    icon = QPixmap(":/icon");

    w = 590;
    h = 590;

//---------- Setup GUI -------------------------------------------------------------------------------------------------------------------------------------------------------------
    Reset_Mission();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//========== BLINKER ============================================================================================================================================================
void MainWindow::timer_blink()
{
//---------- Current Time -------------------------------------------------------------------------------------------------------------------------------------------------------------
    ui->lblTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->lblDate->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy"));

//---------- Mission Timer -------------------------------------------------------------------------------------------------------------------------------------------------------------
    if (endtime.isValid())
    {
        if (endtime < QDateTime::currentDateTime())
        {
            Reset_Mission();
        }
        else
        {
            int secs = QDateTime::currentDateTime().secsTo(endtime);
            int minutes = secs/60;
            secs = secs%60;

            QString m = QString::number(minutes);
            while(m.length() < 2)
                m = "0"+m;

            QString s = QString::number(secs);
            while(s.length() < 2)
                s = "0"+s;

            ui->lbl_missiontime_i->setText(m + ":" + s);
        }
    }

//---------- Last Shot -------------------------------------------------------------------------------------------------------------------------------------------------------------
    if (lastshot.isValid())
    {
        int secs = lastshot.secsTo(QDateTime::currentDateTime());
        int minutes = secs/60;
        secs = secs%60;

        QString m = QString::number(minutes);
        while(m.length() < 2)
            m = "0"+m;

        QString s = QString::number(secs);
        while(s.length() < 2)
            s = "0"+s;

        ui->lbl_lastshot_i->setText(m + ":" + s);
    }
}

//========== KEYS ============================================================================================================================================================
void MainWindow::keyPressed(byte *keysDepressed, DWORD keyPressed)
{
    Q_UNUSED(keysDepressed);

    qDebug("KeyCode: %u (0x%02x)", (unsigned int)keyPressed, (unsigned int)keyPressed);

//---------- MAP -------------------------------------------------------------------------------------------------------------------------------------------------------------
    if(keyPressed == 111) //Num/
    {
        ui->lblMap->setPixmap(map_m.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 106) //Num*
    {
        ui->lblMap->setPixmap(map_k.scaled(w,h,Qt::KeepAspectRatio));
    }

//---------- KILLCOUNT --------------------------------------------------------------------------------------------------------------------------------------
    if(keyPressed == 109) //Num-
    {
        int n = ui->lbl_deadhunter_i->text().toInt();
        n--;

        if(n<0)
        n=0;

        ui->lbl_deadhunter_i->setText(QString::number(n));
    }

    if(keyPressed == 107) //Num+
    {
        int n = ui->lbl_deadhunter_i->text().toInt();
        n++;

        if(n>10)
        n=10;

        ui->lbl_deadhunter_i->setText(QString::number(n));
    }

//---------- SPLITMAP ---------------------------------------------------------------------------------------------------------------------------------------------------
    if(keyPressed == 103) //Num7
        ui->lblMap->setPixmap(map_7.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 104) //Num8
        ui->lblMap->setPixmap(map_8.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 105) //Num9
        ui->lblMap->setPixmap(map_9.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 100) //Num4
        ui->lblMap->setPixmap(map_4.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 101) //Num5
        ui->lblMap->setPixmap(map_5.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 102) //Num6
        ui->lblMap->setPixmap(map_6.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 97) //Num1
        ui->lblMap->setPixmap(map_1.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 98) //Num2
        ui->lblMap->setPixmap(map_2.scaled(w,h,Qt::KeepAspectRatio));

    if(keyPressed == 99) //Num3
        ui->lblMap->setPixmap(map_3.scaled(w,h,Qt::KeepAspectRatio));
}

//========= BUTTONS ============================================================================================================================================================
void MainWindow::on_btn_start_clicked()
{
    ui->lbl_deadhunter_i->setText("0");
    ui->lbl_lastshot_i->setText("-");
    endtime = QDateTime::currentDateTime().addSecs(1*60);
    lastshot = QDateTime();
    ui->lbl_icon->setPixmap(icon.scaled(ui->lbl_icon->width(),ui->lbl_icon->height(),Qt::KeepAspectRatio));
}

void MainWindow::on_btn_lastshot_clicked()
{
    lastshot = QDateTime::currentDateTime();
}

//========= SUBS ============================================================================================================================================================
void MainWindow::Reset_Mission()
{
    ui->lbl_deadhunter_i->setText("-");
    ui->lbl_missiontime_i->setText("-");
    ui->lbl_lastshot_i->setText("-");

    endtime = QDateTime();
    lastshot = QDateTime();

    ui->lbl_icon->setPixmap(icon.scaled(0,0,Qt::KeepAspectRatio));
    ui->lbl_icon->setText("");

    ui->lblMap->setPixmap(map_k.scaled(w,h,Qt::KeepAspectRatio));
}
