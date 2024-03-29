#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include "graphviewer.h"

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
					cout << "Criado v�rtice com prioridade " << priority << " e nome \"" << name << "\"." << endl;
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



void drawGraph(Graph<Tarefa> graph)
{
	GraphViewer *gv = new GraphViewer(400, 400, true);

	gv->createWindow(600, 600);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");

	for(int i=0; i<graph.getNumVertex();i++){
		gv->addNode(i+1);
		gv->setVertexLabel(i+1,graph.getVertexSet()[i]->getInfo().getName());
		graph.getVertexSet()[i]->setID(i+1);
	}
	int edge=0;

	int id=0;
	for(int i=0; i<graph.getNumVertex();i++){
		for(unsigned int j=0;j<graph.getVertexSet()[i]->getAdj().size();j++){



			id= graph.getVertexSet()[i]->getAdj()[j].getDest()->getID();

			gv->addEdge(edge,i+1,id,EdgeType::DIRECTED);
			edge++;
		}
	}

	gv->rearrange();

	cin.ignore();
}


int main (){
	Graph<Tarefa> graph;

	readGraph(graph);
	drawGraph(graph);
	graph.findCycles();
	graph.unifyCycles();

	vector< Vertex<Tarefa>* > tarefas = graph.order();

	cout << endl << "Ordenacao das tarefas:" << endl;

	for (unsigned int i = 0; i < tarefas.size(); i++)
	{
		cout << i+1 << " - " << tarefas[i]->getInfo().getName() << endl;
	}

	return 0;
}


