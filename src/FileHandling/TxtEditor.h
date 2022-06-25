#ifndef TXTEDITOR_H_
#define TXTEDITOR_H_

#include <iostream>
#include <sstream>
#include <fstream>

class TxtEditor {
private:
public:
	static std::string read(const char* filename){
		std::fstream file;
		file.open(filename, std::ios::in);
		std::stringstream sstm;
		if(file.is_open()){
			std::string line;
			while(getline(file, line)){
				sstm << line << "\n";
			}
			file.close();
			return sstm.str();
		} else {
			std::cout << "Unable to open file"; // TODO error throwing
			return 0;
		}
	}

	static void write(const char* filename, std::string data){
		std::fstream file;
		file.open(filename, std::ios::out);
		if(file.is_open()){
			file << data.c_str();
			file.close();
		} else {
			std::cout << "Unable to open file for editing" << std::endl; // TODO error throwing
		}
	}
};




#endif
