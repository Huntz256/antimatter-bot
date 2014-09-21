#include <QtWebKitWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushSwitchBot, &QAbstractButton::released, this, &MainWindow::onSwitchBot);
    connect(ui->pushGo,        &QAbstractButton::released, this, &MainWindow::onGo);
    connect(&m_Thread, &WorkerThread::ready, this, &MainWindow::onMoveReady);

    ui->webView->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    ui->webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);

  //  ui->webView->load(QUrl(QString("http://lifecompetes.com/?token=943d72bd7281e5dfca2182ef0d7d81023851v3")));
  //  ui->webView->load(QUrl(QString("http://lifecompetes.com/?token=808e2059df6755291d9f6dfd810c52652934v3")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMoveReady(int x, int y, QString State)
{
    QWebFrame* pFrame = ui->webView->page()->mainFrame();

    // Get current world state
    QString CurState = pFrame->evaluateJavaScript("gridToString()").toString();

    // Check if state changed while we were computing this move
    if (CurState != State)
        return;

    // Place new cell
    pFrame->evaluateJavaScript(QString("place(%1, %2)").arg(x).arg(y));
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if (m_Thread.isRunning())
        return;

    QWebFrame* pFrame = ui->webView->page()->mainFrame();

    int NumCells = pFrame->evaluateJavaScript("app.game.playerManager.localPlayer.cells").toInt();
    if (NumCells < 1)
        return;

    // Define some javascript functions
    // FIXME: we need to do it only once
    QFile Script(":/script.js");
    Script.open(QFile::ReadOnly);
    QString ScriptContent = QTextStream(&Script).readAll();
    pFrame->evaluateJavaScript(ScriptContent);

    // Get current world state
    QString State = pFrame->evaluateJavaScript("gridToString()").toString();

    // Search for best move in separate thread
    m_Thread.setState(State);
    m_Thread.start(QThread::IdlePriority);
}

void MainWindow::onSwitchBot()
{
    m_BotIsRunning = !m_BotIsRunning;

    if (m_BotIsRunning)
    {
        ui->pushSwitchBot->setText("Stop bot");
        m_iTimer = startTimer(500);
    }
    else
    {
        ui->pushSwitchBot->setText("Start bot");
        killTimer(m_iTimer);
    }
}

void MainWindow::onGo()
{
    ui->webView->load(QUrl(ui->lineAddress->text()));
}
