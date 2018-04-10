#ifndef TYGR_GENOME_HPP
#define TYGR_GENOME_HPP

#include <vector>
#include <memory>
#include "GeneNode.hpp"
#include "GeneLink.hpp"
#include "NerveNetwork.hpp"

class GeneNode;
class GeneLink;

class Genome
{
    friend class NerveNetwork;

  protected:
    std::vector<std::shared_ptr<GeneNode>> nodes;
    std::vector<std::shared_ptr<GeneLink>> links;

  public:
    Genome();
    Genome(std::vector<std::shared_ptr<GeneNode>> nodes, std::vector<std::shared_ptr<GeneLink>> links);
    ~Genome();

    std::shared_ptr<Genome> clone();
};

#endif