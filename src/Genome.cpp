#include "Genome.hpp"

using namespace std;
Genome::Genome()
{
}

Genome::Genome(int g_id, vector<shared_ptr<GeneNode>> nodes, vector<shared_ptr<GeneLink>> src_links)
{
    this->genomme_id = g_id;
    this->nodes = nodes;
    // cout << "constructor gid = " << genomme_id << "\tlink size = " << src_links.size()  << endl;
    for (auto const &link : src_links)
        addLink(link);
    // cout <<"constructor add link result = " << addLink(link) << endl;

    this->restructuring();
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
    return new_g;
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
bool Genome::hasLink(int id)
{
    for (auto const &link : this->links)
    {
        if (link->InnovationId() == id)
            return true;
    }
    return false;
}
bool Genome::addLink(shared_ptr<GeneLink> new_link)
{
    bool HasLink = this->hasLink(new_link->InnovationId());

    if (!GeneInfoController::getInstance()->HasNodeId(new_link->getInNodeId()))
    {
        // cout << "[reject info] \t in_node_id = " << new_link->getInNodeId() << endl;
        return false;
    }

    if (!GeneInfoController::getInstance()->HasNodeId(new_link->getOutNodeId()))
    {
        // cout << "[reject info] \t out_node_id = " << new_link->getOutNodeId() << endl;
        return false;
    }

    if (new_link->getInNodeId() == new_link->getOutNodeId())
    {
        // cout << "[reject info] \t in_node_id = " << new_link->getInNodeId() << "   "
        //  << "out_node_id = " << new_link->getOutNodeId() << endl;
        return false;
    }

    if (!HasLink)
    {
        // cout << "is new link" << endl;
        //沒有的連結
        //進行循環檢查
        bool is_loop = newlink_loop_check(new_link);

        if (!is_loop)
        {
            // cout << "new link can't make loop" << endl;
            this->links.push_back(new_link);
            // cout << "add link info " << new_link->InnovationId() << "-" << new_link->getInNodeId() << "-->" << new_link->getOutNodeId() << endl;
            // this->show();
            return true;
        }
    }

    return false;
}

void Genome::mutationNode()
{
    // this->show();
    // cout << "genome mutation node." << endl;
    auto geneInfo = GeneInfoController::getInstance();

    auto link = this->pick_rand_link();

    // cout << "pick rand link " << endl;

    int in_node = link->getInNodeId();
    int out_node = link->getOutNodeId();

    link->setDisable();
    auto new_node = geneInfo->applyMutationNode(this->nodes); //申請一個突變用的節點
    // cout << "apply mutation node id " << new_node->getNodeId() << " address = " << new_node << endl;

    auto new_link_1 = geneInfo->applyNewGeneLink(in_node, new_node->getNodeId());
    auto new_link_2 = geneInfo->applyNewGeneLink(new_node->getNodeId(), out_node);

    this->nodes.push_back(new_node);
    // cout << "mutation node new node = " << new_node << endl;
    // cout << "mutation node new link 1 = " << new_link_1 << " " << new_link_1->getInNodeId() << "-->" << new_link_1->getOutNodeId() << endl;
    // cout << "mutation node new link 2 = " << new_link_2 << " " << new_link_2->getInNodeId() << "-->" << new_link_2->getOutNodeId() << endl;
    addLink(new_link_1);
    addLink(new_link_2);
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
            {
                continue;
                //輸入節點不能當其他人的輸出
            }
            else if (end_node->getNodeType() == NODE_TYPE::Sensor)
            {
                continue;
                //輸出節點不能當其他人的輸入 <-- 存疑！！？
            }
            else if (start_node->getNodeType() == NODE_TYPE::Output)
            {
                continue;
            }
            else
            {
                //通過前述條件則代表此連結為候選連結,可以考慮新建
                consider_pair.push_back(start_node->getNodeId());
                consider_pair.push_back(end_node->getNodeId());
            }
        }
    }

    //如果沒有候選連結,代表目前已經是全連接狀態,沒有新連結可以添加.
    if (consider_pair.size() == 0)
        return;
    else
    {
        int range_max = (int)consider_pair.size() / 2;
        int idx = NEAT::randint(0, range_max);
        int start_node_id = consider_pair[idx * 2];
        int end_node_id = consider_pair[idx * 2 + 1];
        //cout << "mutation new link [" << start_node_id << "-->" << end_node_id << "]." << endl;
        auto new_link = GeneInfoController::getInstance()->applyNewGeneLink(start_node_id, end_node_id);
        this->addLink(new_link);
    }
}

void Genome::mutationReduceNode()
{
    int node_size = this->nodes.size();
    int pick_idx = NEAT::randint(0, node_size - 1);

    if (node_size < 0)
        return;

    // cout << "pick idx = " << pick_idx << "/" << node_size << endl;
    int picked_node_id = this->nodes[pick_idx]->getNodeId();
    if (this->nodes[pick_idx]->getNodeType() == NODE_TYPE::Output)
        return;
    // cout << "picked_node_id = " << picked_node_id << endl;

    for (auto const &link : this->links)
    {
        // cout << " link_id = " << link->InnovationId() << endl;
        if ((link->getInNodeId() == picked_node_id) || (link->getOutNodeId() == picked_node_id))
        {
            // cout << "disable link " << endl;
            link->setDisable();
            // cout << "disable link done." << endl;
        }
    }

    // cout << "picked_node_id disable done." << endl;
}

void Genome::mutationReduceLink()
{
    int link_size = this->links.size();
    int pick_idx = NEAT::randint(0, link_size - 1);

    if (link_size <= 0)
        return;

    // auto link = links[pick_idx];
    // links[pick_idx]->
    // for (auto const &node : this->nodes)
    // {
    //     if (node->getNodeId() == link->getOutNodeId())
    //     {
    //         if (node->getNodeType() == NODE_TYPE::Output)
    //         {
    //             if(link_count()==)
    //         }
    //         break;
    //     }
    // }
    links[pick_idx]->setDisable();
}

// int Genome::link_enable_count(int node_id){
//     int count = 0 ;
//     for(auto const &link : this->links){
//         if(link->getInNodeId() == node_id || link->getOutNodeId() == node_id && link->IsEnable())
//             count++;

//     }
//     return count;
// }

void Genome::restructuring()
{
    sort(this->links.begin(), this->links.end(), genome_link_oder_by_innov);
}

bool Genome::newlink_loop_check(std::shared_ptr<GeneLink> link)
{
    stack<int> track_number;
    vector<int> track_path;

    track_number.push(link->getInNodeId());

    track_path.push_back(link->getInNodeId());
    track_path.push_back(link->getOutNodeId());

    // for (auto const &_link : this->links)
    // {
    //     if (_link->getInNodeId() == link->getOutNodeId() && _link->getOutNodeId() == link->getInNodeId())
    //     {
    //         return true;
    //     }
    // }

    while (track_number.size() > 0)
    {
        //取得一筆資料出來檢查
        int temp = track_number.top();
        track_number.pop();

        for (auto const &link : this->links)
        {
            //如果找到往前的路線則進行處理
            if (link->getOutNodeId() == temp)
            {
                int in_node = link->getInNodeId();
                track_number.push(in_node);

                //檢查是否存在已經走過得路線
                for (auto const &id : track_path)
                    if (id == in_node)
                        return true;

                track_path.push_back(in_node);
            }
        }
    }

    return false;
}

void Genome::show()
{
    cout << "gid = [" << this->genomme_id << "]" << endl;
    cout << "link size =" << this->links.size() << endl;
    cout << "node size = " << this->nodes.size() << endl;

    for (auto const &link : this->links)
    {
        cout << link->InnovationId() << " " << link->getInNodeId() << "->" << link->getOutNodeId() << endl;
    }
}

int Genome::getGenommeId()
{
    return this->genomme_id;
}

bool genome_link_oder_by_innov(std::shared_ptr<GeneLink> i, std::shared_ptr<GeneLink> j)
{
    return i->InnovationId() < j->InnovationId();
}
