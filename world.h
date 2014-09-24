#pragma once

#include <bitset>
#include <unordered_set>

#include <QString>

static const int Height = 50;
static const int Width = 100;
static const int Interval = 20;

class World
{
    std::bitset<Width> m_Map[Interval][Height];

    bool get(int t, int i, int j) const;

public:

    void set(const QString& Str);
    void set(int t, int i, int j, bool state);
    void getCandidates(std::unordered_set<int>& C) const;
    void simulate(int t, int i, int j);
    int count() const;
};

inline void World::set(const QString& Str)
{
    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
            m_Map[0][i][j] = Str[i*Width + j] == '1';

    for (int t = 1; t < Interval; t++)
        for (int i = 0; i < Height; i++)
            for (int j = 0; j < Width; j++)
                simulate(t, i, j);
}

inline bool World::get(int t, int i, int j) const
{
    return m_Map[t][(i + Height) % Height][(j + Width) % Width];
}

inline void World::set(int t, int i, int j, bool state)
{
    m_Map[t][(i + Height) % Height][(j + Width) % Width] = state;
}

inline void World::getCandidates(std::unordered_set<int> &C) const
{
    C.clear();

    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            if (!get(0, i, j))
                continue;

            // add all cells in 2-range as potential candidates
            for (int i0 = i-2; i0 <= i+2; i0++)
                for (int j0 = j-2; j0 <= j+2; j0++)
                {
                    int i1 = (i0 + Height) % Height;
                    int j1 = (j0 + Width)  % Width;
                    if (!get(0, i1, j1)) // exclude live cells
                        C.insert(i1*Width + j1);
                }
        }
    }
}

inline void World::simulate(int t, int i, int j)
{
    int n = get(t-1, i-1, j-1) + get(t-1, i-1, j+1) + get(t-1, i+1, j-1) + get(t-1, i+1, j+1) + get(t-1, i, j-1) + get(t-1, i-1, j) + get(t-1, i+1, j) + get(t-1, i, j+1);
    bool alive = get(t-1, i, j);
    if (alive && (n == 2 || n == 3))
        set(t, i, j, true);
    else if (!alive && (n == 3))
        set(t, i, j, true);
    else
        set(t, i, j, false);
}

inline int World::count() const
{
    int sum = 0;
    for (int i = 0; i < Height; i++)
        sum += (int)m_Map[Interval-1][i].count();
    return sum;
}
