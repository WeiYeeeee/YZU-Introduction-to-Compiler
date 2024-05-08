#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <iomanip>
#include <queue>
using namespace std;
struct grammarData_list {
	vector<vector<string>> contextFreeGrammar_list;
	vector<string> contextFreeGrammar_header;
	vector<vector<string>> contextFreeGrammar_footer;
	vector<int> dot_index;
	vector<bool> add_line;
	unordered_map<string, int> shiftRecord;
	//bool reduce_action = false;
};
vector<vector<string>> symbol, symbol_first;
struct data_list {
	vector<vector<int>> parsing_stack;
	vector<vector<string>> parsing_symbol, parsing_input;
	vector<string>parsing_action;
	int result = 0; // 0 valid, 1 invalid, 2 invalid -- symbol
};
vector<data_list>testData_list;
vector<grammarData_list> contextFreeGrammar;
int in_vector(string target, vector<string> check_vector) {
	int i = 0;
	while (i < check_vector.size()) {
		if (check_vector[i] == target)
			break;
		i++;
	}
	if (i == check_vector.size())
		return -1;
	else
		return i;
}
int in_vector_vector(vector<string> target, vector<vector<string>> check_vector2) {
	bool not_in = true;
	for (int i = 0; i < check_vector2.size(); i++) {
		if (check_vector2[i] == target) {
			not_in = false;
			break;
		}
	}
	if (not_in == true)
		return false;
	else
		return true;
}
const vector<string> split(const string target, const string delimiter, int mod) {
	vector<string> result;
	size_t pos = 0;
	string token, str = target;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		str.erase(0, pos + delimiter.length());
		if (token != "") {
			result.resize(result.size() + 1);
			result.back() = token;
		}
		if (mod == 1) {
			result.resize(result.size() + 1);
			result.back() = delimiter;
		}
	}
	if (str != "") {
		result.resize(result.size() + 1);
		result.back() = str;
	}
	return result;
}
vector<string> first_action(string symbol_nonterminal) {
	vector<string> result, temp1;
	int i, j, k, l;
	for (i = 1; i < contextFreeGrammar[0].contextFreeGrammar_header.size(); i++) {
		if ((contextFreeGrammar[0].contextFreeGrammar_header[i] == symbol_nonterminal) && (contextFreeGrammar[0].contextFreeGrammar_list[i][0] != symbol_nonterminal)) {
			// -> 後面第一個symbol為nonterminal
			if (in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i][0], symbol[1]) == -1) {
				result.resize(result.size() + 1);
				result.back() = contextFreeGrammar[0].contextFreeGrammar_list[i][0];
			}
			else {
				temp1 = first_action(contextFreeGrammar[0].contextFreeGrammar_list[i][0]);
				for (j = 0; j < temp1.size(); j++)
					if (in_vector(temp1[j], result) == -1)
						result.insert(result.end(), temp1[j]);
			}
		}
	}
	return result;
}
vector<string> footer_action(string symbol_nonterminal, int i_end) {
	vector<string> result, temp1;
	int i, j, k, l, m, num;
	for (i = 0; i < i_end; i++) {
		num = 0;
		j = contextFreeGrammar[0].dot_index[i];
		// dot_index 後面symbol為target --> 【E -> E】,【T -> E】 target為E 
		if (j < contextFreeGrammar[0].contextFreeGrammar_list[i].size()&&contextFreeGrammar[0].contextFreeGrammar_list[i][j] == symbol_nonterminal) {
			// dot_index後面symbol不只一個
			if (j + 1 < contextFreeGrammar[0].contextFreeGrammar_list[i].size()) {
				k = in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i][j + 1], symbol[1]);
				if (k != -1) {
					for (l = 0; l < symbol_first[k].size(); l++)
						if (in_vector(symbol_first[k][l], result) == -1)
							result.insert(result.end(), symbol_first[k][l]);
				}
				else {
					if (in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i][j + 1], result) == -1)
						result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_list[i][j + 1]);
				}
			}
			else
				for (k = 0; k < contextFreeGrammar[0].contextFreeGrammar_footer[i].size(); k++)
					if (in_vector(contextFreeGrammar[0].contextFreeGrammar_footer[i][k], result) == -1)
						result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_footer[i][k]);
		}

		if (contextFreeGrammar[0].contextFreeGrammar_list[i].size() == 1 && contextFreeGrammar[0].contextFreeGrammar_list[i][0] == symbol_nonterminal) {
			for (j = 0; j < contextFreeGrammar[0].contextFreeGrammar_footer[i].size(); j++)
				if (in_vector(contextFreeGrammar[0].contextFreeGrammar_footer[i][j], result) == -1)
					result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_footer[i][j]);
		}
	}
	for (i = i_end + 1; i < contextFreeGrammar[0].contextFreeGrammar_list.size(); i++) {
		// 後面有行數，它的dot_index後面的第一個symbol為target
		if (contextFreeGrammar[0].contextFreeGrammar_list[i][contextFreeGrammar[0].dot_index[i]] == symbol_nonterminal) {
			// 若此symbol不為最後一個，加first至footer
			if (contextFreeGrammar[0].dot_index[i] + 1 < contextFreeGrammar[0].contextFreeGrammar_list[i].size()) {
				j = in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i][contextFreeGrammar[0].dot_index[i] + 1], symbol[1]);
				if (j != -1) {
					for (k = 0; k < symbol_first[j].size(); k++)
						if (in_vector(symbol_first[j][k], result) == -1)
							result.insert(result.end(), symbol_first[j][k]);
				}
				else {
					if (in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i][contextFreeGrammar[0].dot_index[i] + 1], result) == -1)
						result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_list[i][contextFreeGrammar[0].dot_index[i] + 1]);
				}
			}
			else {
				// 若此行的footer沒有東西，代表是需要去找的，不是的話就直接插入
				if (contextFreeGrammar[0].contextFreeGrammar_footer[i].size() == 0) {
					string target_symbol = contextFreeGrammar[0].contextFreeGrammar_header[i], temp_symbol;
					bool end_action = false, find_action = false;
					while (end_action == false) {
						end_action = true;
						find_action = false;
						for (j = 1; j < contextFreeGrammar[0].contextFreeGrammar_list.size(); j++) {
							k = contextFreeGrammar[0].dot_index[j];
							while (k < contextFreeGrammar[0].contextFreeGrammar_list[j].size()) {
								if (contextFreeGrammar[0].contextFreeGrammar_list[j][k] == target_symbol)
									break;
								k++;
							}
							if (k == contextFreeGrammar[0].contextFreeGrammar_list[j].size() - 1 && contextFreeGrammar[0].contextFreeGrammar_list[j][k] != contextFreeGrammar[0].contextFreeGrammar_header[j]) {
								if (contextFreeGrammar[0].contextFreeGrammar_footer[j].size() != 0) {
									for (l = 0; l < contextFreeGrammar[0].contextFreeGrammar_footer[j].size(); l++) {
										if (in_vector(contextFreeGrammar[0].contextFreeGrammar_footer[j][l], result) == -1) {
											result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_footer[j][l]);
										}
									}
									find_action = true;
								}
								else {
									temp_symbol = contextFreeGrammar[0].contextFreeGrammar_header[j];
									end_action = false;
								}
							}
							else if (k < contextFreeGrammar[0].contextFreeGrammar_list[j].size()) {
								l = in_vector(contextFreeGrammar[0].contextFreeGrammar_list[j][k + 1], symbol[1]);
								if (l != -1) {
									for (m = 0; m < symbol_first[l].size(); m++)
										if (in_vector(symbol_first[l][m], result) == -1)
											result.insert(result.end(), symbol_first[l][m]);
								}
								else {
									if (in_vector(contextFreeGrammar[0].contextFreeGrammar_list[j][k + 1], result) == -1)
										result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_list[j][k + 1]);
								}
								find_action = true;
							}
						}
						if (find_action == true)
							end_action = true;
						if (end_action == false)
							target_symbol = temp_symbol;
					}
				}
				else {
					for (j = 0; j < contextFreeGrammar[0].contextFreeGrammar_footer[i].size(); j++) {
						if (in_vector(contextFreeGrammar[0].contextFreeGrammar_footer[i][j], result) == -1) {
							result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_footer[i][j]);
						}
					}
				}
			}
		}
	}
	//dot_index取代0
	if (contextFreeGrammar[0].contextFreeGrammar_list[i_end][0] == contextFreeGrammar[0].contextFreeGrammar_header[i_end]) {
		if (contextFreeGrammar[0].contextFreeGrammar_list[i_end].size() != 1) {
			i = in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i_end][1], symbol[1]);
			if (i != -1) {
				for (j = 0; j < symbol_first[i].size(); j++)
					if (in_vector(symbol_first[i][j], result) == -1)
						result.insert(result.end(), symbol_first[i][j]);
			}
			else {
				if (in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i_end][1], result) == -1)
					result.insert(result.end(), contextFreeGrammar[0].contextFreeGrammar_list[i_end][1]);
			}
		}
	}
	return result;
}
vector<string> footer_action_2(string symbol_nonterminal, int i_end, int i_target) {
	vector<string> result, temp1;
	int i, j, k, l, m, num;
	for (i = 0; i < i_end; i++) {
		num = 0;
		j = contextFreeGrammar[i_target].dot_index[i];
		// dot_index 後面symbol為target --> 【E -> E】,【T -> E】 target為E 
		if (j< contextFreeGrammar[i_target].contextFreeGrammar_list[i].size() && contextFreeGrammar[i_target].contextFreeGrammar_list[i][j] == symbol_nonterminal) {
			// dot_index後面symbol不只一個
			if (j + 1 < contextFreeGrammar[i_target].contextFreeGrammar_list[i].size()) {
				k = in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[i][j + 1], symbol[1]);
				if (k != -1) {
					for (l = 0; l < symbol_first[k].size(); l++)
						if (in_vector(symbol_first[k][l], result) == -1)
							result.insert(result.end(), symbol_first[k][l]);
				}
				else {
					if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[i][j + 1], result) == -1)
						result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_list[i][j + 1]);
				}
			}
			else
				for (k = 0; k < contextFreeGrammar[i_target].contextFreeGrammar_footer[i].size(); k++)
					if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_footer[i][k], result) == -1)
						result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_footer[i][k]);
		}

		if (contextFreeGrammar[i_target].contextFreeGrammar_list[i].size() == 1 && contextFreeGrammar[i_target].contextFreeGrammar_list[i][0] == symbol_nonterminal) {
			for (j = 0; j < contextFreeGrammar[i_target].contextFreeGrammar_footer[i].size(); j++)
				if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_footer[i][j], result) == -1)
					result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_footer[i][j]);
		}
	}
	for (i = i_end + 1; i < contextFreeGrammar[i_target].contextFreeGrammar_list.size(); i++) {
		// 後面有行數，它的dot_index後面的第一個symbol為target
		if (contextFreeGrammar[i_target].contextFreeGrammar_list[i][contextFreeGrammar[i_target].dot_index[i]] == symbol_nonterminal) {
			// 若此symbol不為最後一個，加first至footer
			if (contextFreeGrammar[i_target].dot_index[i] + 1 < contextFreeGrammar[i_target].contextFreeGrammar_list[i].size()) {
				j = in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[i][contextFreeGrammar[i_target].dot_index[i] + 1], symbol[1]);
				if (j != -1) {
					for (k = 0; k < symbol_first[j].size(); k++)
						if (in_vector(symbol_first[j][k], result) == -1)
							result.insert(result.end(), symbol_first[j][k]);
				}
				else {
					if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[i][contextFreeGrammar[i_target].dot_index[i] + 1], result) == -1)
						result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_list[i][contextFreeGrammar[i_target].dot_index[i] + 1]);
				}
			}
			else {
				// 若此行的footer沒有東西，代表是需要去找的，不是的話就直接插入
				if (contextFreeGrammar[i_target].contextFreeGrammar_footer[i].size() == 0) {
					string target_symbol = contextFreeGrammar[i_target].contextFreeGrammar_header[i], temp_symbol;
					bool end_action = false, find_action = false;
					while (end_action == false) {
						end_action = true;
						find_action = false;
						for (j = 1; j < contextFreeGrammar[i_target].contextFreeGrammar_list.size(); j++) {
							k = contextFreeGrammar[i_target].dot_index[j];
							while (k < contextFreeGrammar[i_target].contextFreeGrammar_list[j].size()) {
								if (contextFreeGrammar[i_target].contextFreeGrammar_list[j][k] == target_symbol)
									break;
								k++;
							}
							if (k == contextFreeGrammar[i_target].contextFreeGrammar_list[j].size() - 1 && contextFreeGrammar[i_target].contextFreeGrammar_list[j][k] != contextFreeGrammar[i_target].contextFreeGrammar_header[j]) {
								if (contextFreeGrammar[i_target].contextFreeGrammar_footer[j].size() != 0) {
									for (l = 0; l < contextFreeGrammar[i_target].contextFreeGrammar_footer[j].size(); l++) {
										if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_footer[j][l], result) == -1) {
											result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_footer[j][l]);
										}
									}
									find_action = true;
								}
								else {
									temp_symbol = contextFreeGrammar[i_target].contextFreeGrammar_header[j];
									end_action = false;
								}
							}
							else if (k < contextFreeGrammar[i_target].contextFreeGrammar_list[j].size()) {
								l = in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[j][k + 1], symbol[1]);
								if (l != -1) {
									for (m = 0; m < symbol_first[l].size(); m++)
										if (in_vector(symbol_first[l][m], result) == -1)
											result.insert(result.end(), symbol_first[l][m]);
								}
								else {
									if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[j][k + 1], result) == -1)
										result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_list[j][k + 1]);
								}
								find_action = true;
							}
						}
						if (find_action == true)
							end_action = true;
						if (end_action == false)
							target_symbol = temp_symbol;
					}
				}
				else {
					for (j = 0; j < contextFreeGrammar[i_target].contextFreeGrammar_footer[i].size(); j++) {
						if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_footer[i][j], result) == -1) {
							result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_footer[i][j]);
						}
					}
				}
			}
		}
	}
	//dot_index取代0
	if (contextFreeGrammar[i_target].contextFreeGrammar_list[i_end][0] == contextFreeGrammar[i_target].contextFreeGrammar_header[i_end]) {
		if (contextFreeGrammar[i_target].contextFreeGrammar_list[i_end].size() != 1) {
			i = in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[i_end][1], symbol[1]);
			if (i != -1) {
				for (j = 0; j < symbol_first[i].size(); j++)
					if (in_vector(symbol_first[i][j], result) == -1)
						result.insert(result.end(), symbol_first[i][j]);
			}
			else {
				if (in_vector(contextFreeGrammar[i_target].contextFreeGrammar_list[i_end][1], result) == -1)
					result.insert(result.end(), contextFreeGrammar[i_target].contextFreeGrammar_list[i_end][1]);
			}
		}
	}
	return result;
}
void add_contextFreeGrammar_list(int num1, int num2, string target_symbol) {
	int i, j, k, l;
	queue<string> symbol_queue;
	bool repeat;
	//string target_symbol = contextFreeGrammar[num1].contextFreeGrammar_list[num2][contextFreeGrammar[num1].dot_index[num2]];
	for (i = 1; i < contextFreeGrammar[0].contextFreeGrammar_list.size(); i++) {
		if (contextFreeGrammar[0].contextFreeGrammar_header[i] == target_symbol) {
			repeat = false;
			//判斷有沒有重複過
			for (j = 0; j < contextFreeGrammar[num1].contextFreeGrammar_list.size(); j++) {
				if ((contextFreeGrammar[num1].contextFreeGrammar_list[j] == contextFreeGrammar[0].contextFreeGrammar_list[i])
					&& (contextFreeGrammar[num1].contextFreeGrammar_header[j] == contextFreeGrammar[0].contextFreeGrammar_header[i])
					&& (contextFreeGrammar[num1].dot_index[j] == contextFreeGrammar[0].dot_index[i])

					) {
					repeat = true;
				}
			}
			if (repeat == false) {
				contextFreeGrammar[num1].contextFreeGrammar_list.insert(contextFreeGrammar[num1].contextFreeGrammar_list.end(), contextFreeGrammar[0].contextFreeGrammar_list[i]);
				contextFreeGrammar[num1].contextFreeGrammar_header.insert(contextFreeGrammar[num1].contextFreeGrammar_header.end(), contextFreeGrammar[0].contextFreeGrammar_header[i]);
				contextFreeGrammar[num1].dot_index.insert(contextFreeGrammar[num1].dot_index.end(), contextFreeGrammar[0].dot_index[i]);
				contextFreeGrammar[num1].contextFreeGrammar_footer.resize(contextFreeGrammar[num1].contextFreeGrammar_footer.size() + 1);
				contextFreeGrammar[num1].add_line.insert(contextFreeGrammar[num1].add_line.end(), true);
				if (in_vector(contextFreeGrammar[0].contextFreeGrammar_list[i][contextFreeGrammar[0].dot_index[i]], symbol[1]) != -1) {
					symbol_queue.push(contextFreeGrammar[0].contextFreeGrammar_list[i][contextFreeGrammar[0].dot_index[i]]);
				}
			}
		}
	}
	while (!symbol_queue.empty()) {
		add_contextFreeGrammar_list(num1, num2, symbol_queue.front());
		symbol_queue.pop();
	}
}
bool check_same(int target1, int i_target1, int target2) {
	string target_symbol = contextFreeGrammar[target1].contextFreeGrammar_list[i_target1][contextFreeGrammar[target1].dot_index[i_target1]];
	grammarData_list temp;
	int i, j, k, temp_num1 = 0, temp_num2 = 0;
	// 將此目標的特定symbol 例如(T) S-> .T, L -> .T+S 全部加到temp中
	for (i = 0; i < contextFreeGrammar[target1].contextFreeGrammar_list.size(); i++) {
		if (contextFreeGrammar[target1].dot_index[i] < contextFreeGrammar[target1].contextFreeGrammar_list[i].size()) {
			if (contextFreeGrammar[target1].contextFreeGrammar_list[i][contextFreeGrammar[target1].dot_index[i]] == target_symbol) {
				temp.contextFreeGrammar_header.insert(temp.contextFreeGrammar_header.end(), contextFreeGrammar[target1].contextFreeGrammar_header[i]);
				temp.contextFreeGrammar_list.insert(temp.contextFreeGrammar_list.end(), contextFreeGrammar[target1].contextFreeGrammar_list[i]);
				temp.contextFreeGrammar_footer.insert(temp.contextFreeGrammar_footer.end(), contextFreeGrammar[target1].contextFreeGrammar_footer[i]);
				temp.dot_index.insert(temp.dot_index.end(), (contextFreeGrammar[target1].dot_index[i] + 1));
			}
		}
	}
	//查看是否此state已存在
	for (i = 0; i < contextFreeGrammar[target2].contextFreeGrammar_list.size(); i++) {
		if (contextFreeGrammar[target2].add_line[i] == false) {
			temp_num2++;
			for (j = 0; j < temp.contextFreeGrammar_list.size(); j++) {
				if ((temp.contextFreeGrammar_header[j] == contextFreeGrammar[target2].contextFreeGrammar_header[i])
					&& (temp.contextFreeGrammar_list[j] == contextFreeGrammar[target2].contextFreeGrammar_list[i])
					&& (temp.dot_index[j] == contextFreeGrammar[target2].dot_index[i])
					&& (temp.contextFreeGrammar_footer[j] == contextFreeGrammar[target2].contextFreeGrammar_footer[i])
					) {
					temp_num1++;
				}
			}
		}
	}
	if (temp_num1 == temp_num2 && temp_num1 == temp.contextFreeGrammar_list.size())
		return true;
	else
		return false;
}
void shift_action(int num) {
	grammarData_list temp_data;
	bool next_level = false, repeat_parser = false, check_same_state;
	int i, j, k, l, temp_num1, num_not_add_line;
	for (i = 0; i < contextFreeGrammar[num].contextFreeGrammar_list.size(); i++) {
		//dot_index不在最後一個
		if (!(contextFreeGrammar[num].dot_index[i] == contextFreeGrammar[num].contextFreeGrammar_list[i].size())) {
			// dot_index後面的一個symbol，還未shift整理
			if (contextFreeGrammar[num].shiftRecord.find(contextFreeGrammar[num].contextFreeGrammar_list[i][contextFreeGrammar[num].dot_index[i]]) == contextFreeGrammar[num].shiftRecord.end()) {
				next_level = false;
				for (j = 0; j < contextFreeGrammar.size(); j++) {
					//看有無重複的
					for (k = 0; k < contextFreeGrammar[j].contextFreeGrammar_list.size(); k++) {
						if (
							(contextFreeGrammar[num].contextFreeGrammar_list[i] == contextFreeGrammar[j].contextFreeGrammar_list[k])
							&& (contextFreeGrammar[num].dot_index[i] + 1 == contextFreeGrammar[j].dot_index[k])
							&& (contextFreeGrammar[num].contextFreeGrammar_header[i] == contextFreeGrammar[j].contextFreeGrammar_header[k])
							&& (contextFreeGrammar[num].contextFreeGrammar_footer[i] == contextFreeGrammar[j].contextFreeGrammar_footer[k])
							&& (contextFreeGrammar[j].add_line[k] == false)
							) {
							num_not_add_line = 0;
							l = 0;
							while (l < contextFreeGrammar[j].contextFreeGrammar_list.size()) {
								if (contextFreeGrammar[j].add_line[l] == false)
									num_not_add_line++;
								l++;
							}

							if (num_not_add_line == 1 || check_same(num, i, j) == true) {
								next_level = true;
								temp_num1 = j;
							}
						}
					}
				}
				//有重複
				if (next_level == true) {
					contextFreeGrammar[num].shiftRecord[contextFreeGrammar[num].contextFreeGrammar_list[i][contextFreeGrammar[num].dot_index[i]]] = temp_num1;
				}
				else {
					contextFreeGrammar[num].shiftRecord[contextFreeGrammar[num].contextFreeGrammar_list[i][contextFreeGrammar[num].dot_index[i]]] = contextFreeGrammar.size();
					contextFreeGrammar.resize(contextFreeGrammar.size() + 1);
					contextFreeGrammar.back().contextFreeGrammar_list.resize(contextFreeGrammar.back().contextFreeGrammar_list.size() + 1);
					contextFreeGrammar.back().contextFreeGrammar_list.back() = contextFreeGrammar[num].contextFreeGrammar_list[i];
					contextFreeGrammar.back().contextFreeGrammar_header.resize(contextFreeGrammar.back().contextFreeGrammar_header.size() + 1);
					contextFreeGrammar.back().contextFreeGrammar_header.back() = contextFreeGrammar[num].contextFreeGrammar_header[i];
					contextFreeGrammar.back().contextFreeGrammar_footer.resize(contextFreeGrammar.back().contextFreeGrammar_footer.size() + 1);
					contextFreeGrammar.back().contextFreeGrammar_footer.back() = contextFreeGrammar[num].contextFreeGrammar_footer[i];
					contextFreeGrammar.back().dot_index.resize(contextFreeGrammar.back().dot_index.size() + 1);
					contextFreeGrammar.back().dot_index.back() = contextFreeGrammar[num].dot_index[i] + 1;
					contextFreeGrammar.back().add_line.resize(contextFreeGrammar.back().add_line.size() + 1);
					contextFreeGrammar.back().add_line.back() = false;

					//cout << contextFreeGrammar.back().contextFreeGrammar_list.back()[contextFreeGrammar.back().dot_index.back()];

					if (contextFreeGrammar.back().dot_index.back() == contextFreeGrammar.back().contextFreeGrammar_list.back().size()) {
						//contextFreeGrammar.back().reduce_action = true;
					}
					else if (in_vector(contextFreeGrammar.back().contextFreeGrammar_list.back()[contextFreeGrammar.back().dot_index.back()], symbol[1]) != -1) {
						add_contextFreeGrammar_list(contextFreeGrammar.size() - 1, contextFreeGrammar.back().contextFreeGrammar_list.size() - 1, contextFreeGrammar.back().contextFreeGrammar_list.back()[contextFreeGrammar.back().dot_index.back()]);
					}
				}
			}
			else {
				repeat_parser = false;
				temp_num1 = contextFreeGrammar[num].shiftRecord[contextFreeGrammar[num].contextFreeGrammar_list[i][contextFreeGrammar[num].dot_index[i]]];
				for (j = 0; j < contextFreeGrammar[temp_num1].contextFreeGrammar_list.size(); j++) {
					if (
						(contextFreeGrammar[temp_num1].contextFreeGrammar_list[j] == contextFreeGrammar[num].contextFreeGrammar_list[i])
						&& (contextFreeGrammar[temp_num1].dot_index[j] == contextFreeGrammar[num].dot_index[i]+1)
						&& (contextFreeGrammar[temp_num1].contextFreeGrammar_footer[j] == contextFreeGrammar[num].contextFreeGrammar_footer[i])
						&& (contextFreeGrammar[temp_num1].contextFreeGrammar_header[j] == contextFreeGrammar[num].contextFreeGrammar_header[i])
						&& (contextFreeGrammar[temp_num1].add_line[j] == false)
						) {
						repeat_parser = true;
					}
				}
				if (repeat_parser == false) {

					contextFreeGrammar[temp_num1].contextFreeGrammar_list.resize(contextFreeGrammar[temp_num1].contextFreeGrammar_list.size() + 1);
					contextFreeGrammar[temp_num1].contextFreeGrammar_list.back() = contextFreeGrammar[num].contextFreeGrammar_list[i];
					contextFreeGrammar[temp_num1].contextFreeGrammar_header.resize(contextFreeGrammar[temp_num1].contextFreeGrammar_header.size() + 1);
					contextFreeGrammar[temp_num1].contextFreeGrammar_header.back() = contextFreeGrammar[num].contextFreeGrammar_header[i];
					contextFreeGrammar[temp_num1].contextFreeGrammar_footer.resize(contextFreeGrammar[temp_num1].contextFreeGrammar_footer.size() + 1);
					contextFreeGrammar[temp_num1].contextFreeGrammar_footer.back() = contextFreeGrammar[num].contextFreeGrammar_footer[i];
					contextFreeGrammar[temp_num1].dot_index.resize(contextFreeGrammar[temp_num1].dot_index.size() + 1);
					contextFreeGrammar[temp_num1].dot_index.back() = contextFreeGrammar[num].dot_index[i] + 1;
					contextFreeGrammar[temp_num1].add_line.resize(contextFreeGrammar[temp_num1].add_line.size() + 1);
					contextFreeGrammar[temp_num1].add_line.back() = false;
					// 若dot_index為nonterminal，要添加
					if (contextFreeGrammar[temp_num1].dot_index.back() == contextFreeGrammar[temp_num1].contextFreeGrammar_list.back().size()) {
						//contextFreeGrammar[temp_num1].reduce_action = true;
					}
					else if (in_vector(contextFreeGrammar[temp_num1].contextFreeGrammar_list.back()[contextFreeGrammar[temp_num1].dot_index.back()], symbol[1]) != -1) {
						add_contextFreeGrammar_list(temp_num1, contextFreeGrammar[temp_num1].contextFreeGrammar_list.size() - 1, contextFreeGrammar[temp_num1].contextFreeGrammar_list.back()[contextFreeGrammar[temp_num1].dot_index.back()]);
					}
				}
			}
		}
	}
}
void reduce_action(int target) {
	int i, j, k, l;
	//處理table中的r
	for (i = 0; i < contextFreeGrammar[target].contextFreeGrammar_list.size(); i++) {
		if (contextFreeGrammar[target].dot_index[i] == contextFreeGrammar[target].contextFreeGrammar_list[i].size()) {
			j = 0;
			while (j < contextFreeGrammar[0].contextFreeGrammar_list.size()) {
				if ((contextFreeGrammar[0].contextFreeGrammar_list[j] == contextFreeGrammar[target].contextFreeGrammar_list[i])
					&& (contextFreeGrammar[0].contextFreeGrammar_header[j] == contextFreeGrammar[target].contextFreeGrammar_header[i])
					) {
					break;
				}
				j++;
			}
			for (k = 0; k < contextFreeGrammar[target].contextFreeGrammar_footer[i].size(); k++) {
				contextFreeGrammar[target].shiftRecord[contextFreeGrammar[target].contextFreeGrammar_footer[i][k]] = 0 - j;
			}
		}
	}
}
void syntax_analysis(int target) {
	int i = 0, j, k, l;
	string target_symbol, symbol_aft;
	vector<string> temp_str1;
	int target_num, shift_num;
	bool find_list = false;
	while ((i == 0) || (testData_list[target].parsing_action[i - 1] != "acc") || (testData_list[target].result == 0)) {
		target_num = testData_list[target].parsing_stack[i].back();
		target_symbol = testData_list[target].parsing_input[i][0];
		//下一個action找到的是空的
		if (contextFreeGrammar[target_num].shiftRecord.count(target_symbol) == 0) {
			testData_list[target].result = 1;
			break;
		}
		else {
			shift_num = contextFreeGrammar[target_num].shiftRecord[target_symbol];
		}
		//i++;
		if (shift_num == 0) {
			if (testData_list[target].parsing_symbol[i].size() == 2)
				testData_list[target].parsing_action[i] = "acc";
			else
				testData_list[target].result = 1;
			break;
		}
		else if (shift_num > 0) {
			temp_str1.clear();
			temp_str1.insert(temp_str1.end(), testData_list[target].parsing_input[i].begin() + 1, testData_list[target].parsing_input[i].end());

			testData_list[target].parsing_action[i] = "S" + to_string(shift_num);
			//先把原本的stack插入，然後在後面新增 剛才action的數字
			testData_list[target].parsing_stack.insert(testData_list[target].parsing_stack.end(), testData_list[target].parsing_stack[i]);
			testData_list[target].parsing_stack.back().insert(testData_list[target].parsing_stack.back().end(), shift_num);
			//先把原本的symbol插入，然後在後面新增 剛才input的第一個symbol
			testData_list[target].parsing_symbol.insert(testData_list[target].parsing_symbol.end(), testData_list[target].parsing_symbol[i]);
			testData_list[target].parsing_symbol.back().insert(testData_list[target].parsing_symbol.back().end(), testData_list[target].parsing_input[i][0]);
			//先用一個變數把原本的input除了第一個symbol全部存插入，然後再新增
			testData_list[target].parsing_input.insert(testData_list[target].parsing_input.end(), temp_str1);

			testData_list[target].parsing_action.resize(testData_list[target].parsing_action.size() + 1);
		}
		else {
			testData_list[target].parsing_action[i] = "R" + to_string((0 - shift_num));
			//input不變
			testData_list[target].parsing_input.insert(testData_list[target].parsing_input.end(), testData_list[target].parsing_input[i]);

			testData_list[target].parsing_action.resize(testData_list[target].parsing_action.size() + 1);

			temp_str1 = contextFreeGrammar[0].contextFreeGrammar_list[(0 - shift_num)]; // 要轉換的字串
			symbol_aft = contextFreeGrammar[0].contextFreeGrammar_header[(0 - shift_num)]; // 轉換後的結果
			find_list = false;
			for (j = 0; j < testData_list[target].parsing_symbol[i].size(); j++) {
				if (testData_list[target].parsing_symbol[i][j] == temp_str1[0]) {
					k = 0;
					l = j;
					while (k < temp_str1.size() && l < testData_list[target].parsing_symbol[i].size()) {
						if (testData_list[target].parsing_symbol[i][l] != temp_str1[k]) {
							break;
						}
						k++;
						l++;
					}
					if (k == temp_str1.size() && l == testData_list[target].parsing_symbol[i].size()) {
						testData_list[target].parsing_symbol.insert(testData_list[target].parsing_symbol.end(), testData_list[target].parsing_symbol[i]);
						testData_list[target].parsing_symbol[i + 1].resize(j + 1);
						testData_list[target].parsing_symbol[i + 1].back() = symbol_aft;
						int temp_num = testData_list[target].parsing_stack[i][j - 1];
						if (contextFreeGrammar[temp_num].shiftRecord.count(symbol_aft) == 0) {
							testData_list[target].result == 1;
							break;
						}
						else {
							shift_num = contextFreeGrammar[temp_num].shiftRecord[symbol_aft];
						}
						testData_list[target].parsing_stack.insert(testData_list[target].parsing_stack.end(), testData_list[target].parsing_stack[i]);
						testData_list[target].parsing_stack[i + 1].resize(j + 1);
						testData_list[target].parsing_stack[i + 1].back() = shift_num;
						find_list = true;
					}
				}
			}
			if (find_list == false) {
				testData_list[target].result == 1;
				break;
			}
		}
		i++;
	}
}
void output(int num1, int num2) {
	int i, j, k;
	for (i = num1; i < num2; i++) {
		cout << setw(2) << i << ":\n";
		for (j = 0; j < contextFreeGrammar[i].contextFreeGrammar_list.size(); j++) {
			cout << setw(6) << contextFreeGrammar[i].contextFreeGrammar_header[j] << " -> ";

			for (k = 0; k < contextFreeGrammar[i].contextFreeGrammar_list[j].size(); k++) {
				if (contextFreeGrammar[i].dot_index[j] == k)
					cout << "·";
				cout << setw(2) << contextFreeGrammar[i].contextFreeGrammar_list[j][k];
			}
			if (contextFreeGrammar[i].dot_index[j] == contextFreeGrammar[i].contextFreeGrammar_list[j].size())
				cout << "·";
			if (contextFreeGrammar[i].contextFreeGrammar_footer[j].size() != 0) {
				cout << ",";
				cout << setw(16 - 2 * contextFreeGrammar[i].contextFreeGrammar_list[j].size()) << "{";
			}
			for (k = 0; k < contextFreeGrammar[i].contextFreeGrammar_footer[j].size(); k++)
				if (k == contextFreeGrammar[i].contextFreeGrammar_footer[j].size() - 1)
					cout << contextFreeGrammar[i].contextFreeGrammar_footer[j][k] << "}\n";
				else
					cout << contextFreeGrammar[i].contextFreeGrammar_footer[j][k] << ", ";

		}
	}
}
void output_table() {
	int i, j, k;
	cout << setw(3) << " ";
	for (j = 0; j < symbol[0].size(); j++)
		cout << setw(10) << symbol[0][j];
	cout << setw(10) << "$";
	for (j = 0; j < symbol[1].size(); j++)
		cout << setw(10) << symbol[1][j];
	cout << endl;

	for (i = 0; i < contextFreeGrammar.size(); i++) {
		cout << setw(3) << i;
		for (j = 0; j < symbol[0].size(); j++) {
			if (contextFreeGrammar[i].shiftRecord.count(symbol[0][j]) > 0) {
				if (contextFreeGrammar[i].shiftRecord[symbol[0][j]] > 0)
					cout << setw(10) << "S" + to_string(contextFreeGrammar[i].shiftRecord[symbol[0][j]]);
				else if (contextFreeGrammar[i].shiftRecord[symbol[0][j]] < 0)
					cout << setw(10) << "R" + to_string(0 - contextFreeGrammar[i].shiftRecord[symbol[0][j]]);
				else
					cout << setw(10) << "acc";
			}
			else {
				cout << setw(10) << " ";
			}
		}
		if (contextFreeGrammar[i].shiftRecord.count("$") > 0) {
			if (contextFreeGrammar[i].shiftRecord["$"] > 0)
				cout << setw(10) << "S" + to_string(contextFreeGrammar[i].shiftRecord["$"]);
			else if (contextFreeGrammar[i].shiftRecord["$"] < 0)
				cout << setw(10) << "R" + to_string(0 - contextFreeGrammar[i].shiftRecord["$"]);
			else
				cout << setw(10) << "acc";
		}
		else {
			cout << setw(10) << " ";
		}
		for (j = 0; j < symbol[1].size(); j++) {
			if (contextFreeGrammar[i].shiftRecord.count(symbol[1][j]) > 0) {
				if (contextFreeGrammar[i].shiftRecord[symbol[1][j]] > 0)
					cout << setw(10) << contextFreeGrammar[i].shiftRecord[symbol[1][j]];
				else
					cout << setw(10) << 0 - contextFreeGrammar[i].shiftRecord[symbol[1][j]];
			}
			else {
				cout << setw(10) << " ";
			}
		}
		cout << endl;
		/*
		cout << setw(2) << i << endl;
		for (const auto& s_map : contextFreeGrammar[i].shiftRecord) {
			cout << "symbol: " << s_map.first << ", " << s_map.second << "\n";
		}*/
	}
}
int main() {
	ifstream fin_1, fin_2;
	string str = "2";
	fin_1.open("./input/" + str + "/" + str + "_grammar.txt");
	fin_2.open("./input/" + str + "/" + str + "_testdata.txt");
	if (!fin_1.is_open() || !fin_2.is_open())
		cout << "Can' t open the file!\n";
	else {
		symbol.resize(2); // 0 terminal, 1 terminal
		contextFreeGrammar.resize(1); // I0
		string str;
		int num = 0, i = 0, j = 0, k = 0, state_num_bef = 1, state_num_aft = 1;
		// 處理I0 但不包刮後面符號
		while (getline(fin_1, str)) {
			// 去除空格
			str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
			if (++num == 1)
				symbol[0] = split(str.substr(9), ",", 0);
			else if (num == 2)
				symbol[1] = split(str.substr(12), ",", 0);
			else {
				vector<string> temp1 = split(str, "->", 0), temp2 = split(temp1[1], "|", 0), temp3, temp4, temp5;
				if (num == 3) {
					contextFreeGrammar[0].contextFreeGrammar_list.resize(contextFreeGrammar[0].contextFreeGrammar_list.size() + 1);
					contextFreeGrammar[0].contextFreeGrammar_list.back().resize(1);
					contextFreeGrammar[0].contextFreeGrammar_header.resize(contextFreeGrammar[0].contextFreeGrammar_header.size() + 1);
					contextFreeGrammar[0].dot_index.resize(contextFreeGrammar[0].dot_index.size() + 1);
					contextFreeGrammar[0].add_line.resize(contextFreeGrammar[0].add_line.size() + 1);

					contextFreeGrammar[0].contextFreeGrammar_list.back().back() = temp1[0];
					contextFreeGrammar[0].contextFreeGrammar_header.back() = temp1[0] + "'";
					contextFreeGrammar[0].dot_index.back() = 0;
					contextFreeGrammar[0].add_line.back() = false;
				}
				for (i = 0; i < temp2.size(); i++) {
					contextFreeGrammar[0].contextFreeGrammar_list.resize(contextFreeGrammar[0].contextFreeGrammar_list.size() + 1);
					contextFreeGrammar[0].contextFreeGrammar_header.resize(contextFreeGrammar[0].contextFreeGrammar_header.size() + 1);
					contextFreeGrammar[0].dot_index.resize(contextFreeGrammar[0].dot_index.size() + 1);
					contextFreeGrammar[0].add_line.resize(contextFreeGrammar[0].add_line.size() + 1);

					temp3.resize(1);
					temp3.back() = temp2[i];
					for (j = 0; j < (symbol[0].size() + symbol[1].size()); j++) {
						for (k = 0; k < temp3.size(); k++) {
							j < symbol[0].size() ? temp4 = split(temp3[k], symbol[0][j], 1) : temp4 = split(temp3[k], symbol[1][j - symbol[0].size()], 1);
							temp5.insert(temp5.end(), temp4.begin(), temp4.end());
						}
						temp3 = temp5;
						temp5.clear();
					}
					contextFreeGrammar[0].contextFreeGrammar_list.back() = temp3;
					contextFreeGrammar[0].contextFreeGrammar_header.back() = temp1[0];
					contextFreeGrammar[0].dot_index.back() = 0;
					contextFreeGrammar[0].add_line.back() = false;
				}
			}
		}
		symbol_first.resize(symbol[1].size());
		for (i = 0; i < symbol_first.size(); i++) {
			symbol_first[i] = first_action(symbol[1][i]);
		}
		//處理I0尾巴
		contextFreeGrammar[0].contextFreeGrammar_footer.resize(contextFreeGrammar[0].contextFreeGrammar_list.size());
		contextFreeGrammar[0].contextFreeGrammar_footer[0].insert(contextFreeGrammar[0].contextFreeGrammar_footer[0].end(), "$");
		for (i = 1; i < contextFreeGrammar[0].contextFreeGrammar_list.size(); i++) {
			contextFreeGrammar[0].contextFreeGrammar_footer[i] = footer_action(contextFreeGrammar[0].contextFreeGrammar_header[i], i);
		}
		//處理I0 shift
		shift_action(0);
		// 處理因I0 shift而新增的In
		state_num_aft = contextFreeGrammar.size();
		while (state_num_aft != state_num_bef) {
			for (i = state_num_bef; i < state_num_aft; i++) {
				for (j = 1; j < contextFreeGrammar[i].contextFreeGrammar_list.size(); j++) {
					if (contextFreeGrammar[i].contextFreeGrammar_footer[j].size() == 0) {
						contextFreeGrammar[i].contextFreeGrammar_footer[j] = footer_action_2(contextFreeGrammar[i].contextFreeGrammar_header[j], j, i);
					}
				}
			}
			for (i = state_num_bef; i < state_num_aft; i++) {
				shift_action(i);
				//cout << "------------------------ I" << i << "  -------------------------\n";	
				//output(0, contextFreeGrammar.size());
			}
			state_num_bef = state_num_aft;
			state_num_aft = contextFreeGrammar.size();
		}
		for (i = 0; i < contextFreeGrammar.size(); i++) {
			for (j = i + 1; j < contextFreeGrammar.size(); j++) {
				if ((contextFreeGrammar[i].contextFreeGrammar_list == contextFreeGrammar[j].contextFreeGrammar_list)
					&& (contextFreeGrammar[i].contextFreeGrammar_header == contextFreeGrammar[j].contextFreeGrammar_header)
					&& (contextFreeGrammar[i].contextFreeGrammar_footer == contextFreeGrammar[j].contextFreeGrammar_footer)
					&& (contextFreeGrammar[i].add_line == contextFreeGrammar[j].add_line)
					&& (contextFreeGrammar[i].dot_index == contextFreeGrammar[j].dot_index)
					) {

					cout << "OHOHOHOHOHO HO the same with I" << i << " and I" << j << endl;
				}
			}
			reduce_action(i);
		}
		cout << "-------------------------------------------------state-------------------------------------------------\n";
		output(0, contextFreeGrammar.size());
		cout << "-------------------------------------------------state-------------------------------------------------\n\n-------------------------------------------------parsing table-------------------------------------------------\n";
		output_table();
		cout << "-------------------------------------------------parsing table-------------------------------------------------\n\n";
		while (getline(fin_2, str)) {
			testData_list.resize(testData_list.size() + 1);
			vector<string> temp2, temp3, temp4, temp5;
			vector<int> temp1;
			temp1.insert(temp1.end(), 0);
			temp2.insert(temp2.end(), "$");
			temp3.insert(temp3.end(), str);
			for (j = 0; j < (symbol[0].size() + symbol[1].size()); j++) {
				for (k = 0; k < temp3.size(); k++) {
					j < symbol[0].size() ? temp4 = split(temp3[k], symbol[0][j], 1) : temp4 = split(temp3[k], symbol[1][j - symbol[0].size()], 1);
					temp5.insert(temp5.end(), temp4.begin(), temp4.end());
				}
				temp3 = temp5;
				temp5.clear();
			}
			temp3.insert(temp3.end(), "$");
			testData_list.back().parsing_stack.insert(testData_list.back().parsing_stack.end(), temp1);
			testData_list.back().parsing_symbol.insert(testData_list.back().parsing_symbol.end(), temp2);
			testData_list.back().parsing_input.insert(testData_list.back().parsing_input.end(), temp3);
			testData_list.back().parsing_action.resize(1);
			testData_list.back().result = 0;
		}

		for (i = 0; i < testData_list.size(); i++) {
			for (j = 0; j < testData_list[i].parsing_input[0].size(); j++) {
				if ((testData_list[i].parsing_input[0][j] != "$") && (in_vector((testData_list[i].parsing_input[0][j]), symbol[0]) == -1)) {
					testData_list[i].result = 2;
					break;
				}
			}
		}

		for (i = 0; i < testData_list.size(); i++) {
			if (testData_list[i].result != 2)
				syntax_analysis(i);
		}

		for (i = 0; i < testData_list.size(); i++) {
			cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
			cout << "\nparsing: ";
			for (j = 0; j < testData_list[i].parsing_input[0].size() - 1; j++) {
				cout << testData_list[i].parsing_input[0][j];
			}

			if (testData_list[i].result == 2)
				cout << "\nInvalid character exist!\n";
			else {
				if (testData_list[i].result != 0) {
					cout << "\nresult: Invalid!\n\n";
				}
				else {
					cout << "\nresult: Valid!\n\n";
				}
				cout << setw(45) << setiosflags(ios::left) << "stack" << setw(45) << setiosflags(ios::left) << "symbol" << setw(30) << setiosflags(ios::left) << "input" << setw(4) << "action" << endl;
				for (j = 0; j < testData_list[i].parsing_stack.size(); j++) {
					for (k = 0; k < testData_list[i].parsing_stack[j].size(); k++) {
						cout << setw(3) << testData_list[i].parsing_stack[j][k];
					}
					if (testData_list[i].parsing_stack[j].size() < 15)
						cout << setw(45 - 3 * testData_list[i].parsing_stack[j].size()) << " ";
					else
						cout << "    ";
					for (k = 0; k < testData_list[i].parsing_symbol[j].size(); k++) {
						cout << setw(3) << testData_list[i].parsing_symbol[j][k];
					}
					if (testData_list[i].parsing_symbol[j].size() < 15) {
						cout << setw(45 - 3 * testData_list[i].parsing_symbol[j].size()) << " ";
					}
					else
						cout << "    ";
					for (k = 0; k < testData_list[i].parsing_input[j].size(); k++) {
						cout << setw(3) << testData_list[i].parsing_input[j][k];
					}
					if (testData_list[i].parsing_input[j].size() < 10) {
						cout << setw(30 - 3 * testData_list[i].parsing_input[j].size()) << " ";
					}
					else {
						cout << "    ";
					}
					cout << setw(4) << testData_list[i].parsing_action[j] << endl;
				}
			}
		}
		cout << "end\n";
	}
};