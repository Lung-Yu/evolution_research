#include "Population.hpp"

using namespace std;

Population::Population()
{
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

    // this->reproduce_agitation();

    this->calculate_all_fitness();
    this->natural_seletion();
    // best_organism = this->organisms[0]->clone();

    // cout << "evolution end." << endl;
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

#pragma omp parallel for
    for (int i = 0; i < (int)this->organisms.size(); i++)
        this->organisms[i]->growthUp();
}
void Population::calculate_all_fitness()
{

#pragma omp parallel for
    for (int i = 0; i < (int)this->organisms.size(); i++)
        this->organisms[i]->evolution();
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
        org_score--; //下一個降一分
    }

    //cout << "reproduce_roulette size : " << reproduce_roulette.size() << endl;

    //開始輪盤遊戲
    int reproduce_pair_size = (this->organisms.size() * reproduce_rate) / 2;
    for (int i = 0; i < reproduce_pair_size; i++)
    {
        int idx_parent1 = NEAT::randint(0, reproduce_roulette.size() - 1);
        int idx_parent2 = NEAT::randint(0, reproduce_roulette.size() - 1);

        this->crossover_pool.push_back(reproduce_roulette[idx_parent1]->clone()); //複製一份一模一樣的進入交配池
        this->crossover_pool.push_back(reproduce_roulette[idx_parent2]->clone()); //複製一份一模一樣的進入交配池
    }

    reproduce_roulette.clear(); //清空暫存輪盤
}

void Population::mutation()
{
    vector<std::shared_ptr<Organism>> mutation_pool;
    for (auto const &org : this->crossover_pool)
    {
        double val = NEAT::randfloat();
        if (val < NEAT::mutation_link)
        {
            auto mutation_org = org->clone();
            mutation_org->mutationLink();
            mutation_pool.push_back(mutation_org);
        }

        val = NEAT::randfloat();
        if (val < NEAT::mutation_node)
        {
            auto mutation_org = org->clone();
            mutation_org->mutationNode();
            mutation_pool.push_back(mutation_org);
        }
    }

    for (auto const &org : mutation_pool)
    {
        this->organisms.push_back(org);
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
    for (auto const &org : this->organisms)
    {
        cout << "[INFO]\torganism [" << org->getOrganismId() << "]-> fitness(loss) = " << org->getFitness() << endl;
    }
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

void Population::save_best_organism(char *filename)
{
    // fstream fp;
    // fp.open(filename, ios::out); //開啟檔案
    // if (!fp)
    // { //如果開啟檔案失敗，fp為0；成功，fp為非0
    //     cout << "Fail to open file: " << filename << endl;
    // }
    // cout << "File Descriptor: " << fp << endl;
    // fp << "Hello HappyMan!!" << endl; //寫入字串

    // fp.close(); //關閉檔案
}

void Population::showInfo()
{
    //    cout << "****************Population****************" << endl;
    //    cout << "* organisms size " << this->organisms.size() << endl;
    //    cout << "* genome id " << this->genome_id << endl;
    //    cout << "******************************************" << endl;
    // cout << "[INFO] best organism ["<< this->best_organism->getOrganismId() <<"]-> fitness(accuracy) = " << this->best_organism->getFitness() << endl;
    auto org = this->organisms[0];

    cout << "[INFO] organism [" << org->getOrganismId() << "]-> fitness(loss) = " << org->getFitness() << "\taccuracy = " << org->getAccuracy() << "\t";
    //    for (auto const &org : this->organisms)
    //    {
    //    cout << "* evoluation ... ";
    //    cout << "[INFO] organism ["<< org->getOrganismId() <<"]-> fitness(accuracy) = " << org->getFitness() << endl;
    //    }

    // for (auto const &org : this->organisms)
    // {
    //     cout << "* evoluation ... ";
    //     cout << "org , race = " << org->species_id << "\tfitness = " << org->getFitness()
    //          << "\taccuracy = " << this->best_organism->calculate_accuracy() << endl;
    // }

    cout << "* organisms size " << this->organisms.size() << endl;
}

bool organisms_order_by_fitness_and_race(std::shared_ptr<Organism> i, std::shared_ptr<Organism> j)
{
    if (i->species_id == j->species_id)
    {
        //return (i->getFitness() > j->getFitness());   //max -> min
        return (i->getFitness() < j->getFitness()); //min -> max
        // return i->calculate_accuracy() < j->calculate_accuracy();
    }
    else
    {
        return (i->species_id < j->species_id);
    }
}
