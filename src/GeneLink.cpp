#include "GeneLink.hpp"

using namespace std;

GeneLink::GeneLink(int innov, int in_node_id, int out_node_id, double w)
{
    this->innovationId = innov;
    this->inNodeId = in_node_id;
    this->outNodeId = out_node_id;
    this->weight = w;
    this->isEnable = true;
}

GeneLink::~GeneLink()
{
}

int GeneLink::InnovationId()
{
    return this->innovationId;
}

std::shared_ptr<GeneLink> GeneLink::clone()
{
    return move(make_unique<GeneLink>(this->innovationId, this->inNodeId, this->outNodeId, this->weight));
}