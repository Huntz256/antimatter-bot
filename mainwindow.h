#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void timerEvent(QTimerEvent *);

private slots:
    void onSwitchBot();

};

#endif // MAINWINDOW_H
