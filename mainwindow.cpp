#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    if(SystemKeyboardReadWrite::instance()->setConnected(true))
        connect(SystemKeyboardReadWrite::instance(), SIGNAL(keyPressed(byte*,DWORD)), this, SLOT(keyPressed(byte*,DWORD)));
    else
        qDebug("Could not connect Keyboard Hook!");

    ui->setupUi(this);

    ui->lblTime->setText("");
    ui->lblDate->setText("");

    // Statusbar initialisieren:
/*
    statuslabel0 = new QLabel( "acz", statusBar() );
    statuslabel0->setAlignment(Qt::AlignLeft);
    statuslabel1 = new QLabel( "", statusBar() );
    statuslabel1->setAlignment(Qt::AlignCenter);
    statuslabel2 = new QLabel( "", statusBar() );
    statuslabel2->setAlignment(Qt::AlignCenter);
    statuslabel3 = new QLabel( "", statusBar() );
    statuslabel3->setAlignment(Qt::AlignRight);
    statusBar()->addWidget( statuslabel0, true );
    statusBar()->addWidget( statuslabel1, true );
    statusBar()->addWidget( statuslabel2, true );
    statusBar()->addWidget( statuslabel3, true );
*/
    // Find screen and calculate offset to move the window:
    QList<QRect> screensizes;
    for(int i=0; i<QDesktopWidget().screenCount(); i++)
    {
        screensizes << QDesktopWidget().screenGeometry(i);
        //qDebug("Screen Size %u: %u x %u", i+1, screensizes[i].width(), screensizes[i].height());
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
    qDebug( "Screen Offset: %u", screenoffsetx );

    // Show the GUI:
    if(screenoffsetx > 0)
    {
        this->move(screenoffsetx, screenoffsety);
        //this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
        this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->showMaximized();
        this->setWindowFlags(Qt::Tool | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_QuitOnClose, true);
    }
    else
        this->show();

    blinktimer = new QTimer(this);
    connect(blinktimer, SIGNAL(timeout()), this, SLOT(timer_blink()));
    blinktimer->start(500);

    timer_blink();  // do it immediately to show time, etc.

    QMenu *stimenu = new QMenu(this);
        stimenu->addAction("Exit", this, SLOT(close()));
//        stimenu->addAction("Abort");

        // Show the SystemTrayIcon:
        QSystemTrayIcon  *sti = new QSystemTrayIcon(this);
        sti->setToolTip("TacScreen");
        sti->toolTip();
        sti->setIcon(QIcon(":/icon"));
        sti->setContextMenu(stimenu);
        sti->show();


//        QPixmap map(":/map.bmp");
        QPixmap map_k(":/map_keys");

        int w = 590;
        int h = 590;

//        ui->lblMap->setPixmap(map.scaled(w,h,Qt::KeepAspectRatio));
        ui->lblMap->setPixmap(map_k.scaled(w,h,Qt::KeepAspectRatio));

    //    ui->lblMap->setPixmap(map);


        int x = ui->lbl_icon->width();
        int y = ui->lbl_icon->height();

        QPixmap icon(":/icon");
        ui->lbl_icon->setPixmap(icon.scaled(x,y,Qt::KeepAspectRatio));
        ui->lbl_icon->setText("");

        ui->lbl_deadhunter_i->setText("0");
        ui->lbl_lastshot_i->setText("0");
        ui->lbl_missiontime_i->setText("0");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timer_blink()
{
    /*****************
     * Current Time: *
     *****************/
    ui->lblTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->lblDate->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy"));

    // Mission Time:
    //endtime.isValid()


    int secs = QDateTime::currentDateTime().secsTo(endtime);
    int minutes = secs/60;
    secs = secs%60;

    QString m = QString::number(minutes);
    while(m.length() < 2)
        m = "0"+m;

    QString s = QString::number(secs);
    while(s.length() < 2)
        s = "0"+s;

    ui->lbl_missiontime_i->setText(QString::number(minutes) + ":" + s);
}

//==========KEYS====================================================

void MainWindow::keyPressed(byte *keysDepressed, DWORD keyPressed)
{
    Q_UNUSED(keysDepressed);

    qDebug("KeyCode: %u (0x%02x)", (unsigned int)keyPressed, (unsigned int)keyPressed);

    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

    QPixmap map_m(":/map_comp");
    QPixmap map_k(":/map_keys");

    QPixmap map_1(":/1.bmp");
    QPixmap map_2(":/2.bmp");
    QPixmap map_3(":/3.bmp");
    QPixmap map_4(":/4.bmp");
    QPixmap map_5(":/5.bmp");
    QPixmap map_6(":/6.bmp");
    QPixmap map_7(":/7.bmp");
    QPixmap map_8(":/8.bmp");
    QPixmap map_9(":/9.bmp");

//    int w = ui->lblMap->width();
//    int h = ui->lblMap->height();


//    qDebug("width: %d height: %d"), (int)w, (int)h;

    int w = 590;
    int h = 590;

//-------------------MAP------------------------------------

    if(keyPressed == 111) //Num/
    {
        ui->lblMap->setPixmap(map_m.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 106) //Num*
    {
        ui->lblMap->setPixmap(map_k.scaled(w,h,Qt::KeepAspectRatio));
    }

//-------------------KILLCOUNT------------------------------------

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
//       ui->lbl_deadhunter_i->setText("Dead Hunters: " + QString::number(n));
    }

//-------------------SPLITMAP------------------------------------

    if(keyPressed == 103) //Num7
    {
        ui->lblMap->setPixmap(map_7.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 104) //Num8
    {
        ui->lblMap->setPixmap(map_8.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 105) //Num9
    {
        ui->lblMap->setPixmap(map_9.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 100) //Num4
    {
        ui->lblMap->setPixmap(map_4.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 101) //Num5
    {
        ui->lblMap->setPixmap(map_5.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 102) //Num6
    {
        ui->lblMap->setPixmap(map_6.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 97) //Num1
    {
        ui->lblMap->setPixmap(map_1.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 98) //Num2
    {
        ui->lblMap->setPixmap(map_2.scaled(w,h,Qt::KeepAspectRatio));
    }

    if(keyPressed == 99) //Num3
    {
        ui->lblMap->setPixmap(map_3.scaled(w,h,Qt::KeepAspectRatio));
    }
}

//==========BUTTONS====================================================

void MainWindow::on_btn_start_clicked()
{
    ui->lbl_deadhunter_i->setText("0");
    //ui->lbl_missiontime_i->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->lbl_lastshot_i->setText("0");

    endtime = QDateTime::currentDateTime().addSecs(3600);
}




void MainWindow::on_btn_lastshot_clicked()
{
    ui->lbl_lastshot_i->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
}
