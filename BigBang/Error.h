#pragma warning(disable:4996)
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime> 

class ErrorReport {
private:
	std::ofstream file;
	std::string getNowTime() {
		auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::stringstream ss;
		ss << std::put_time(std::localtime(&now), "%Y-%m-%d %X");
		return ss.str();
	}
public:
	ErrorReport(std::string filename) {
		file.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);
	}
	
	void Write(std::string text) {
		file << getNowTime() << " : " << text;
	}
	void WriteLine(std::string text) {
		Write(text + "\n");
	}
	
	void CloseReport() {
		file.close();
	}
};