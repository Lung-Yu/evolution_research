#include "Genome.hpp"

using namespace std;

Genome::Genome()
{

}

Genome::Genome(vector<shared_ptr<GeneNode>> nodes, vector<shared_ptr<GeneLink>> links){
    this->links = links;
    this->nodes = nodes;
}

Genome::~Genome()
{

}

std::shared_ptr<Genome> Genome::clone()
{
    std::vector<shared_ptr<GeneNode>> new_nodes;
    for(auto const &node:this->nodes){
        auto new_node = node->clone();
        new_nodes.push_back(new_node);
    }

    vector<shared_ptr<GeneLink>> new_links;
    for(auto const &link : this->links){
        auto new_link = link->clone();
        new_links.push_back(new_link);
    }

    auto new_g = std::make_unique<Genome>(new_nodes,new_links);
    return std::move(new_g);
}