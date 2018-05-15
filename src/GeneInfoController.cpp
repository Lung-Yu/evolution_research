#include "GeneInfoController.hpp"

using namespace std;

shared_ptr<GeneInfoController> GeneInfoController::_instance = nullptr;

GeneInfoController::GeneInfoController()
{
}

GeneInfoController::~GeneInfoController()
{
}

std::shared_ptr<GeneInfoController> GeneInfoController::getInstance()
{
    if (GeneInfoController::_instance == nullptr)
    {
        auto ptr = new GeneInfoController();
        GeneInfoController::_instance.reset(ptr);
    }

    return _instance;
}

void GeneInfoController::putNode(std::shared_ptr<GeneNode> node)
{
    this->nodes.push_back(node);
}

void GeneInfoController::putLink(std::shared_ptr<GeneLink> link)
{
    if (this->existLink(link->inNodeId, link->outNodeId))
        return;

    this->links.push_back(link);
}

std::shared_ptr<GeneNode> GeneInfoController::applyNewInputGeneNode()
{
    auto node = make_shared<GeneNode>(Innovation::getInstance()->applyNodeInnovation(), NODE_TYPE::Sensor, NODE_FUNC_TYPE::Identity);
    this->putNode(node);

    return node->clone();
}

std::shared_ptr<GeneNode> GeneInfoController::applyNewOutputGeneNode()
{
    auto node = make_shared<GeneNode>(Innovation::getInstance()->applyNodeInnovation(), NODE_TYPE::Output, NODE_FUNC_TYPE::Sigmoid);
    this->putNode(node);

    return node->clone();
}

std::shared_ptr<GeneNode> GeneInfoController::applyNewHiddenGeneNode()
{
    auto node = make_shared<GeneNode>(Innovation::getInstance()->applyNodeInnovation(), NODE_TYPE::Hidden, NODE_FUNC_TYPE::Sigmoid);
    this->putNode(node);

    return node->clone();
}

std::shared_ptr<GeneLink> GeneInfoController::applyNewGeneLink(int src, int dst)
{
    if (!this->existLink(src, dst))
    {
        auto link = make_shared<GeneLink>(Innovation::getInstance()->applyLinkInnovation(), src, dst, NEAT::randfloat() - 0.5);
        this->putLink(link);

        return link->clone();
    }

    return findLink(src, dst)->clone();
}

vector<std::shared_ptr<GeneNode>> GeneInfoController::getInputNodes()
{
    return this->getNodesByType(NODE_TYPE::Sensor);
}

vector<std::shared_ptr<GeneNode>> GeneInfoController::getOutputNodes()
{
    return this->getNodesByType(NODE_TYPE::Output);
}

vector<std::shared_ptr<GeneNode>> GeneInfoController::getHiddenNodes()
{
    return this->getNodesByType(NODE_TYPE::Hidden);
}

std::vector<std::shared_ptr<GeneNode>> GeneInfoController::getNodesByType(NODE_TYPE type)
{
    vector<std::shared_ptr<GeneNode>> pool;

    for (auto const &node : this->nodes)
    {
        if (node->node_type == type)
            pool.push_back(node->clone());
    }

    return pool;
}

std::vector<std::shared_ptr<GeneNode>> GeneInfoController::getAllNodes()
{
    return this->nodes;
}

std::vector<std::shared_ptr<GeneLink>> GeneInfoController::getAllLinks()
{
    return this->links;
}

bool GeneInfoController::existLink(int src, int dst)
{
    for (auto const &link : this->links)
        if (link->inNodeId == src && link->outNodeId == dst)
            return true;

    return false; //不存在的連結
}

std::shared_ptr<GeneLink> GeneInfoController::findLink(int src, int dst)
{
    for (auto const &link : this->links)
        if (link->inNodeId == src && link->outNodeId == dst)
            return link;

    return nullptr;
}

std::shared_ptr<GeneNode> GeneInfoController::applyMutationNode(std::vector<std::shared_ptr<GeneNode>> exist_nodes)
{
    vector<shared_ptr<GeneNode>> consider_nodes;

    //檢查是否存在,並建立所有尚未有的候選節點
    for (auto const &_node : this->nodes)
    {
        bool isExist = false;
        for (auto const &exist_node : exist_nodes)
        {
            if (_node->getNodeId() == exist_node->getNodeId())
            {
                isExist = true;
                break;
            }
        }

        if (!isExist)
        {
            consider_nodes.push_back(_node);
        }
    }

    //如果所有節點都存在,則須產生新的節點
    if (consider_nodes.size() == 0)
    {
        return this->applyNewHiddenGeneNode();
    }
    else
    {
        int idx = NEAT::randint(0, consider_nodes.size() - 1);
        return consider_nodes[idx]->clone();
    }
}

bool GeneInfoController::HasNodeId(int id)
{
    int size  = this->nodes.size();
    for(int i=0;i<size;i++){
        if(this->nodes[i]->getNodeId() == id)
            return true;
    }

    return false;
}
bool GeneInfoController::HasLinkId(int id) 
{
    int size  = this->links.size();
    for(int i=0;i<size;i++){
        if(this->links[i]->InnovationId() == id)
            return true;
    }

    return false;
}

void GeneInfoController::showInfo()
{
    cout << "********* GeneInfoController *********" << endl;
    cout << "* node size : " << this->nodes.size() << endl;
    cout << "* link size : " << this->links.size() << endl;
    cout << "**************************************" << endl;
}