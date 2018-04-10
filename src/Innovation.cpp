#include "Innovation.hpp"

Innovation::Innovation()
{
    this->node_innovtion = 0;
    this->link_innovtion = 0;
}

Innovation::~Innovation()
{
}

int Innovation::applyNodeInnovation()
{
    return this->node_innovtion++;
}
int Innovation::applyLinkInnovation()
{
    return this->link_innovtion++;
}

void Innovation::showInfo()
{
    
}