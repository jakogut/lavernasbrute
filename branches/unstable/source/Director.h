//Part of Laverna's Brute

#ifndef DIRECTOR_H_
#define DIRECTOR_H_

#include <string>

struct Node;

class Director
{
public:

	/* Possible modes are 'continuous' and 'fixed'
	Under the 'continuous' mode, the director will assign portions of the keyspace along with a list of targets
	until there are no more targets, at which point it will wait for more. Under the 'fixed' mode, the director
	will assign portions of the keyspace along with a given, fixed list of targets, and exit when there are no more. */

	Director(std::string mode);
	~Director();

	void operator()();

protected:

	//It is optional for new clients to provide a target hash
	void addNode(int id, std::string hash, std::string username);
	void addNode(int id);

	void removeNode(int id);

	void assignKeyspace(int id, unsigned long long begin, unsigned long long end);

	int numNodes;
	Node* nodes;

	std::string mode;
};

#endif