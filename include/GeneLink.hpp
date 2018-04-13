#ifndef TYGR_GENE_LINK_HPP
#define TYGR_GENE_LINK_HPP

#include "GeneNode.hpp"
#include "NerveNetwork.hpp"
#include <memory>
#include "NEAT.hpp"
// typedef struct
// {
//     int innovationId;
//     int inNodeId;
//     int outNodeId;
//     double weight;
// } GENE_LINK;

class GeneLink
{

    friend class NerveNetwork;
    friend class GeneInfoController;

  private:
    int innovationId;
    int inNodeId;
    int outNodeId;
    double weight;
    bool isEnable;
    int mutationCount;

  public:
    GeneLink(int innov, int in_node_id, int out_node_id, double weight);
    ~GeneLink();
    void harassWeight();
    int InnovationId();
    int getMutationCount();
    std::shared_ptr<GeneLink> clone();
};

#endif