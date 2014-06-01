#include "theme.h"

string Theme::getName()
{
	return name;
}

Theme::Theme(string name)
{
	this->name=name;
}

vector<string> Theme::getWords()
{
	return words;
}

void Theme::addWord(string word)
{
	words.push_back(word);
}
