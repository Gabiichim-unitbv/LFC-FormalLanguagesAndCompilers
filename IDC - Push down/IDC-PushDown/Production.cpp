#include "Production.h"

std::string Production::first() const
{
    return m_first;
}

std::string Production::second() const
{
    return m_second;
}

char Production::first(int index) const
{
    return m_first[index];
}

char Production::second(int index) const
{
    return m_second[index];
}

std::istream& operator>>(std::istream& in, Production& prod)
{
    in >> prod.m_first >> prod.m_second;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Production& prod)
{
    out << prod.m_first << " -> " << prod.m_second;
    return out;
}
