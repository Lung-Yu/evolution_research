#include "GeneNode.hpp"

using namespace std;

GeneNode::GeneNode(int id, NODE_TYPE type, NODE_FUNC_TYPE func)
{
    this->node_id = id;
    this->node_type = type;
    this->func_type = func;
}
GeneNode::~GeneNode()
{
}
int GeneNode::getNodeId(){
    return this->node_id;
}

std::shared_ptr<GeneNode> GeneNode::clone()
{
    return move(make_unique<GeneNode>(this->node_id, this->node_type, this->func_type));
}