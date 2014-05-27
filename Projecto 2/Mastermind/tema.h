/*
 * tema.h
 *
 *  Created on: 26 de Mai de 2013
 *      Author: Jorge Lima
 */

#ifndef TEMA_H_
#define TEMA_H_

#include <string>
#include <vector>
#include "word.h"

class Tema{

private:
	string name;
	vector <Word> words;

public:
	Tema(string name);
	string getName();
	vector <Word> getWords();
	void addWords(string word);

};




#endif /* TEMA_H_ */
