#ifndef TYGR_GENE_NODE_HPP
#define TYGR_GENE_NODE_HPP

#include <memory>
#include "NerveNeuron.hpp"
#include "NerveNetwork.hpp"
//activity 激勵函數的方法
typedef enum {
    Identity,
    Sigmoid
} NODE_FUNC_TYPE;

typedef enum {
    Sensor,
    Hidden,
    Output
} NODE_TYPE;

// typedef struct
// {
//     int node_id;
//     NODE_TYPE node_type;
//     NODE_FUNC_TYPE func_type;
// } GENE_NODE;

class GeneNode
{
    friend class NerveNeuron; //締結友好關係,可以直接讀取資料
    friend class NerveNetwork;

  protected:
    int node_id;
    NODE_TYPE node_type;
    NODE_FUNC_TYPE func_type;

  public:
    GeneNode(int id, NODE_TYPE type, NODE_FUNC_TYPE func);
    ~GeneNode();

    int getNodeId();

    std::shared_ptr<GeneNode> clone();
};

#endif