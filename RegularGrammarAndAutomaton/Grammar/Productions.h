#pragma once
#include<string>
#include<iostream>

class Productions
{
private:
	std::string m_leftMember;
	std::string m_rightMember;
public:
	Productions() = default;

	std::string getLeftMember();
	std::string getRightMember();


	friend std::istream& operator>>(std::istream& in, Productions& pr);
	friend std::ostream& operator<<(std::ostream& out, const Productions& pr);
};

