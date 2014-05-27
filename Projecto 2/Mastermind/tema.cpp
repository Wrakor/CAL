/*
 * tema.cpp
 *
 *  Created on: 26 de Mai de 2013
 *      Author: Jorge Lima
 */

#include "tema.h"

string Tema::getName()
{
	return name;
}

Tema::Tema(string name)
{
	this->name=name;
}

vector<Word> Tema::getWords()
{
	return words;
}

void Tema::addWords(string word)
{
	Word aux;
	aux = Word(word);
	words.push_back(aux);
}
