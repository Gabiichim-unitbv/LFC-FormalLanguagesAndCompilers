#pragma once
#include <string>
#include <iostream>

class Production
{
private:
	std::string m_first;
	std::string m_second;
public:
	Production() = default;
	~Production() = default;

	std::string first() const;
	std::string second() const;

	char first(int) const;
	char second(int) const;

	friend std::istream& operator>>(std::istream& in, Production& prod);
	friend std::ostream& operator<<(std::ostream& out, const Production& prod);
};

