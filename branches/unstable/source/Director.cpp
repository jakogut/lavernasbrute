//Part of Laverna's Brute

#include "Director.h"

struct Node
{
	std::string username;
	std::string target;

	std::string status;

	unsigned long long startKeyspace, endKeyspace, keyLocation;

	//Used to determine which clients get priority for larger keyspace portions
	unsigned int averageSpeed;
};

Director::Director(std::string mode)
: mode(mode)
{
	Director::nodes = new Node[];
}

Director::~Director()
{
}

void Director::addNode(int id, std::string hash, std::string username)
{
	nodes[id].username = username;
	nodes[id].target = hash;

	nodes[id].status = "RUNNING";

	assignKeyspace(id, nodes[id].startKeyspace, nodes[id].endKeyspace);
}

void Director::addNode(int id)
{
	nodes[id].username = "EMPTY";
	nodes[id].target = "EMPTY";

	nodes[id].status = "RUNNING";

	assignKeyspace(id, nodes[id].startKeyspace, nodes[id].endKeyspace);
}

void Director::removeNode(int id)
{
}

void Director::assignKeyspace(int id, unsigned long long begin, unsigned long long end)
{
}