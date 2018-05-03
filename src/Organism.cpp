#include "Organism.hpp"

using namespace std;

Organism::Organism(std::shared_ptr<Genome> g)
{
    this->evolution_time = NEAT::growth_time;
    this->species_id = -1;

    this->gemone = g;

    this->fitness = 0;
    this->accuracy = 0;
    this->accuracy_train = 0;
    this->loss = 0;
}

Organism::~Organism()
{
}

void Organism::evolution()
{
    evolution_fitness();
}

void Organism::evolution_fitness()
{
    this->accuracy = calculate_accuracy();
    this->accuracy_train = calculate_accuracy_train();

    auto net = make_shared<NerveNetwork>(this->gemone);
    this->loss = net->inference(true);
    this->fitness = this->loss;

    // cout << "evolution_fitness = " << this->fitness << "\tloss = " << this->loss << "\taccuracy = "
    //      << this->accuracy << "\taccuracy_train = " << this->accuracy_train << endl;
}

void Organism::growthUp()
{
    auto net = make_shared<NerveNetwork>(this->gemone);
    // net->train(this->evolution_time);
    net->train_SGD(this->evolution_time);
    // cout << "[" << this->gemone->genomme_id << "] growthUp\ttrain loss before= " << old_loss << ",\ttrain loss after= " << new_loss << endl;
    auto new_gemone = net->toGenome();
    this->gemone = new_gemone;
    // cout << this->gemone->genomme_id << "\t growthup old_loss = " << old_loss << "\t new_loss" << new_loss << endl;

    this->evolution_fitness(); //計算相關參數
}

double Organism::getAccuracy()
{
    return this->accuracy;
}

double Organism::getTrainAccuracy()
{
    return this->accuracy_train;
}

double Organism::calculate_accuracy_train()
{
    auto net = make_shared<NerveNetwork>(this->gemone);
    double accuracy = net->get_train_accuracy();
    return accuracy;
}

double Organism::calculate_accuracy()
{
    auto net = make_shared<NerveNetwork>(this->gemone);
    double accuracy = net->get_inference_accuracy();
    return accuracy;
}

double Organism::calculate_loss()
{
    auto net = make_shared<NerveNetwork>(this->gemone);
    this->loss = net->inference(true);

    return this->loss;
}

double Organism::compatibility(std::shared_ptr<Organism> org)
{
    return this->gemone->compatibility(org->gemone);
}

double Organism::getLoss()
{
    return this->loss;
}

std::shared_ptr<Organism> Organism::crossover(int new_org_id, std::shared_ptr<Organism> org)
{
    vector<int> gene_innovs_p1;
    vector<int> gene_innovs_p2;

    vector<shared_ptr<GeneNode>> offspring_nodes;
    vector<shared_ptr<GeneLink>> offspring_links;

    //put all innovation in gene innovs arrays
    for (auto const &val : this->gemone->links)
        gene_innovs_p1.push_back(val->InnovationId());
    for (auto const &val : org->gemone->links)
        gene_innovs_p2.push_back(val->InnovationId());
    // cout << "pull all innovation in gene innovs arrays done." << endl;
    //put all nodes
    int idx_p1 = 0, idx_p2 = 0;
    int gene1_node_size = this->gemone->nodes.size();
    int gene2_node_size = org->gemone->nodes.size();
    // cout << "node\tg1 size = " << gene1_node_size << " g2 size = " << gene2_node_size << endl;
    while (idx_p1 < gene1_node_size || idx_p2 < gene2_node_size)
    {
        if (idx_p1 == gene1_node_size)
        {
            auto node2 = org->gemone->nodes[idx_p2];
            offspring_nodes.push_back(node2);
            idx_p2++;
        }
        else if (idx_p2 == gene2_node_size)
        {
            auto node1 = this->gemone->nodes[idx_p1];
            offspring_nodes.push_back(node1);
            idx_p1++;
        }
        else
        {
            auto node1 = this->gemone->nodes[idx_p1];
            auto node2 = org->gemone->nodes[idx_p2];
            if (node1->getNodeId() == node2->getNodeId())
            {
                auto node_temp = (NEAT::randfloat() > 0.5) ? node1 : node2;
                offspring_nodes.push_back(node_temp);
                idx_p1++, idx_p2++;
            }
            else if (node1->getNodeId() < node2->getNodeId())
            {
                offspring_nodes.push_back(node1);
                idx_p1++;
            }
            else
            {
                offspring_nodes.push_back(node2);
                idx_p2++;
            }
        }
    }

    idx_p1 = 0, idx_p2 = 0;
    int gene1_innov_size = (int)gene_innovs_p1.size();
    int gene2_innov_size = (int)gene_innovs_p2.size();
    // cout << "link\tg1 size = " << gene1_innov_size << " g2 size = " << gene2_innov_size << endl;
    while ((idx_p1 < gene1_innov_size) || (idx_p2 < gene2_innov_size))
    {
        if (idx_p1 == gene1_innov_size)
        {
            //基因p1已經完全排列完畢,而p2還未排完
            //所以p2剩餘的所有基因直接排入
            auto gene = org->gemone->links[idx_p2];
            offspring_links.push_back(gene);
            idx_p2++;
        }
        else if (idx_p2 == gene2_innov_size)
        {
            //基因p2已經完全排列完畢,而p1還未排完
            //所以p1所有剩餘的基因直接排入
            auto gene = this->gemone->links[idx_p1];
            offspring_links.push_back(gene);
            idx_p1++;
        }
        else
        {
            int p1_innov = gene_innovs_p1[idx_p1];
            int p2_innov = gene_innovs_p2[idx_p2];

            //如果兩個基因的位置相同則隨意取一個
            if (p1_innov == p2_innov)
            {
                if (NEAT::randfloat() > 0.5)
                {
                    //取得p1的基因
                    auto gene = this->gemone->links[idx_p1];
                    offspring_links.push_back(gene);
                }
                else
                {
                    //取得p2的基因
                    auto gene = org->gemone->links[idx_p2];
                    offspring_links.push_back(gene);
                }

                idx_p1++;
                idx_p2++;
            }
            else if (p1_innov < p2_innov)
            {
                //如果第一個基因的數值比較小代表中間有空間
                //直接取得p1的基因
                auto gene = this->gemone->links[idx_p1];
                offspring_links.push_back(gene);
                idx_p1++;
            }
            else
            {
                //如果第二個基因的數值比較小代表中間有空間
                //直接取得p2的基因
                auto gene = org->gemone->links[idx_p2];
                offspring_links.push_back(gene);
                idx_p2++;
            }
        }
        // cout << "link\tg1 size = " << gene1_innov_size << " g2 size = " << gene2_innov_size << "\tidx p1 =" << idx_p1 << "\tidx p2 = " << idx_p2 << endl;
    }
    // cout << "get offspring gemone " << endl;

    auto offspring_gemone = make_shared<Genome>(new_org_id, offspring_nodes, offspring_links);
    auto offspring_org = make_shared<Organism>(offspring_gemone);
    return offspring_org;
}

void Organism::harass()
{
    for (auto const &link : this->gemone->links)
    {
        link->harassWeight();
    }
}

double Organism::getFitness()
{
    return this->loss;
}

int Organism::getOrganismId()
{
    return this->gemone->getGenommeId();
}

void Organism::mutationNode()
{
    this->gemone->mutationNode();
}

void Organism::mutationLink()
{
    this->gemone->mutationLink();
}

std::shared_ptr<Organism> Organism::clone()
{
    auto new_org = make_shared<Organism>(this->gemone);
    return new_org;
}

std::shared_ptr<Genome> Organism::getGenome()
{
    return this->gemone->clone();
}