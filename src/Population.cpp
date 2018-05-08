#include "Population.hpp"

using namespace std;

Population::Population()
{
    this->enable_growth = false;
}

Population::Population(int in_size, int out_size, int pop_size) : Population()
{
    this->input_size = in_size;
    this->output_size = out_size;
    this->population_size = pop_size;

    auto g = generator_first_organism();
    this->spawn(g, population_size);
    this->separate_species();
    this->calculate_all_fitness();
    this->sort_all_organism();
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
        g = new_genome;
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
    this->reproduce();
    this->natural_seletion();

    if (this->IsEnableGroth())
        this->organism_growth_up(); //所有神經網路進行訓練
}

void Population::reproduce_agitation()
{
    double agitation_rate = 0.4;

    vector<std::shared_ptr<Organism>> new_orgs;

    for (auto const &org : this->organisms)
    {
        double agitation_probability = NEAT::randfloat();

        if (agitation_probability < agitation_rate)
        {
            auto new_org = org->clone();

            new_org->harass();
            new_orgs.push_back(new_org);
        }
    }

    for (auto const &new_org : new_orgs)
        this->organisms.push_back(new_org);
}

void Population::organism_growth_up()
{
    cout << "[growing up]\t";

#pragma omp parallel for
    for (int i = 0; i < (int)this->organisms.size(); i++)
        this->organisms[i]->growthUp();
}

void Population::calculate_new_organisms_fitness()
{

#pragma omp parallel for
    for (int i = 0; i < (int)this->new_organisms_pool.size(); i++)
    {
        auto new_org = this->new_organisms_pool[i]->clone();
        new_org->evolution();
        this->organisms.push_back(new_org);
    }

    this->new_organisms_pool.clear();
}

void Population::calculate_all_fitness()
{

#pragma omp parallel for
    for (int i = 0; i < (int)this->organisms.size(); i++)
        this->organisms[i]->evolution();
}

void Population::reproduce_to_pool()
{
    this->reproduce_pool.clear();
    sort(this->organisms.begin(), this->organisms.end(), organisms_order_by_fitness_and_race); //依照fitness 進行排序

    int org_score = this->organisms.size();
    //int reproduce_size = org_size * reproduce_rate;
    vector<std::shared_ptr<Organism>> reproduce_roulette;
    for (auto const &org : this->organisms)
    {
        for (int i = 0; i < org_score; i++)
            reproduce_roulette.push_back(org->clone());
        org_score--; //下一個降一分
    }

    //cout << "reproduce_roulette size : " << reproduce_roulette.size() << endl;

    //開始輪盤遊戲
    int reproduce_pair_size = (this->organisms.size() * NEAT::reproduce_rate) / 2;
    for (int i = 0; i < reproduce_pair_size; i++)
    {
        int idx_parent1 = NEAT::randint(0, reproduce_roulette.size() - 1);
        int idx_parent2 = NEAT::randint(0, reproduce_roulette.size() - 1);

        this->reproduce_pool.push_back(reproduce_roulette[idx_parent1]->clone()); //複製一份一模一樣的進入交配池
        this->reproduce_pool.push_back(reproduce_roulette[idx_parent2]->clone()); //複製一份一模一樣的進入交配池
    }

    reproduce_roulette.clear(); //清空暫存輪盤
}

void Population::reproduce()
{
    this->reproduce_to_pool();
    // cout << "crossover start." << endl;
    this->crossover();
    // cout << "mutation start." << endl;
    this->mutation();
    // cout << "mutation done." << endl;
    this->calculate_new_organisms_fitness(); //計算新生物種們的適應值
}

void Population::crossover()
{
    // cout << "cross-over " << (int)this->reproduce_pool.size() << endl;
    for (int i = 1; i < (int)this->reproduce_pool.size(); i += 2)
    {
        // auto offspring = crossover(this->applyGenomeId(), orgs[i - 1]->getGenome(), orgs[i]->getGenome());
        auto parent1 = this->reproduce_pool[i - 1];
        auto parent2 = this->reproduce_pool[i];
        auto offspring = parent1->crossover(applyGemoneId(), parent2);
        this->new_organisms_pool.push_back(offspring->clone());
    }
}

void Population::mutation()
{
    for (auto const &org : this->reproduce_pool)
    {
        double val = NEAT::randfloat();
        if (val < NEAT::mutation_link)
        {
            auto mutation_org = org->clone();
            mutation_org->mutationLink();

            this->new_organisms_pool.push_back(mutation_org);
        }

        val = NEAT::randfloat();
        if (val < NEAT::mutation_node)
        {
            auto mutation_org = org->clone();
            mutation_org->mutationNode();

            this->new_organisms_pool.push_back(mutation_org);
        }
    }
}

void Population::natural_seletion()
{
    // this->separate_species();
    this->sort_all_organism();
    int remove_size = this->organisms.size() - this->population_size;

    if (remove_size <= 0)
        return; //無須淘汰

    for (int i = 0; i < remove_size; i++)
        this->organisms.pop_back();
}

void Population::sort_all_organism()
{
    sort(this->organisms.begin(), this->organisms.end(), organisms_order_by_fitness_and_race); //依照fitness 進行排序
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

void Population::enableGrowthState()
{
    cout << "enableGrowthState" << endl;
    this->enable_growth = true;
}

void Population::disableGrothState()
{
    cout << "disableGrothState" << endl;
    this->enable_growth = false;
}

bool Population::IsEnableGroth()
{
    return this->enable_growth;
}

void Population::save_best_organism(char *filename)
{
    auto best_org = this->organisms[0];
    auto model_saver = make_unique<GeneModelSaver>(best_org->getGenome());
    string file_name(filename);
    model_saver->Save(file_name);
}

void Population::showInfo()
{
    //    cout << "****************Population****************" << endl;
    //    cout << "* organisms size " << this->organisms.size() << endl;
    //    cout << "* genome id " << this->genome_id << endl;
    //    cout << "******************************************" << endl;

    auto org = this->organisms[0];
    cout << "* evoluation..."
         << "[INFO] Best organism [" << setw(3) << setfill('0') << org->getOrganismId()
         << "]-> fitness(loss) = " << setw(8) << setprecision(8) << org->getFitness()
         << "\ttrain accuracy = " << setw(8) << setprecision(8) << org->getTrainAccuracy()
         << "\tvalid accuracy = " << setw(8) << setprecision(8) << org->getAccuracy()
         << " * organisms size " << this->organisms.size() << endl;

    // for (auto const &org : this->organisms)
    // {
    //     cout << "* evoluation..."
    //          << "\t[INFO] organism [" << setw(3) << setfill('0') << org->getOrganismId()
    //          << "]-> fitness(loss) = " << setprecision(8) << org->getFitness()
    //          << "\ttrain accuracy = " << setprecision(8) << org->getTrainAccuracy()
    //          << "\tvalid accuracy = " << setprecision(8) << org->getAccuracy() << endl;
    // }
}

bool organisms_order_by_fitness_and_race(std::shared_ptr<Organism> i, std::shared_ptr<Organism> j)
{
    //return (i->getFitness() > j->getFitness());   //max -> min
    if (i->getFitness() == j->getFitness())
        return i->getOrganismId() < j->getOrganismId();
    else
        return (i->getFitness() < j->getFitness()); //min -> max
}
