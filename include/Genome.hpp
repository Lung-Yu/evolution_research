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
  int genomme_id;

protected:
  std::vector<std::shared_ptr<GeneNode>> nodes;
  std::vector<std::shared_ptr<GeneLink>> links;

public:
  Genome();
  Genome(int g_id, std::vector<std::shared_ptr<GeneNode>> nodes, std::vector<std::shared_ptr<GeneLink>> links);
  ~Genome();

  double compatibility(std::shared_ptr<Genome> g);
  std::shared_ptr<Genome> duplicate(int new_genome_id);
  std::shared_ptr<Genome> clone();
};

#endif