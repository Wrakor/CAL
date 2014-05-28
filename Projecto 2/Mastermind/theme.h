#ifndef THEME_H_
#define THEME_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

class Theme{

private:
	string name;
	vector <string> words;

public:
	Theme(string name);
	string getName();
	vector <string> getWords();
	void addWord(string word);
};




#endif
