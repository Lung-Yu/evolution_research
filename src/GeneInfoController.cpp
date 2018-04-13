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
    auto node = make_shared<GeneNode>(Innovation::getInstance()->applyNodeInnovation(), NODE_TYPE::Output, NODE_FUNC_TYPE::Identity);
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
    if (this->existLink(src, dst))
    {
        auto link = make_shared<GeneLink>(Innovation::getInstance()->applyLinkInnovation(), src, dst, NEAT::randfloat());
        this->putLink(link);

        return link->clone();
    }

    return findLink(src, dst)->clone();
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