#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <bitset>
#include<string.h>
using namespace std;

map<int, string> reg = {
	{0, "$r0"}, {1, "$at"}, {2, "$v0"}, {3, "$v1"}, {4, "$a0"},
	{5, "$a1"}, {6, "$a2"}, {7, "a3"}, {8, "$t0"}, {9, "$t1"},
	{10, "$t2"}, {11, "$t3"}, {12, "$t4"}, {13, "$t5"}, {14, "$t6"},
	{15, "$t7"}, {16, "$s0"}, {17, "$s1"}, {18, "$s2"}, {19, "$s3"},
	{20, "$s4"}, {21, "$s5"}, {22, "$s6"}, {23, "$s7"}, {24, "$t8"},
	{25, "$t9"}, {26, "$k0"}, {27, "$k1"}, {28, "$gp"}, {29, "$sp"},
	{30, "$s8"}, {31, "$ra"}
};

string argu[4];
string binary(int a, int width = 5)
{
	string result = "";
	for (int i = 0; i < width; ++i) {
		result = to_string(a % 2) + result; // 取餘數，插入到字串前
		a /= 2;
	}
	return result;
}
string count_inst(string& inst, int a) {
	for (const auto& s : reg) {
		if (s.second == argu[a]) {
			return inst + binary(s.first);
		}
	}
	cerr << "Error: Register not found for argument: " << argu[a] << endl;
	return inst + "00000"; // 返回全0，表示未匹配
}

string bin_to_hex(const string& bin) {
	stringstream ss;
	ss << hex << stoi(bin, nullptr, 2);
	string hex_str = ss.str();
	while (hex_str.length() < 8) {
		hex_str = "0" + hex_str; // 保???8位16?制（32位二?制）
	}
	return hex_str;
}
int main(int argc, char* argv[]) {
	ifstream in;
	string line;
	//開檔
	in.open(argv[1]);
	//沒打開要給提示
	if (in.fail()) {
		cout << "Input file opening failed!" << endl;
		return 1;
	}
	// 讀取檔案
	while (getline(in, line)) {
		//cout << line << endl;
		if (line.empty()) continue; // 跳過空行
		// 切割行為單詞
		istringstream iss(line);
		string word;
		vector<string> tokens;

		while (iss >> word) {
			// 移除label
			size_t colonPos = word.find(':');
			if (colonPos != string::npos) {
				word = word.substr(colonPos + 1);
			}

			// 切割逗號並加入 tokens
			size_t pos = 0;
			while ((pos = word.find(',')) != string::npos) {
				tokens.push_back(word.substr(0, pos)); // 加入逗號前的部分
				word.erase(0, pos + 1);                // 移除已處理部分
			}

			//把切好的放入tokens
			if (!word.empty()) {
				tokens.push_back(word);
			}

		}
		// 如果 tokens 為空，跳過
		if (tokens.empty()) continue;
		string argu3 = tokens.back();
		if (argu3[0] == '.')continue;//.xxx 跳過
		//tokens存入argu
		int i = 0;
		for (const auto& token : tokens) {
			argu[i] = token;
			//		cout << i<<" "<<argu[i] << endl; 
			i++;
		}

		//argu[0] store inst
		//argu[1] store argu1
		//argu[2] store argu2
		//argu[3] store argu3
		string inst;
		string two = "2";
		string hex = "16";
		//if (strcmp(argv[2], two.c_str()) == 0)
		//{
		if (argu[0] == "add")//r-format Op(6)+ rs( 5)+ rt (5)+ rd (5)+ shamt (5)+ funct( 6)
		{
			inst = "000000";
			inst = count_inst(inst, 2);
			inst = count_inst(inst, 3);
			inst = count_inst(inst, 1);
			inst += "00000";//shame
			inst += "100000";//32
			if (strcmp(argv[2], two.c_str()) == 0)
			{
				cout << "R-format:" << inst << endl;
			}
			else if (strcmp(argv[2], hex.c_str()) == 0)
			{
				cout << "R-format:" << bin_to_hex(inst) << endl;
			}
		}
		else if (argu[0] == "sub")
		{
			inst = "000000";
			inst = count_inst(inst, 2);
			inst = count_inst(inst, 3);
			inst = count_inst(inst, 1);
			inst += "00000";//shame
			inst += "100010";//32
			if (strcmp(argv[2], two.c_str()) == 0)
			{
				cout << "R-format:" << inst << endl;
			}
			else if (strcmp(argv[2], hex.c_str()) == 0)
			{
				cout << "R-format:" << bin_to_hex(inst) << endl;
			}
		}//i-format op(6)+rs(5)+rt(5)+Immediate(16)
		else if (argu[0] == "addi")
		{
			inst = "001000";//op
			inst = count_inst(inst, 2);//rs
			inst = count_inst(inst, 1);//rt
			inst += bitset<16>(stoi(argu[3])).to_string();//immediately
			if (strcmp(argv[2], two.c_str()) == 0)
			{
				cout << "I-format:" << inst << endl;
			}
			else if (strcmp(argv[2], hex.c_str()) == 0)
			{
				cout << "I-format:" << bin_to_hex(inst) << endl;
			}
		}
		else if (argu[0] == "slti")
		{
			inst = "001010";
			inst = count_inst(inst, 2);
			inst = count_inst(inst, 1);
			inst += bitset<16>(stoi(argu[3])).to_string();
			if (strcmp(argv[2], two.c_str()) == 0)
			{
				cout << "i-format:" << inst << endl;
			}
			else if (strcmp(argv[2], hex.c_str()) == 0)
			{
				cout << "i-format:" << bin_to_hex(inst) << endl;
			}
		}
		//}

	}

	in.close();
	return 0;
}

//讀argv
/*不用argument line
	string file;
	getline(cin,file);
	vector<string> tokens;
	istringstream iss(file);
	string token;

	while (iss >> token) {
		tokens.push_back(token);
	}
	for (int i = 1; i < argc; i++) {
		//argv[i]= nullptr;
		tokens.push_back(argv[i]);
	}
	cout << "Tokens:" << endl;
	for (const string& t : tokens) {
		cout << t << endl;
	}
*/
/*test
	for (int i = 1; i < argc; i++)
	{
		cout << argv[i] << endl;
	}
	*/
	/*if (tokens[0]=="add")//r
			{
				cout << "f-r" << endl;
			}
			else if (tokens[0] == "sub")//r
			{
				cout << "f-r" << endl;
			}
			else if(tokens[0] == "addi")//i
			{
				cout << "f-i" << endl;
			}
			else if (tokens[0] == "slti")//i
			{
				cout << "f-i" << endl;
			}
			*/
