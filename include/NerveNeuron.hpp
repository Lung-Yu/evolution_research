#ifndef TYGR_NERVE_NEURON_HPP
#define TYGR_NERVE_NEURON_HPP

#include <vector>
#include <memory>
#include <cmath>
#include "GeneNode.hpp"
#include "NerveSynapse.hpp"
class GeneNode;
class NerveSynapse;

class NerveNeuron
{
  // friend class GeneNode;
  friend NerveSynapse;

protected:
  int node_id;                        //神經元的編號
  std::shared_ptr<GeneNode> nodeInfo; //神經元的資訊

  double sum_z;      //神經元的總和 z
  double output_val; //神經元的實際輸出
  // double loss;    //神經元累積之損失

  bool isActivity; // 該神經元是否以啟動

  std::vector<std::shared_ptr<NerveSynapse>> synapses;
  void execute_activity();
public:
  NerveNeuron(std::shared_ptr<GeneNode> node_info);
  ~NerveNeuron();

  //神經溝通
  void attach(std::shared_ptr<NerveSynapse> link); //將神經元與突觸進行連接
  void notify();
  void notifyError(double error);
  void adjust_all();
  double get_differential();

  //方法
  void feed(double val); //若狀態為sensor時使用此方法放置資料,會直接覆蓋到output_val
  double getAxon();      //取得神經輸出之資料
  void recover(); //恢復未啟動狀態（未啟動時isActivity = false)
  //getter setter
  int getNodeId();
  bool IsOutputNode();
};


//activity
double sigmoid_func(double x);

double identity_func(double x);

//adjust activity
double differential_sigmoid_func(double output);
double differential_sigmoid_func(double input, double output, double error);

double differential_identity_func(double output, double error);

#endif