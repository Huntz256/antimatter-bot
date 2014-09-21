#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "workerthread.h"

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
    bool m_BotIsRunning = false;
    int m_iTimer = 0;
    WorkerThread m_Thread;

    void timerEvent(QTimerEvent *);
    void onMoveReady(int x, int y, QString State);
    void onSwitchBot();
    void onGo();
};

#endif // MAINWINDOW_H
