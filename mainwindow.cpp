#include <QtWebKitWidgets>
#include <QTime>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using std::vector;
using std::unordered_set;

static const int Height = 50;
static const int Width = 100;

class World
{
    bool m_State = false;
    vector<bool> m_Map[2];

    bool get(int i, int j);

public:

    World();

    void set(QString Str);
    void getCandidates(std::unordered_set<int>& C);
    void next();
    int count();

    void set(int i, int j, bool state, bool Next = false);
};

World::World()
{
    m_Map[0].resize(Width*Height);
    m_Map[1].resize(Width*Height);
}

void World::set(QString Str)
{
    for (int i = 0; i < Width*Height; i++)
    {
        m_Map[m_State][i] = Str[i] == '1';
    }
}

bool World::get(int i, int j)
{
    return m_Map[m_State][((i + Height) % Height)*Width + ((j + Width) % Width)];
}

void World::set(int i, int j, bool state, bool Next)
{
    m_Map[m_State ^ Next][((i + Height) % Height)*Width + ((j + Width) % Width)] = state;
}

void World::getCandidates(std::unordered_set<int> &C)
{
    C.clear();

    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            if (!get(i, j))
                continue;

            // add all cells in 2-range as potential candidates
            for (int i0 = i-2; i0 <= i+2; i0++)
                for (int j0 = j-2; j0 <= j+2; j0++)
                {
                    if (!get(i0, j0)) // exclude live cells
                        C.insert(i0*Width + j0);
                }
        }
    }
}

void World::next()
{
    m_Map[!m_State].assign(Width*Height, false);

    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            int n = get(i-1, j-1) + get(i-1, j+1) + get(i+1, j-1) + get(i+1, j+1) + get(i, j-1) + get(i-1, j) + get(i+1, j) + get(i, j+1);
            bool alive = get(i, j);
            if (alive && (n == 2 || n == 3))
                set(i, j, true, true);
            if (!alive && (n == 3))
                set(i, j, true, true);
        }
    }

    m_State = !m_State;
}

int World::count()
{
    return std::count(m_Map[m_State].begin(), m_Map[m_State].end(), true);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushSwitchBot, &QAbstractButton::released, this, &MainWindow::onSwitchBot);

    ui->webView->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    ui->webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);

    ui->webView->load(QUrl(QString("http://lifecompetes.com/?token=943d72bd7281e5dfca2182ef0d7d81023851v3")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    QWebFrame* pFrame = ui->webView->page()->mainFrame();

    int NumCells = pFrame->evaluateJavaScript("app.game.playerManager.localPlayer.cells").toInt();
    if (NumCells < 1)
        return;

    QFile Script(":/script.js");
    Script.open(QFile::ReadOnly);
    QString ScriptContent = QTextStream(&Script).readAll();
    pFrame->evaluateJavaScript(ScriptContent);

    QVariant grid = pFrame->evaluateJavaScript("gridToString()");
    QString Str = grid.toString();

    QTime t;
    t.start();

    static World world;
    world.set(Str);

    static unordered_set<int> CandidatesSet;
    world.getCandidates(CandidatesSet);

    static vector<int> Canditates;
    Canditates.clear();
    for (int c : CandidatesSet)
        Canditates.push_back(c);

  //  #pragma omp parallel for
    static World TestWorld;
    int Min = Width*Height + 99;
    int iMin = 0;
    for (unsigned int i = 0; i < Canditates.size(); i++)
    {
        TestWorld = world;
        TestWorld.set(Canditates[i] / Width, Canditates[i] % Width, true);

        for (int j = 0; j < 2; j++)
            TestWorld.next();
        int Count = TestWorld.count();
        if (Count < Min)
        {
            Min = Count;
            iMin = Canditates[i];
        }
    }


   /* for (int i = 0; i < 50*100; i++)
        world.next();*/

    int t0 = t.elapsed();

    grid = ui->webView->page()->mainFrame()->evaluateJavaScript("gridToString()");
    QString Str2 = grid.toString();
    if (Str != Str2)
        return;

    pFrame->evaluateJavaScript(QString("place(%1, %2)").arg(iMin % Width).arg(iMin / Width));

    ui->lineAddress->setText(QString("%1").arg(t0));
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
