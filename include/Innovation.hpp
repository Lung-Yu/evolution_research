#ifndef TYGR_INNVATION_HPP
#define TYGR_INNVATION_HPP

#include <iostream>
#include <memory>

class Innovation
{
  private:
    Innovation();
    static std::shared_ptr<Innovation> _instance;
  protected:
    int node_innovtion;
    int link_innovtion;

  public:
    static std::shared_ptr<Innovation> getInstance();
    // Innovation(& innov);
    ~Innovation();

    int applyNodeInnovation();
    int applyLinkInnovation();

    void showInfo();
};
#endif
