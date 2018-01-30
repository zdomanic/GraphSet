/* debugging example */

#include <iostream>
#include <vector>

class Node
{
    int value;
    std::vector<Node*> edges;
public:
    // insert constructors, destructors,
    // accessors, and mutators here
    bool dfs(int to_find);
};

// true indicates that the value was found, and false indicates that the value was not found.
bool Node::dfs(int to_find)
{
    if(this->value == to_find)
    {
        return true;
    }
    std::vector<Node*>::iterator i;
    for(i = this->edges.begin(); i != this->edges.end(); i++)
    {
        Node * n = *i;
        bool r = n->dfs(to_find);
        if(r)
        {
            return r;
        }
    }
    return false;
}

int main(void)
{
    // TODO: Write your test code here

    return 0;

}