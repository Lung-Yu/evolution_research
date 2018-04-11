
#include <memory>

#include "Genome.hpp"
#include "Innovation.hpp"
#include "GeneLink.hpp"
#include "GeneNode.hpp"
#include "NEAT.hpp"

int main(){

    // auto innovation = std::make_unique<Innovation>();
    // std::vector<GeneNode> nodes;
    // std::vector<GeneLink> links;

    // GeneNode node_in;
    // node_in.node_id = innovation->applyNodeInnovation();
    // node_in.node_type = NODE_TYPE::Sensor;
    // node_in.func_type = NODE_FUNC_TYPE::Identity;
    // nodes.push_back(node_in);

    // GeneNode node_out;
    // node_out.node_id = innovation->applyNodeInnovation();
    // node_out.node_type = NODE_TYPE::Output;
    // node_out.func_type = NODE_FUNC_TYPE::Identity;
    // nodes.push_back(node_out);

    // GeneNode link;
    // link.innovationId = innovation->applyLinkInnovation();
    // link.inNodeId = node_in.node_id;
    // link.outNodeId = node_out.node_id;
    // link.weight = randfloat();
    // links.push_back(link);

    // std::shared_ptr<Genome> g = std::make_unique<Genome>(nodes,links);
    // std::shared_ptr<Genome> g2 = g->clone();

    // std::cout << "g1 " << g <<std::endl;
    
    // std::cout << "g2 " << g2 <<std::endl;
    

    return 0;
}

double randfloat()
{
    return rand() / (double)RAND_MAX;
}
