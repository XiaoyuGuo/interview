#include "ini_parser.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>

void test1()
{
	const char* ini_text = "a=1\nb=2\n";
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "\n", "=")) {
		assert(false);
	}

	const std::string& a = parser.Get("a", NULL);
	assert(a == "1");

	std::string b = parser.Get("b", NULL);
	assert(b == "2");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

void test2()
{
	const char* ini_text = "a=1||b=2||c=3";
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "||", "=")) {
		assert(false);
	}

	const std::string& a = parser.Get("a", NULL);
	assert(a == "1");

	std::string b = parser.Get("b", NULL);
	assert(b == "2");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "3");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

void test3()
{
	const char* ini_text = "||||a:1||b:2||||c:3||||||";
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "||", ":")) {
		assert(false);
	}

	const std::string& a = parser.Get("a", NULL);
	assert(a == "1");

	std::string b = parser.Get("b", NULL);
	assert(b == "2");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "3");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

void test4()
{
	const char* ini_text = "asdfasdfasdfasdf||||:";
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "||", ":")) {
		assert(false);
	}

	const std::string& a = parser.Get("a", NULL);
	assert(a == "");

	std::string b = parser.Get("b", NULL);
	assert(b == "");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

void test5()
{
	const char* ini_text = "||||||||a=3||b=||=3||";
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "||", "=")) {
		assert(false);
	}

	const std::string& a = parser.Get("a", NULL);
	assert(a == "3");

	std::string b = parser.Get("b", NULL);
	assert(b == "");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

void test6()
{
	qh::INIParser parser;

	if (!parser.Parse("test.txt", "=")) {
		assert(false);
	}

	const std::string& a = parser.Get("test_section", "a", NULL);
        assert(a == "3");

	std::string b = parser.Get("test_section", "b", NULL);
	assert(b == "4");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

void test7()
{
	const char* ini_text = "";
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "||", "=")) {
		assert(false);
	}

	const std::string& a = parser.Get("a", NULL);
	assert(a == "");

	std::string b = parser.Get("b", NULL);
	assert(b == "");

	const std::string& c = parser.Get("c", NULL);
	assert(c == "");

	printf("[%s] All test is ok\n", __FUNCTION__);
}

int main(int argc, char* argv[])
{

	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
#ifdef WIN32
	system("pause");
#endif
	return 0;
}


