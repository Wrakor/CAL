#include "Tarefa.h"

Tarefa::Tarefa(int p, string n)
{
	priority=p;
	name=n;
}

string Tarefa::getName()
{
	return name;
}

bool Tarefa::operator==(const Tarefa t)
{
	if(priority==t.priority&&name==t.name)
		return true;
	return false;
}

bool Tarefa::operator<(const Tarefa &t) const
{
	if(priority<t.priority)
		return true;
	return false;
}
