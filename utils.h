#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <vector>

std::string get_string(std::string prompt) {
	std::cout << prompt;
	std::string str;
	getline(std::cin, str);
	return str;
}

bool is_whitespace(char token);
bool is_whitespace(std::string str);

std::string trim(std::string str) {
	size_t start = 0;
	size_t end = str.length() - 1;
	while (is_whitespace(str[start])) start++;
	while (is_whitespace(str[end])) end--;
	return str.substr(start, end - start + 1);
}

bool is_whitespace(char token) {
	return token == ' ' || token == '\t' || token == '\n' || token == '\r';
}

bool is_whitespace(std::string str) {
	return trim(str).empty();
}

void each_line(std::string file_name, std::function<void(std::string)> fn) {
	std::ifstream file(file_name);
	std::string line;
	while (std::getline(file, line)) {
		fn(trim(line));
	}
}

void println(char character) {
	printf("%c\n", character);
}

void println(std::string str = "") {
	std::cout << str << std::endl;
}

#endif