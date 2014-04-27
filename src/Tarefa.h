#ifndef TAREFA_H_
#define TAREFA_H_

#include <iostream>
#include <string>

using namespace std;

class Tarefa{

	int priority;
	string name;

public:
	Tarefa(int priority, string name);
	string getName();
	int getPriority();
	bool operator==(const Tarefa t);
	bool operator<(const Tarefa &t) const;
};



#endif /* TAREFA_H_ */
