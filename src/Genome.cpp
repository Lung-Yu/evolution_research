#include "Genome.hpp"

using namespace std;
Genome::Genome()
{
}

Genome::Genome(int g_id, vector<shared_ptr<GeneNode>> nodes, vector<shared_ptr<GeneLink>> links)
{
    this->genomme_id = g_id;
    this->links = links;
    this->nodes = nodes;
}

Genome::~Genome()
{
}

std::shared_ptr<Genome> Genome::clone()
{
    std::vector<shared_ptr<GeneNode>> new_nodes;
    for (auto const &node : this->nodes)
    {
        auto new_node = node->clone();
        new_nodes.push_back(new_node);
    }

    vector<shared_ptr<GeneLink>> new_links;
    for (auto const &link : this->links)
    {
        auto new_link = link->clone();
        new_links.push_back(new_link);
    }

    auto new_g = std::make_unique<Genome>(this->genomme_id, new_nodes, new_links);
    return std::move(new_g);
}

std::shared_ptr<Genome> Genome::duplicate(int new_genome_id)
{
    std::vector<shared_ptr<GeneNode>> new_nodes;
    for (auto const &node : this->nodes)
    {
        auto new_node = node->clone();
        new_nodes.push_back(new_node);
    }

    vector<shared_ptr<GeneLink>> new_links;
    for (auto const &link : this->links)
    {
        auto new_link = link->clone();
        new_link->harassWeight();
        new_links.push_back(new_link);
    }

    auto new_g = std::make_unique<Genome>(new_genome_id, new_nodes, new_links);
    return std::move(new_g);
}

double Genome::compatibility(std::shared_ptr<Genome> g)
{
    //Set up the counters
    int count_disjoint = 0;
    int count_excess = 0;
    int count_match = 0; //Used to normalize mutation_num differences
    int count_mutation_diff_total = 0;

    int self_gene_size = this->links.size();
    int comp_gene_size = g->links.size();


    return 0;
}