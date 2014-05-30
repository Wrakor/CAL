#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include "theme.h"
using namespace std;

vector<Theme> readDictionary() {

	ifstream file("dictionary.txt");
	vector<Theme> themes;
	string line, palavra, themeName;

	if(file.is_open())
	{
		while(file.good())
		{
			getline(file, line);

			while(!file.eof())
			{
				themeName = line;
				Theme temp = Theme(themeName);

				getline(file, line);

				while(line != "")
				{
					palavra = line;
					temp.addWord(palavra);
					getline(file, line);
				}
				themes.push_back(temp);

				palavra = line;
				break;
			}
		}
	}
	else cout << "Unable to open file";

	file.close();

	return themes;
}

bool play(string word, Theme theme, int player) {

	bool win = false;
	vector<string> dictionary = theme.getWords();
	string tryWord, check[word.size()]; //tryWord - tentativa para adivinhar a palavra // check - guarda os resultado da tentativa com B, W ou _
	int play = 1, matchingLetters = 0;  //play - numero da jogada, termina ao fim de 10 //match - numero de letras certas, termina quando for igual ao tamanho da palavra
	map<char,int> letterCounter; //guarda a quantidade de cada letra. ex: canas - c=1; a=2 etc.

	//Usado apenas quando joga o computador:
	string existingLetters, nonExistingLetters; //letras existentes e nao existentes na palavra a adivinhar
	vector<string> usedWords = vector<string>(); //palavras ja usadas

	//inicializar check com espacos
	for(size_t i = 0; i < word.size(); i++){
		check[i]=" ";
	}

	while(play < 11 && matchingLetters != word.size()) {//cada iteraçao corresponde a uma jogada

		//limpar counter e match
		letterCounter.clear();
		matchingLetters = 0;

		cout<< "\nTentativa " << play << ":" << endl;

		if (player == 1) { //humano
			//print da palavra codificada (em _)
			for(size_t i=0; i < word.size(); i++)
				cout << "_ ";

			cout << "(" << word.size() << " letras)" << endl;

			cin >> tryWord;

			while (word.size() != tryWord.size()){ //se o tamanho for diferente nao aceita
				cout << "Numero de letras nao coincide! Tente outra vez:";
				cin >> tryWord;
			}
		}
		else if (player == 2) { //computador
			//primeira tentativa é a primeira plavra que encontrar com o mesmo numero de letras
			if (play == 1) {
				for (size_t i = 0; i < dictionary.size(); i++) {
					string temp = dictionary[i];

					if (temp.size() == word.size()) {
						tryWord = temp;
						usedWords.push_back(temp);

						cout << "Computador encontrou: " << temp << endl << endl;
						break;
					}
					else { //remover palavras que nao sao validas para acelerar a proxima pesquisa
						//dictionary[index].getWords().erase(dictionary[index].getWords().begin()+i);
						//i--;
					}
				}
			}
			else {
				//procurar palavras no dicionario tendo informacao da tentativa anterior (usando o check da iteracao anterior)
				for (size_t i = 0; i < dictionary.size(); i++) {
					string temp = dictionary[i];
					bool valid = true;

					//verifica se a palavra ja foi usada antes
					if (temp.size() == word.size()) {
						for (size_t j = 0; j < usedWords.size(); j++) {
							if (temp == usedWords[j])
								valid = false;
						}

						//verifica se as letras existentes (mas que nao sabemos a posicao) estao presentes na palavra
						for (size_t j = 0; j < existingLetters.size(); j++) {
							if (temp.find(existingLetters[j]) == std::string::npos) { //se nao estiverem nao e uma palavra valida
								valid = false;
								break;
							}
						}

						//verifica se as letras nao existentes estao presentes na palavra
						for (size_t j = 0; j < nonExistingLetters.size(); j++) {
							if (temp.find(existingLetters[j]) != std::string::npos) { //se estiverem nao e uma palavra valida
								valid = false;
								break;
							}
						}

						//verificar se as letras existentes (a quais ja sabemos a posicao) estao presentes na palavra
						for (size_t j = 0; j < word.size(); j++) {
							string s;
							s += temp[j];

							if (check[j] != "_" && check[j] != "W") //se for uma letra
								if (check[j] != s) { //se nao for igual
									valid = false;
									break;
								}
						}
					}
					else
						valid = false;

					if (valid) {
						tryWord = temp;
						cout << "Computador encontrou: " << tryWord << endl;
						usedWords.push_back(temp);
						break;
					}
					else { //remover palavras que nao sao validas para acelerar a proxima pesquisa
						//dictionary[index].getWords().erase(dictionary[index].getWords().begin()+i);
						//i--;
					}

				}
			}
		}

		//volta a por check "vazio"
		for(size_t i = 0; i < word.size(); i++){
			check[i]=" ";
		}
		existingLetters.clear();


		//actualiza o counter com a quantidade de cada letra
		for(size_t i = 0; i < word.size(); i++){
			if(!letterCounter.empty()){
				if(letterCounter.find(word[i]) == letterCounter.end()){ //a letra ainda nao foi inicializada no counter
					letterCounter[word[i]]=1;
				}else{ //a letra ja existe
					letterCounter[word[i]]+=1;
				}
			}else{//counter vazio
				letterCounter[word[i]]=1;
			}
		}

		//procura letras ja na posicao certa; actualiza o check e o counter e incrementa o match
		for(size_t i = 0; i < word.size(); i++){
			if(word[i]==tryWord[i]){
				check[i]="B";
				letterCounter[word[i]]--;
				matchingLetters++;
			}
		}

		if(matchingLetters < word.size()){
			//verifica o resto das letras (fora da posiçao e nao existentes)
			for(size_t i = 0; i < word.size(); i++){
				for( size_t j = 0; j < tryWord.size(); j++){
					if(i != j && word[i] == tryWord[j] && letterCounter[word[i]] > 0 && check[j] != "B"){//só conta como W se nao exceder o nr de letras da palavra
						check[j] = "W";
						letterCounter[word[i]]=letterCounter[word[i]]-1;
					}else if(check[j]==" "){//no caso de exceder o nr de letras, por exemplo, 2º a em canas é tratado como _ apesar de pertencer à palavra
						check[j]="_";

						//Usado para o computador:
						if (player == 2) {
							//verifica se existem mais letras iguais (para o caso de ter sido encontrada antes uma letra igual que foi classificada com W)
							bool exists = false;

							for (size_t k = 0; k < tryWord.size(); k++)
								if (tryWord[j] == tryWord[k])
									exists = true;

							//senao existirem, entao a palavra a acertar nao tem esta letra, sendo adicionada ao respectivo vetor
							if (!exists)
								nonExistingLetters += check[j];
						}
					}
				}
			}
		}

		//print da jogada
		for(size_t i = 0; i < tryWord.size(); i++){
			cout << tryWord[i] << " ";
		}
		cout<< endl;
		for(size_t i = 0; i < word.size(); i++){
			cout << check[i] << " ";
		}
		cout << endl << endl;

		if (player == 2) {
			//ve as letras marcadas com W e B e modifica o check e actualiza o existingLetters para ser usado pelo computador na proxima iteracao
			for (size_t i = 0; i < word.size(); i++) {
				if (check[i] == "B")
					check[i] = tryWord[i];
				else if (check[i] == "W")
					existingLetters += tryWord[i];
			}
		}


		//fim da jogada
		play++;

		if(matchingLetters == word.size()){
			win = true;
		}
	}

	//fim do jogo
	if(!win){
		cout<< "Game Over" << endl;
		cout<< "A palavra era : "<< word << endl;
	}else {
		cout << "Acertou!!" << endl;
	}

}

int main()
{
	vector<Theme> dictionary = readDictionary();
	string palavra, gameMode, player;
	int themeIndex, wordIndex, mode, playerNumber;
	srand (time(NULL));

	cout << "Escolha quem vai jogar MasterMind:" << endl;
	cout << "	1 - Humano" << endl;
	cout << "	2 - Computador" << endl;

	cin >> player;

	while (!(playerNumber = atoi(player.c_str())) || playerNumber < 1 || playerNumber > 2) {
		cout << "Input invalido! Escolha o jogador:";
		cin >> player;
	}

	cout << "Escolha o modo de jogo\n";
	cout << "	1-Theme aleatório\n";
	cout << "	2-Theme surpresa\n";
	cout << "	3-Escolher Tema\n";

	cin >> gameMode;

	while (!(mode = atoi(gameMode.c_str())) || mode > 3 || mode < 1) {
		cout << "Input invalido! Escolha o modo de jogo:";
		cin >> gameMode;
	}

	switch(mode){

	case 1:
		themeIndex = rand() % dictionary.size();
		wordIndex = rand() % dictionary[themeIndex].getWords().size();

		cout << "Tema: " << dictionary[themeIndex].getName()<<endl;
		palavra = dictionary[themeIndex].getWords()[wordIndex];
		break;

	case 2:
		themeIndex = rand() % dictionary.size();
		wordIndex = rand() % dictionary[themeIndex].getWords().size();
		palavra = dictionary[themeIndex].getWords()[wordIndex];
		break;

	case 3:
		for(int i = 0; i < dictionary.size(); i++){
			cout << i+1 <<": "<< dictionary[i].getName()<<endl;
		}

		cin >> gameMode;

		while (!(themeIndex = atoi(gameMode.c_str()))) {
			cout << "Input invalido! Escolha o tema:";
			cin >> gameMode;
		}

		themeIndex--;

		wordIndex = rand() % dictionary[themeIndex].getWords().size();
		cout << "Tema: " << dictionary[themeIndex].getName() << endl;
		cout << "Legenda:" << endl;
		cout << "	B - Black" << endl;
		cout << "	W - White" << endl;
		palavra = dictionary[themeIndex].getWords()[wordIndex];

		break;
	}

	if (playerNumber == 1)
		play(palavra, dictionary[themeIndex], 1);
	else
		play(palavra, dictionary[themeIndex], 2);

	return 0;
}

