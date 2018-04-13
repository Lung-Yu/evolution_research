#ifndef TYGR_GENE_INFO_CONTROLLER_HPP
#define TYGR_GENE_INFO_CONTROLLER_HPP

#include <vector>
#include <memory>

#include "GeneNode.hpp"
#include "GeneLink.hpp"
#include "Innovation.hpp"

class GeneNode;
class GeneLink;

class GeneInfoController
{

  private:
    static std::shared_ptr<GeneInfoController> _instance;
    GeneInfoController();
    
    bool existLink(int src,int dst);    //該連結是否存在,若已經存在則回傳其innovation number,否則回傳 -1
    std::shared_ptr<GeneLink> findLink(int src,int dst);
  protected:
    std::vector<std::shared_ptr<GeneNode>> nodes;
    std::vector<std::shared_ptr<GeneLink>> links;

  public:
    std::shared_ptr<GeneInfoController> getInstance();
    ~GeneInfoController();

    void putNode(std::shared_ptr<GeneNode> node);
    void putLink(std::shared_ptr<GeneLink> link);
    std::shared_ptr<GeneNode> applyNewOutputGeneNode();
    std::shared_ptr<GeneNode> applyNewInputGeneNode();
    std::shared_ptr<GeneNode> applyNewHiddenGeneNode();
    std::shared_ptr<GeneLink> applyNewGeneLink(int src_id,int dst_id);
};

#endif