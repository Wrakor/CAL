
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <stack>
#include <sstream>
#include "Tarefa.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;

const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = 999999999;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T>  > adj;
	bool visited;
	bool processing;
	bool instack;
	int indegree;
	int dist;
	bool done;
	/////////
	int index;
	int lowlink;
public:

	Vertex(T in);
	friend class Graph<T>;

	void addEdge(Vertex<T> *dest, double w);
	void addEdge(Vertex<T> *dest);
	bool removeEdgeTo(Vertex<T> *d);

	T getInfo() const;
	void setInfo(T info);
	void appendAdj(vector<Edge<T> > vec);
	vector<Edge<T> > getAdj();
	int getDist() const;
	int getIndegree() const;


	Vertex* path;
};

template <class T>
vector<Edge<T> > Vertex<T>::getAdj()
{
	return adj;
}

template <class T>
void Vertex<T>::appendAdj(vector<Edge<T> > vec) {
	adj.insert(adj.begin(), vec.begin(), vec.end());
}

template <class T>
struct vertex_greater_than {
	bool operator()(Vertex<T> * a, Vertex<T> * b) const {
		return a->getDist() > b->getDist();
	}
};
template <class T>
struct vertex_lesser_than {
	bool operator()(Vertex<T> * a, Vertex<T> * b) const {
		return a->getInfo() < b->getInfo();
	}
};

template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	d->indegree--; //adicionado do exercicio 5
	typename vector<Edge<T> >::iterator it= adj.begin();
	typename vector<Edge<T> >::iterator ite= adj.end();
	while (it!=ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

//atualizado pelo exerc�cio 5
template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false),done(false), processing(false), indegree(0), dist(0),index(0),lowlink(0),instack(false) {
	path = NULL;
}


template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest,w);
	adj.push_back(edgeD);
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest) {
	Edge<T> edgeD(dest);
	adj.push_back(edgeD);
}


template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
int Vertex<T>::getDist() const {
	return this->dist;
}


template <class T>
void Vertex<T>::setInfo(T info) {
	this->info = info;
}

template <class T>
int Vertex<T>::getIndegree() const {
	return this->indegree;
}




/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	Vertex<T> * dest;
	double weight;
public:
	Edge(Vertex<T> *d, double w);
	Edge(Vertex<T> *d);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w){}
template <class T>
Edge<T>::Edge(Vertex<T> *d): dest(d), weight(0){}




/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	void dfs(Vertex<T> *v, vector<T> &res) const;

	//exercicio 5
	int numCycles;

	void dfsVisit(Vertex<T> *v);
	void dfsVisit();
	void getPathTo(Vertex<T> *origin, list<T> &res);


	int index;
	stack<Vertex<T>* > pilha;





public:
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool addEdge(const T &sourc, const T &dest);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;

	//exercicio 5
	Vertex<T>* getVertex(const T &v) const;
	void resetIndegrees();
	vector<Vertex<T>*> getSources() const;
	int getNumCycles();
	vector<T> topologicalOrder();
	vector<T> getPath(const T &origin, const T &dest);
	void unweightedShortestPath(const T &v);
	bool isDAG();


	//projecto
	vector<vector<Vertex<T>* > > ciclos;
	vector< Vertex<T>* > order();
	void cycle();
	void strongconnect(Vertex<T> *v);
	void unifyCycles();



};

template<class T>
void Graph<T>::unifyCycles(){

	for (int i = 0; i < ciclos.size(); i++)
	{
		double priority = 0;

		for (int j = 0; j < ciclos[i].size(); j++)
		{
			priority += ciclos[i][j]->getInfo().getPriority();

			if (j > 0)
			{
				ciclos[i][0]->appendAdj(ciclos[i][j]->getAdj());
				removeVertex(ciclos[i][j]->getInfo());
			}
		}

		priority = priority/ciclos[i].size();
		round(priority);

		stringstream name;
		name << "Nova Tarefa ";
		name << i;
		Tarefa t(int(priority), name.str());
		ciclos[i][0]->setInfo(t);

	}

}

template<class T>
void Graph<T>::cycle(){


	pilha = stack<Vertex<T>* >();

	while(!pilha.empty()){
		pilha.pop();
	}


	for(int i=0; i< getNumVertex();i++){
		getVertexSet()[i]->visited = false;
		getVertexSet()[i]->instack = false;
	}

	for(int i=0; i< getNumVertex();i++){

		strongconnect(getVertexSet()[i]);

	}



}

template<class T>
void Graph<T>::strongconnect(Vertex<T> *v){

	stack<Vertex<T>* > pilha2 = stack<Vertex<T>* >();

	vector<Vertex<T> *> ciclo;




	if(!v->visited){

		cout <<v->getInfo().getName()<<" :"<< "not visited\n";
		pilha.push(v);
		cout <<v->getInfo().getName() << " adicionado � pilha\n";
		v->instack=true;
		v->visited=true;

		for(int i=0; i<v->adj.size();i++){
			strongconnect(v->adj[i].dest);
		}


		if (v==pilha.top()){
			cout << v->getInfo().getName() << " retirado da pilha e terminou\n";
			getVertex(pilha.top()->getInfo())->instack=false;
			pilha.pop();
			v->instack=false;
		}
		else{
			cout <<"Mau pop na "<< v->getInfo().getName()<<", terminou"<<endl;
		}
		//cout << "um" << pilha.size() << endl;
	}
	else if(!v->instack){

		cout <<v->getInfo().getName()<<" :"<< "visited/ not instack\n";


	}
	else{
		cout <<v->getInfo().getName()<<" :"<< "visited/ instack\n";
		if (pilha.empty())
			cout << "vazio" << endl;
		else
		{


			while (v!=pilha.top())
			{


				//cout << getVertex(pilha.top()->getInfo()).getName() << endl;


				if (pilha.size() > 1){
					ciclo.push_back(pilha.top());
					cout << pilha.top()->getInfo().getName()<< " adicionado a ciclo\n";
					getVertex(pilha.top()->getInfo())->instack=false;
					cout << pilha.top()->getInfo().getName() << " retirado da pilha\n" << endl;
					pilha2.push(pilha.top());
					cout << pilha.top()->getInfo().getName() <<" adicionado � fila\n";
					pilha.pop();

				}
				else
					break;
			}
			while(!pilha2.empty()){
				pilha.push(pilha2.top());
				cout << pilha2.top()->getInfo().getName()<< "adicionado novamente � pilha e retirado da pilha2\n";
				pilha2.pop();
			}


			ciclo.push_back(v);
			cout << v->getInfo().getName()<< "adicionado a ciclo\n";
			ciclos.push_back(ciclo);
			cout << "Ciclo Terminado"<<endl;
		}
		/*
		cout << "visited/instack\n";
		cout << "Pilha size: "<<pilha.size()<<endl;
		cout << pilha.top()->getInfo().getName() << " retirado da pilha\n";
		pilha.pop();
		cout << pilha.top()->getInfo().getName() << " retirado da pilha\n";
		pilha.pop();
		cout << pilha.top()->getInfo().getName() << " retirado da pilha\n";
		pilha.pop();
		 */
	}



}




template<class T>
vector< Vertex<T>* > Graph<T>::order(){

	static int numUndone;

	for(int i=0;i<vertexSet.size();i++){
		vertexSet[i]->done=false;
		numUndone++;
	}

	//static?
	static vector<Vertex<T>* > dep;
	static vector<Vertex<T>* > nodep;
	static vector<Vertex<T>* > order;

	bool check=false;

	while(numUndone>0){

		dep.clear();
		nodep.clear();

		//encontra todos os vertices com depend�ncias, que sao adicionados ao vector dep
		for(int i=0; i<vertexSet.size();i++){
			check=false;

			for(int j=0; j<vertexSet[i]->adj.size();j++){
				check=false;
				if(!vertexSet[i]->adj[j].dest->done){
					for(int k=0; k<dep.size();k++){
						if (vertexSet[i]->adj[j].dest==dep[k]){
							check=true;
							break;
						}
					}
				}

				if(!check && !vertexSet[i]->done){
					dep.push_back(vertexSet[i]->adj[j].dest);
				}

			}

		}


		//adiciona ao vector nodep as tarefas sem dependencias
		for(int i=0; i<vertexSet.size();i++){
			check = false;
			for(int j=0; j<dep.size();j++){
				if(vertexSet[i]==dep[j]){
					check=true;
					break;
				}

			}
			if(!check && !vertexSet[i]->done){
				nodep.push_back(vertexSet[i]);
			}

		}

		//executa a tarefa com maior prioridade que ja nao tem dependencias
		sort(nodep.begin(), nodep.end(),vertex_lesser_than<T>());
		Vertex<T>* v;
		v = nodep.back();

		getVertex(v->getInfo())->done = true;
		order.push_back(v);
		nodep.pop_back();

		numUndone--;
	}

	return order;
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}
template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
int Graph<T>::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}


template <class T>
bool Graph<T>::isDAG() {
	return (getNumCycles() == 0);
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	vertexSet.push_back(v1);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge<T> >::iterator itAdj= v->adj.begin();
			typename vector<Edge<T> >::iterator itAdje= v->adj.end();
			for (; itAdj!=itAdje; itAdj++) {
				itAdj->dest->indegree--;
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
		{ vS=*it; found++;}
		if ( (*it)->info == dest )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD,w);

	return true;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
		{ vS=*it; found++;}
		if ( (*it)->info == dest )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD);

	return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
		{ vS=*it; found++;}
		if ( (*it)->info == dest )
		{ vD=*it; found++;}
		it ++;
	}
	if (found!=2)
		return false;

	vD->indegree--;

	return vS->removeEdgeTo(vD);
}




template <class T>
vector<T> Graph<T>::dfs() const {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<T> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->visited==false )
			dfs(*it,res);
	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const {
	v->visited = true;
	res.push_back(v->info);
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++)
		if ( it->dest->visited == false ){
			dfs(it->dest, res);
		}
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
}


template <class T>
Vertex<T>* Graph<T>::getVertex(const T &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->info == v) return vertexSet[i];
	return NULL;
}

template<class T>
void Graph<T>::resetIndegrees() {
	//colocar todos os indegree em 0;
	for(unsigned int i = 0; i < vertexSet.size(); i++) vertexSet[i]->indegree = 0;

	//actualizar os indegree
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		//percorrer o vector de Edges, e actualizar indegree
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++) {
			vertexSet[i]->adj[j].dest->indegree++;
		}
	}
}


template<class T>
vector<Vertex<T>*> Graph<T>::getSources() const {
	vector< Vertex<T>* > buffer;
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		if( vertexSet[i]->indegree == 0 ) buffer.push_back( vertexSet[i] );
	}
	return buffer;
}


template <class T>
void Graph<T>::dfsVisit() {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
		if ( (*it)->visited==false )
			dfsVisit(*it);
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v) {
	v->processing = true;
	v->visited = true;
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++) {
		if ( it->dest->processing == true) numCycles++;
		if ( it->dest->visited == false ){
			dfsVisit(it->dest);
		}
	}
	v->processing = false;
}

template<class T>
vector<T> Graph<T>::topologicalOrder() {
	//vector com o resultado da ordenacao
	vector<T> res;

	//verificar se � um DAG
	if( getNumCycles() > 0 ) {
		cout << "Ordenacao Impossivel!" << endl;
		return res;
	}

	//garantir que os "indegree" estao inicializados corretamente
	this->resetIndegrees();

	queue<Vertex<T>*> q;

	vector<Vertex<T>*> sources = getSources();
	while( !sources.empty() ) {
		q.push( sources.back() );
		sources.pop_back();
	}

	//processar fontes
	while( !q.empty() ) {
		Vertex<T>* v = q.front();
		q.pop();

		res.push_back(v->info);

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			v->adj[i].dest->indegree--;
			if( v->adj[i].dest->indegree == 0) q.push( v->adj[i].dest );
		}
	}

	//testar se o procedimento foi bem sucedido
	if ( res.size() != vertexSet.size() ) {
		while( !res.empty() ) res.pop_back();
	}

	//garantir que os "indegree" ficam atualizados ao final
	this->resetIndegrees();

	return res;
}



template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest){

	list<T> buffer;
	Vertex<T>* v = getVertex(dest);

	//cout << v->info << " ";
	buffer.push_front(v->info);
	while ( v->path != NULL &&  v->path->info != origin) {
		v = v->path;
		buffer.push_front(v->info);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->info);


	vector<T> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}


template<class T>
void Graph<T>::unweightedShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;
			if( w->dist == INT_INFINITY ) {
				w->dist = v->dist + 1;
				w->path = v;
				q.push(w);
			}
		}
	}
}


#endif /* GRAPH_H_ */
