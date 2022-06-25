#ifndef BINEDITOR_H_
#define BINEDITOR_H_

#include <fstream>
#include <iostream>
#include <iomanip>

struct BinaryData {
	uint8_t* data;
	int size = 0;
};

class BinEditor {
public:
	static BinaryData readBinary(const char* filename){
		std::fstream file(filename, std::ios::binary | std::ios::in);
		std::streampos begin, end;
		BinaryData data;

		if(file.is_open()){
			begin = file.tellg();
			file.seekg(0, std::ios::end);
			end = file.tellg();
			file.seekg(0, std::ios::beg);
			int size = end-begin;

			char* memblock = new char[size];
			file.read(memblock, size);
			data.size = size;
			data.data = reinterpret_cast<uint8_t*>(memblock);
		} else {
			std::cout << "Could not open file \"" << filename << "\"" << std::endl;
			exit(1);
		}
		return data;
	}

	static void writeBinary(const char* filename, BinaryData bData){
		std::fstream outFile(filename, std::ios::out);
		if(outFile.is_open()){
			int bytesPerLine = 20;
			int lines = bData.size/bytesPerLine+1;
			int remainder = bData.size % bytesPerLine;

			outFile << std::hex << std::uppercase;
			for(int i = 0; i < lines; i++){
				for(int j = 0; i == lines-1 ? j < remainder : j < bytesPerLine; j++){
					outFile << std::setfill('0') << std::setw(sizeof(uint8_t)*2)
							<< (int) bData.data[i*bytesPerLine + j] << " ";
				}
				outFile << "\n";
			}
			outFile.close();
		} else {
			std::cout << "Could not open output file\n";
			exit(1);
		}
	}

	static void printHex(uint8_t* data, int count){
		std::cout << std::hex << std::uppercase;
		for(int i = 0; i < count; i++){
			std::cout << std::setfill('0') << std::setw(sizeof(uint8_t)*2)<<
					(int) data[i] << " ";
		}
	}

	static void printChars(uint8_t* data, int count){
		for(int i = 0; i < count; i++){
			std::cout << (char) data[i];
		}
	}

	static bool compareCharsToBin(const char* chars1, uint8_t* bin, int count){
		bool out = true;
		for(int i = 0; i < count && out; i++){
			if(chars1[i] != (char) bin[i]) out = false;
		}
		return out;
	}

	static int binaryToInt(uint8_t* start, int len){
		int out = 0;
		for(int i = 0; i < len; i++){
			out += (int) (start[len-1-i] << (8*i));
		}
		return out;
	}
};

#endif
