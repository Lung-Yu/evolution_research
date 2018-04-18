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

    //Size of larger Genome
    int max_genome_size = (self_gene_size > comp_gene_size) ? self_gene_size : comp_gene_size;

    for (int i = 0, j = 0; i < max_genome_size || j < max_genome_size; i++, j++)
    {
        if (i >= self_gene_size)
        {
            count_excess++;
        }
        else if (j >= comp_gene_size)
        {
            count_excess++;
        }
        else
        {
            int innov_1 = this->links[i]->InnovationId();
            int innov_2 = g->links[j]->InnovationId();

            if (innov_1 == innov_2)
            {
                count_mutation_diff_total += abs(this->links[i]->getMutationCount() - g->links[j]->getMutationCount());
                count_match++;
            }
            else
                count_disjoint++;
        }
    }

    return 0;
}

std::shared_ptr<GeneLink> Genome::pick_rand_link()
{
    int max_range = this->links.size() - 1;
    int min_range = 0;
    int idx = NEAT::randint(min_range, max_range);
    auto link = this->links[idx];

    return link;
}

void Genome::mutationNode()
{
    // cout << "genome mutation node." << endl;

    auto geneInfo = GeneInfoController::getInstance();

    auto link = this->pick_rand_link();
    int in_node = link->getInNodeId();
    int out_node = link->getOutNodeId();

    link->setDisable();
    auto new_node = geneInfo->applyMutationNode(this->nodes); //申請一個突變用的節點
    // cout << "apply mutation node id " << new_node->getNodeId() << " address = " << new_node << endl;

    auto new_link_1 = geneInfo->applyNewGeneLink(in_node, new_node->getNodeId());
    auto new_link_2 = geneInfo->applyNewGeneLink(new_node->getNodeId(), out_node);

    this->nodes.push_back(new_node);
    this->links.push_back(new_link_1);
    this->links.push_back(new_link_2);
}

void Genome::mutationLink()
{
    vector<int> consider_pair; //兩個數字一組（in,out）（in,out）（in,out） ...

    for (auto const &start_node : this->nodes)
    {
        for (auto const &end_node : this->nodes)
        {
            //如果為同一個神經元則略過
            if (start_node->getNodeId() == end_node->getNodeId())
                continue;

            //輸入節點不能當其他人的輸出
            if (end_node->getNodeType() == NODE_TYPE::Sensor)
                continue;

            //輸出節點不能當其他人的輸入 <-- 存疑！！？
            if (start_node->getNodeType() == NODE_TYPE::Output)
                continue;

            //通過前述條件則代表此連結為候選連結,可以考慮新建
            consider_pair.push_back(start_node->getNodeId());
            consider_pair.push_back(end_node->getNodeId());
        }
    }

    //如果沒有候選連結,代表目前已經是全連接狀態,沒有新連結可以添加.
    if (consider_pair.size() == 0)
        return;

    int idx = NEAT::randint(0, consider_pair.size() / 2);
    int start_node_id = consider_pair[idx * 2];
    int end_node_id = consider_pair[idx * 2 + 1];
    //cout << "mutation new link [" << start_node_id << "-->" << end_node_id << "]." << endl;
    GeneInfoController::getInstance()->applyNewGeneLink(start_node_id, end_node_id);
}

int Genome::getGenommeId()
{
    return this->genomme_id;
}