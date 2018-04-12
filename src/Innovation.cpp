#include "Innovation.hpp"

using namespace std;

shared_ptr<Innovation> Innovation::_instance = nullptr;

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

std::shared_ptr<Innovation> Innovation::getInstance()
{

    if (Innovation::_instance == nullptr)
    {
        auto ptr = new Innovation();
        Innovation::_instance.reset(ptr);
    }
    return _instance;
}

void Innovation::showInfo()
{
    cout << "******Innovation Info******" <<endl;
    cout << "* link innovation : " <<  this->link_innovtion <<endl;
    cout << "* node innovation : " <<  this->node_innovtion <<endl;
    cout << "***************************" << endl;
}