#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const static string no_tag = "NOTAG";
const static string no_word = "NOWORD";

struct feat{
	string word;
	string tag;
	vector<string> feats;

	feat(){
		feats.resize(6);
	}
};

void split_bychar(const string& str, vector<string>& vec, const char separator);
void extract_pairs(const vector<string>& info, vector<feat>& pairs);
void save_pairs(ofstream& out, const vector<feat>& pairs);

int main(int argc, char* argv[]) {
	if (argc > 2) {
		ifstream giga_file(argv[1]);
		ofstream out(argv[2]);
		if (!out.is_open()) {
			cout << "out file open error" << endl;
			return 0;
		}
		string line;
		vector<string> info;
		vector<feat> pairs;
		if (giga_file.is_open()) {
			while (getline(giga_file, line)) {
				split_bychar(line, info, ' ');
				extract_pairs(info, pairs);
				save_pairs(out, pairs);
			}
			giga_file.close();
			out.close();
			cout << "extract ok" << endl;
		}
		else
			cout << "giga file open error." << endl;
	}
	else
		cout << "no args" << endl;
	getchar();
	return 0;
}

void split_bychar(const string& str, vector<string>& vec, const char separator = ' ') {
	//assert(vec.empty());
	vec.clear();
	string::size_type pos1 = 0, pos2 = 0;
	string word;
	while ((pos2 = str.find_first_of(separator, pos1)) != string::npos) {
		word = str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + 1;
		if (!word.empty())
			vec.push_back(word);
	}
	word = str.substr(pos1);
	if (!word.empty())
		vec.push_back(word);
}


void extract_pairs(const vector<string>& info, vector<feat>& pairs){
	pairs.clear();
	int maxsize = info.size();
	pairs.resize(maxsize);
	string cur_info;
	int pos;

	for (int idx = 0; idx < maxsize; idx++) {
		cur_info = info[idx];
		feat* cur_pair = &pairs[idx];
		pos = cur_info.find('_');
		cur_pair->word = cur_info.substr(0, pos);
		cur_pair->tag = cur_info.substr(pos + 1, -1);
	}

	string last_tag, cur_tag, next_tag;
	for(int idx = 0; idx < maxsize; idx++) {
		cur_info = info[idx];
		feat* cur_pair = &pairs[idx];
		cur_tag = cur_pair->tag;
		cur_pair->feats[0] = cur_tag;

		if (idx - 1 >= 0)
			cur_pair->feats[1] = info[idx - 1];
		else
			cur_pair->feats[1] = no_word + "_" + no_tag;

		if(idx - 2 >= 0)
			cur_pair->feats[2] = info[idx - 2];
		else
			cur_pair->feats[2] = no_word + "_" + no_tag;

		if(idx + 1 < maxsize)
			cur_pair->feats[3] = info[idx + 1];
		else
			cur_pair->feats[3] = no_word + "_" + no_tag;

		if(idx + 2 < maxsize)
			cur_pair->feats[4] = info[idx + 2];
		else
			cur_pair->feats[4] = no_word + "_" + no_tag;

		if (idx - 1 >= 0)
			last_tag = pairs[idx - 1].tag;
		else
			last_tag = no_tag;

		if (idx + 1 < maxsize)
			next_tag = pairs[idx + 1].tag;
		else
			next_tag = no_tag;
		cur_pair->feats[5] = last_tag + "_" + cur_tag + "_" + next_tag;
	}
}

void save_pairs(ofstream& out, const vector<feat>& pairs)  {
	int maxsize = pairs.size();
	for (int idx = 0; idx < maxsize; idx++) {
		const feat* cur_pair = &pairs[idx];
		for (int idy = 0; idy < 6; idy++) {
			out << cur_pair->word << " " << cur_pair->feats[idy] << endl;
		}
	}
	out.flush();
}
