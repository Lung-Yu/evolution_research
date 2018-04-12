#include "Population.hpp"

using namespace std;

Population::Population()
{
    this->innovation = make_unique<Innovation>();
}
Population::Population(int in_size, int out_size, int g_size, int pop_size) : Population()
{
    this->input_size = in_size;
    this->output_size = out_size;
    this->generate_size = g_size;
    this->population_size = pop_size;

    auto g = generator_first_organism();
    this->spawn(g, g_size);
}

Population::~Population()
{
}

shared_ptr<Genome> Population::generator_first_organism()
{
    auto g = generator_fully_connection_genome();
    auto org = make_shared<Organism>(g);
    this->putOrganism(org);
    return g;
}

shared_ptr<Genome> Population::generator_fully_connection_genome()
{
    //building input nodes
    for (int i = 0; i < this->input_size; i++)
    {
        auto node = make_shared<GeneNode>(this->innovation->applyNodeInnovation(), NODE_TYPE::Sensor, NODE_FUNC_TYPE::Identity);
        this->putNode(node);
    }

    //building output nodes
    for (int i = 0; i < this->output_size; i++)
    {
        auto node = make_shared<GeneNode>(this->innovation->applyNodeInnovation(), NODE_TYPE::Output, NODE_FUNC_TYPE::Sigmoid);
        this->putNode(node);
    }

    //building links
    int outNodeOffset = this->input_size;

    for (int i = 0; i < this->input_size; i++)
        for (int j = 0; j < this->input_size; j++)
        {
            auto node_1 = this->getNodeId(i);
            auto node_2 = this->getNodeId(outNodeOffset + j);
            auto link = make_shared<GeneLink>(this->innovation->applyLinkInnovation(), node_1, node_2, 1);
            this->putLink(link);
        }

    auto g = make_shared<Genome>(applyGemoneId(), this->nodes, this->links);
    return g;
}

void Population::spawn(std::shared_ptr<Genome> g, int size)
{
    for (int i = 1; i < size; i++)
    {
        //複製一個結構相同但節點內容與連結權重不同的基因組
        auto new_genome = g->duplicate(this->applyGemoneId());
        auto new_organism = std::make_shared<Organism>(new_genome);
        this->putOrganism(new_organism);
    }
}

void Population::separate_species()
{
    this->initializeSpeciesId();
    vector<shared_ptr<Organism>> race_represents;
    bool IsNotFoundRace = true;
    for (auto const &org : this->organisms)
    {
        //尋找所屬的種族
        IsNotFoundRace = true;
        int race_idx = 0;
        for (auto const &race_org : race_represents)
        {
            if (org->compatibility(race_org) < NEAT::compat_threshold)
            {
                org->species_id = race_idx;
                IsNotFoundRace = false;
            }
            else
                race_idx++;
        }
        //若未找到所屬的種族,則自建一個新的種族
        if (IsNotFoundRace)
        {
            int race_id = this->applySpeciesId();
            org->species_id = race_id;
        }
    }
}

void Population::evolution()
{

}

void Population::reproduce()
{
    
}

void Population::mutation()
{

}

void Population::crossover()
{
    
}

void Population::initializeSpeciesId()
{
    this->species_idx = 0;
}

int Population::applySpeciesId()
{
    return this->species_idx++;
}

int Population::applyGemoneId()
{
    return this->genome_id++;
}

void Population::putNode(std::shared_ptr<GeneNode> node)
{
    this->nodes.push_back(node);
}

void Population::putLink(std::shared_ptr<GeneLink> link)
{
    this->links.push_back(link);
}

int Population::getNodeId(int node_index)
{
    if (node_index < (int)this->nodes.size())
    {
        auto node = nodes[node_index];
        return node->getNodeId();
    }

    return -1;
}

void Population::putOrganism(std::shared_ptr<Organism> org)
{
    this->organisms.push_back(org);
}

void Population::showInfo()
{
    cout << "****************Population****************" << endl;
    cout << "* organisms size " << this->organisms.size() << endl;
    cout << "* genome id " << this->genome_id << endl;
    cout << "******************************************" << endl;
}