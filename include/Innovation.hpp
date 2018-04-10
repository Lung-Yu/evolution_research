#ifndef TYGR_INNVATION_HPP
#define TYGR_INNVATION_HPP

#include <iostream>

class Innovation
{
  protected:
    int node_innovtion;
    int link_innovtion;

  public:
    Innovation();
    ~Innovation();

    int applyNodeInnovation();
    int applyLinkInnovation();

    void showInfo();
};
#endif
