#ifndef TYGR_GENOME_HPP
#define TYGR_GENOME_HPP

#include <vector>
#include <memory>
#include "GeneNode.hpp"
#include "GeneLink.hpp"
#include "NerveNetwork.hpp"
#include "Organism.hpp"
#include "NEAT.hpp"

class GeneNode;
class GeneLink;
class Organism;

class Genome
{
  friend class NerveNetwork;
  friend class Organism;
  int genomme_id;

protected:
  std::vector<std::shared_ptr<GeneNode>> nodes;
  std::vector<std::shared_ptr<GeneLink>> links;

  // void mutationNode(std::shared_ptr<GeneNode> new_node);
  // void mutationLink(std::shared_ptr<GeneLink> new_link);

  std::shared_ptr<GeneNode> mutationNode();

public:
  Genome();
  Genome(int g_id, std::vector<std::shared_ptr<GeneNode>> nodes, std::vector<std::shared_ptr<GeneLink>> links);
  ~Genome();

  double compatibility(std::shared_ptr<Genome> g);
  int getGenommeId();
  std::shared_ptr<Genome> duplicate(int new_genome_id);
  std::shared_ptr<Genome> clone();
};

#endif