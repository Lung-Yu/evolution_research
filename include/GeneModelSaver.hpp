#ifndef TYGR_GENE_MODEL_SAVER_HPP
#define TYGR_GENE_MODEL_SAVER_HPP

#include <iostream>
#include <fstream>
#include <memory>
#include <cstring>
#include <string>
#include "Genome.hpp"

class GeneModelSaver
{
  private:
    std::shared_ptr<Genome> genome;
  public:
    GeneModelSaver(std::shared_ptr<Genome> genome);
    ~GeneModelSaver();

    void Save(std::string& filename);
};

#endif