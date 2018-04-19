#include "Population.hpp"

using namespace std;

Population::Population()
{
}
Population::Population(int in_size, int out_size, int g_size, int pop_size) : Population()
{
    this->input_size = in_size;
    this->output_size = out_size;
    this->generate_size = g_size;
    this->population_size = pop_size;

    auto g = generator_first_organism();
    this->spawn(g, g_size);
    this->separate_species();
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
    for (int i = 0; i < this->input_size; i++)
        GeneInfoController::getInstance()->applyNewInputGeneNode();

    for (int i = 0; i < this->output_size; i++)
        GeneInfoController::getInstance()->applyNewOutputGeneNode();

    for (auto const &in_node : GeneInfoController::getInstance()->getInputNodes())
    {
        for (auto const &out_node : GeneInfoController::getInstance()->getOutputNodes())
        {
            auto node_src = in_node->getNodeId();
            auto node_dst = out_node->getNodeId();

            GeneInfoController::getInstance()->applyNewGeneLink(node_src, node_dst);
        }
    }

    auto g = make_shared<Genome>(applyGemoneId(),
                                 GeneInfoController::getInstance()->getAllNodes(),
                                 GeneInfoController::getInstance()->getAllLinks());
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
            race_represents.push_back(org);
        }
    }
}

void Population::evolution()
{
    // cout << "evolution start." << endl;
    // cout << "reproduce start." << endl;
    this->reproduce();
    // cout << "crossover start." << endl;
    this->crossover();
    // cout << "mutation start." << endl;
    this->mutation();
    // cout << "natural_seletion start." << endl;
    this->natural_seletion();
    // cout << "evolution end." << endl;
}

void Population::reproduce()
{
    this->crossover_pool.clear();
    sort(this->organisms.begin(), this->organisms.end(), organisms_order_by_fitness_and_race); //依照fitness 進行排序

    const double reproduce_rate = 0.7;
    
    int org_score = this->organisms.size();
    //int reproduce_size = org_size * reproduce_rate;
    vector<std::shared_ptr<Organism>> reproduce_roulette;
    for (auto const &org : this->organisms)
    {
        for (int i = 0; i < org_score; i++)
            reproduce_roulette.push_back(org->clone());
        org_score--;    //下一個降一分
    }

    cout << "reproduce_roulette size : " << reproduce_roulette.size() << endl;

    //開始輪盤遊戲
    int reproduce_pair_size = (this->organisms.size() * reproduce_rate) / 2;
    for(int i=0;i<reproduce_pair_size;i++)
    {
        int idx_parent1 = NEAT::randint(0,reproduce_roulette.size() - 1);
        int idx_parent2 = NEAT::randint(0,reproduce_roulette.size() - 1);

        this->crossover_pool.push_back(reproduce_roulette[idx_parent1]->clone() ); //複製一份一模一樣的進入交配池
        this->crossover_pool.push_back(reproduce_roulette[idx_parent2]->clone() ); //複製一份一模一樣的進入交配池
    }
    reproduce_roulette.clear(); //清空暫存輪盤
}

void Population::mutation()
{
    for (auto const &org : this->crossover_pool)
    {
        double val = NEAT::randfloat();
        if (val < NEAT::mutation_link)
        {
            org->mutationLink();
        }

        val = NEAT::randfloat();
        if (val < NEAT::mutation_node)
        {
            org->mutationNode();
        }
    }
}

void Population::crossover()
{
    std::vector<std::shared_ptr<Organism>> offspring_genome_pool; //孫子的基因池

    for (int i = 1; i < (int)this->crossover_pool.size(); i += 2)
    {
        // auto offspring = crossover(this->applyGenomeId(), orgs[i - 1]->getGenome(), orgs[i]->getGenome());
        auto parent1 = crossover_pool[i - 1];
        auto parent2 = crossover_pool[i];
        auto offspring = parent1->crossover(applyGemoneId(), parent2);

        organisms.push_back(offspring);
    }

    this->crossover_pool.clear();
}

void Population::natural_seletion()
{
    this->separate_species();
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

    for (auto const &org : this->organisms)
    {
        cout << "* evoluation ";
        // org->evolution();
        cout << "org , race = " << org->species_id << ",fitness = " << org->getFitness() << endl;
    }

    cout << "* organisms size " << this->organisms.size() << endl;
}

bool organisms_order_by_fitness_and_race(std::shared_ptr<Organism> i, std::shared_ptr<Organism> j)
{
    if (i->species_id == j->species_id)
    {
        return (i->getFitness() < j->getFitness());
    }
    else
    {
        return (i->species_id < j->species_id);
    }
}