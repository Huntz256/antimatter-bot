#include <atomic>

#include "workerthread.h"
#include "world.h"

WorkerThread::WorkerThread(QObject *parent) :
    QThread(parent)
{
}

void WorkerThread::setState(const QString& State)
{
    m_State = State;
}

void WorkerThread::run()
{/*
    QTime t;
    t.start();
    int t0 = t.elapsed();*/

    World world;
    world.set(m_State);

    // Get all possible move candidates
    world.getCandidates(m_CandidatesSet);

    // Convert candidates set to vector
    m_Canditates.clear();
    for (int c : m_CandidatesSet)
        m_Canditates.push_back(c);

    std::atomic<int> Min;
    std::atomic<int> iMin(-1);

    World TestWorld = world;
    for (int j = 0; j < 5; j++)
        TestWorld.next();
    Min = TestWorld.count();

    // FIXME: datarace here (Min, iMin)
    #pragma omp parallel for
    for (int i = 0; i < (int)m_Canditates.size(); i++)
    {
        int iCandidate = m_Canditates[i];

        World TestWorld = world;
        TestWorld.set(iCandidate / Width, iCandidate % Width, true);

        for (int j = 0; j < 5; j++)
            TestWorld.next();
        int Count = TestWorld.count();
        if (Count < Min)
        {
            Min = Count;
            iMin = iCandidate;
        }
    }

    if (iMin != -1)
        emit ready(iMin % Width, iMin / Width, m_State);
}
