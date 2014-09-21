#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <unordered_set>
#include <vector>

#include <QThread>

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject *parent = 0);

    void setState(const QString& State);

signals:
    void ready(int x, int y, QString State);

private:
    QString m_State;

    std::unordered_set<int> m_CandidatesSet;
    std::vector<int> m_Canditates;

    void run() override;
};

#endif // WORKERTHREAD_H
