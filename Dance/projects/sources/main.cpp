#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <fstream>
#include <streambuf>
#include <time.h>

constexpr int length = 16;

void show(const char(&s)[length])
{
	for (char x : s) std::cout << x;

	std::cout << std::endl;
}

void spin(char (&s)[length], int n)
{
	char t[length]{ 0 };
	int z = length - n;
	for (int i = 0; i < z; ++i)
		t[i] = s[i];

	for (int i = 0; i < n; ++i)
		s[i] = s[z + i];

	for (int i = 0; i < z; ++i)
		s[n + i] = t[i];
}

void exchange(char (&s)[length], int a, int b)
{
	s[a] ^= s[b] ^= s[a] ^= s[b];
}

void partner(char (&s)[length], char a, char b)
{
	for (char& x : s)
	{
		if (x == a)
			x = b;
		else if (x == b)
			x = a;
	}
}

using action_type = std::function<void (char(&)[length])>;

action_type parse_action(const std::string& a)
{
	switch (a[0])
	{
	case 's':
	{
		int n = std::stoi(std::string(a, 1));
		return [=](char(&s)[length]) { spin(s, n); };
	}
	case 'x':
	{
		size_t pos = a.find('/', 1);
		int m = std::stoi(a.substr(1, pos));
		int n = std::stoi(a.substr(pos+1));
		return [=](char(&s)[length]) { exchange(s, m, n); };
	}
	case 'p':
		return [=](char(&s)[length]) { partner(s, a[1], a[3]); };
	default:
		return nullptr;
	}
}

std::vector<action_type> parse_step(const std::string& s)
{
	std::vector<action_type> actions;

	for (size_t start = 0;;)
	{
		size_t pos = s.find(',', start);
		if (pos == std::string::npos)
		{
			actions.push_back(parse_action(s.substr(start)));
			break;
		}
		actions.push_back(parse_action(s.substr(start, pos - start)));
		start = pos + 1;
	}

	return actions;
}

int main()
{
	std::ifstream fin("../resources/step.txt", std::ios::in);
	std::istreambuf_iterator<char> beg(fin), end;
	std::string step_text(beg, end);
	fin.close();

	std::vector<action_type> actions = parse_step(step_text);

	std::cout << actions.size() << std::endl;

	char s[length]{ 0 };
	for (int i = 0; i < length; ++i)
		s[i] = 'a' + i;

	time_t t1 = clock();
	for (int i = 0; i < 1000; ++i)
	{
		for (auto action : actions)
			action(s);
	}

	time_t t2 = clock();

	std::cout << "time: " << t2 - t1 << std::endl;

	show(s);

	return 0;
}


