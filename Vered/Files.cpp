//#include "Files.h"
//#include <fstream>
//#include <sstream>
//#include <filesystem>
//
//std::vector<std::vector<vered::casCell>> Files::loadTable(const char* filepath)
//{
//
//	std::vector<std::vector<vered::casCell>> table;
//	
//	std::ifstream file(filepath);
//	
//	std::string ops_str;
//	std::string cons_str;
//
//	std::getline(file, ops_str, ';');
//	std::getline(file, cons_str, ';');
//	file.close();
//
//	std::stringstream ops(ops_str);
//	std::stringstream cons(cons_str);
//
//	double opData[vered::op_params];
//
//
//	std::string num;
//
//	while (ops.rdbuf()->in_avail() != 0)
//	{
//		for (int i = 0; i < vered::op_params; i++)
//		{
//			std::getline(ops, num, ':');
//
//			opData[i] = std::stod(num);
//		}
//
//		if(opData[0] + 1 > table.size())
//			table.push_back(std::vector<vered::casCell>());
//
//		if (opData[1] + 1 > table[(int)opData[0]].size()) {
//			table[(int)opData[0]].emplace_back(new Cascade());
//
//			int row = table[(int)opData[0]].size() - 1;
//
//			table[(int)opData[0]][row].setPos((int)opData[0], row);
//		}
//
//		Envelope* env = new Envelope(opData[4], opData[5], opData[6], opData[7], opData[8], opData[9], opData[10]);
//		Operator* op = new Operator(opData[2], opData[3], *env);
//
//		table[(int)opData[0]][(int)opData[1]].cas->Prepend(op);
//
//	}
//
//	int conData[vered::pos_params * 2];
//
//	while (cons.rdbuf()->in_avail() != 0)
//	{
//		for (int i = 0; i < vered::pos_params * 2; i++)
//		{
//			std::getline(cons, num, ':');
//
//			conData[i] = std::stoi(num);
//		}
//
//		bool ifExistsFront = conData[0] < table.size() && conData[1] < table[conData[0]].size();
//		bool ifExistsBack = conData[2] < table.size() && conData[3] < table[conData[2]].size();
//
//		if(ifExistsBack && ifExistsFront)
//			table[conData[0]][conData[1]].cMyInput(&table[conData[2]][conData[3]]);
//	}
//
//	return table;
//}
//
//void Files::saveTable(const std::vector<std::vector<vered::casCell>>& table,const char* filepath)
//{
//	std::string content = "";
//
//	if (table.size() > 0) {
//		for (int i = 0; i < table.size(); i++)
//		{
//			for (int j = 0; j < table[i].size(); j++)
//			{
//				std::string pos = "";
//				pos += std::to_string(i);
//				pos += ":";
//				pos += std::to_string(j);
//				pos += ":";
//
//				table[i][j].cas->getData(content, pos);
//			}
//		}
//
//		content += ";";
//
//		for (int i = 0; i < table.size() - 1; i++)
//		{
//			for (int j = 0; j < table[i].size(); j++)
//			{
//				for (vered::casCell* cell : table[i][j].in)
//				{
//					content += std::to_string(i);
//					content += ":";
//					content += std::to_string(j);
//					content += ":";
//
//					content += std::to_string(cell->column);
//					content += ":";
//					content += std::to_string(cell->row);
//					content += ":";
//				}
//			}
//		}
//
//		std::ofstream file(filepath);
//		file << content;
//		file.close();
//	}
//}
//
