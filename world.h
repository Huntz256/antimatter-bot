#pragma once

#include <bitset>
#include <unordered_set>

#include <QString>

static const int Height = 50;
static const int Width = 100;

class World
{
    bool m_State = false;
    std::bitset<Width*Height> m_Map[2];

    bool get(int i, int j) const;

public:

    void set(const QString& Str);
    void getCandidates(std::unordered_set<int>& C) const;
    void next();
    int count() const;

    void set(int i, int j, bool state, bool Next = false);
};

inline void World::set(const QString& Str)
{
    for (int i = 0; i < Width*Height; i++)
    {
        m_Map[m_State][i] = Str[i] == '1';
    }
}

inline bool World::get(int i, int j) const
{
    return m_Map[m_State][((i + Height) % Height)*Width + ((j + Width) % Width)];
}

inline void World::set(int i, int j, bool state, bool Next)
{
    m_Map[m_State ^ Next][((i + Height) % Height)*Width + ((j + Width) % Width)] = state;
}

inline void World::getCandidates(std::unordered_set<int> &C) const
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

inline void World::next()
{
    m_Map[!m_State].reset();

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

inline int World::count() const
{
    return (int)m_Map[m_State].count();
}
