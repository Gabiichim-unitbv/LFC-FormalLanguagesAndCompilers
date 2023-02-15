#include "Productions.h"



std::istream& operator>>(std::istream& in, Productions& pr)
{
  
    std::string operation;
    std::string delimiter = "->";
    std::getline(in, operation);
    pr.m_leftMember= operation.substr(0, operation.find(delimiter));
    pr.m_rightMember = operation.substr(operation.find(delimiter) + 2, operation.size());

    return in;

}

std::ostream& operator<<(std::ostream& out, const Productions& pr)
{
    out << pr.m_leftMember << "->" << pr.m_rightMember;
    return out;
}

std::string Productions::getLeftMember()
{
    return m_leftMember;
}

std::string Productions::getRightMember()
{
    return m_rightMember;
}
