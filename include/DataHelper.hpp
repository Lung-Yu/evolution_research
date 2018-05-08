#ifndef TYGR_DATA_HELPER_HPP
#define TYGR_DATA_HELPER_HPP

#include <vector>

//讀取檔案
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
//讀取目錄夾
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

class DataHelper
{
protected:
  bool isTrain;
  int current_idx = 0;
  std::vector<std::vector<double>> input_datas;
  std::vector<std::vector<double>> desire_datas;

  std::vector<std::string> file_names;

public:
  DataHelper();
  ~DataHelper();

  int batch_size();
  void move_next();

  int getInputSize();
  int getOutputSize();

  void TrainingMode();
  void InferenceMode();

  std::vector<double> getInputs();
  std::vector<double> getOutputs();
};

#endif