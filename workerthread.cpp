#include <omp.h>
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

    const int iNumThreads = omp_get_num_threads();

    int Min[20];
    int iMin[20];

    for (int i = 0; i < iNumThreads; i++)
    {
        Min[i] = world.count();
        iMin[i] = -1;
    }

    #pragma omp parallel for
    for (int k = 0; k < (int)m_Canditates.size(); k++)
    {
        int iCandidate = m_Canditates[k];
        int i0 = iCandidate / Width;
        int j0 = iCandidate % Width;

        World TestWorld = world;
        TestWorld.set(0, i0, j0, true);

        for (int t = 1; t < Interval; t++)
        {
            for (int i = i0 - t; i <= i0 + t; i++)
                for (int j = j0 - t; j <= j0 + t; j++)
                    TestWorld.simulate(t, i, j);
        }

        int iThread = omp_get_thread_num();
        int Count = TestWorld.count();
        if (Count < Min[iThread])
        {
            Min[iThread] = Count;
            iMin[iThread] = iCandidate;
        }
    }

    for (int i = 1; i < iNumThreads; i++)
    {
        if (Min[i] < Min[0])
        {
            Min[0] = Min[i];
            iMin[0] = iMin[i];
        }
    }

    if (iMin[0] != -1)
        emit ready(iMin[0] % Width, iMin[0] / Width, m_State);
}
