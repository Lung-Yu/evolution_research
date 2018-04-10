#ifndef TYGR_DATA_HELPER_HPP
#define TYGR_DATA_HELPER_HPP

#include <vector>

class DataHelper
{
  protected:
    int current_idx = 0;
    std::vector<std::vector<double>> input_datas;
    std::vector<std::vector<double>> desire_datas;

  public:
    DataHelper();
    ~DataHelper();

    int batch_size();
    void move_next();

    int getInputSize();
    int getOutputSize();

    std::vector<double> getInputs();
    std::vector<double> getOutputs();
};

#endif