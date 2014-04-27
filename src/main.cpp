#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

#include "Graph.h"
using namespace std;

void readGraph(Graph<Tarefa> &graph){

	cout << "--------------------------------------" << endl;
	cout << "Inicio da leitura do ficheiro de texto" << endl;
	cout << "--------------------------------------" << endl;

	ifstream file;
	file.open("graph.txt");
	string line;

	if (file.is_open())
	{
		getline(file, line);

		if (line == "Vertex:")
		{
			while (!file.eof())
			{
				getline(file, line);

				if (line == "Edge:")
					break;
				else
				{
					stringstream ss(line);
					string priority, name;

					ss >> priority; //extrai o primeiro token de line e poe em priority
					name = line.substr(priority.size()+1); //o nome da tarefa

					graph.addVertex(Tarefa(atoi(priority.c_str()), name));
					cout << "Criado vértice com prioridade " << priority << " e nome \"" << name << "\"." << endl;
				}
			}

			if (line == "Edge:")
			{
				cout << endl;

				while (!file.eof())
				{
					getline(file, line);

					stringstream ss(line);
					string source, dest;
					int sourceInt, destInt;


					ss >> source; //extrai o primeiro token de line e poe em source
					dest = line.substr(source.size()+4); //poe em dest o que vem a seguir a "->"

					//converter o numero das tarefas para inteiros
					sourceInt = atoi(source.c_str());
					destInt = atoi(dest.c_str());

					//obter as tarefas
					Tarefa sourceT = graph.getVertexSet()[sourceInt-1]->getInfo();
					Tarefa destT = graph.getVertexSet()[destInt-1]->getInfo();

					//adicionar aresta
					graph.addEdge(sourceT, destT);

					cout << "Adicionada aresta entre tarefa \"" << sourceT.getName() << "\" e \"" << destT.getName() << "\"."<< endl;
				}
			}
		}
	}

	cout << "--------------------------------------" << endl;
	cout << "Fim da leitura do ficheiro de texto" << endl;
	cout << "--------------------------------------" << endl;
	file.close();
}

int main (){
	Graph<Tarefa> graph;


	readGraph(graph);


	//vector<vector<Vertex<Tarefa>*> > ciclos;


	//graph.cycle();


	for(int i=0; i<graph.ciclos.size();i++){
		cout<<endl;
		for(int j=0; j<graph.ciclos[i].size();j++){
			cout <<graph.ciclos[i][j]->getInfo().getName()<<endl;

		}
	}
	graph.unifyCycles();

	/*
	vector< Vertex<Tarefa>* > ciclo = graph.order();

	cout << "Ordenacao das tarefas:" << endl;
	for (unsigned int i = 0; i < ciclo.size(); i++)
	{
		cout << i+1 << " - " << ciclo[i]->getInfo().getName() << endl;
	}
	 */
	return 0;
}
