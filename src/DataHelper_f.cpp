#include "DataHelper.hpp"

using namespace std;

DataHelper::DataHelper()
{
    this->current_idx = 0;
    this->TrainingMode();

    for (int i = 0; i < 10; i++)
    {
        std::vector<double> input;
        input.push_back(i);

        std::vector<double> output;
        output.push_back(cos(3 * i));  // 0.2x

        this->input_datas.push_back(input);
        this->desire_datas.push_back(output);
    }
}

DataHelper::~DataHelper()
{
}

void DataHelper::TrainingMode()
{
}

void DataHelper::InferenceMode()
{
}

int DataHelper::batch_size()
{
    return 10;
}

void DataHelper::move_next()
{
    this->current_idx++;
    if (current_idx >= this->batch_size())
        this->current_idx = 0;
}

int DataHelper::getInputSize()
{
    return 1;
}

int DataHelper::getOutputSize()
{
    return 1;
}

std::vector<double> DataHelper::getInputs()
{

    return this->input_datas[current_idx];
}

std::vector<double> DataHelper::getOutputs()
{
    return this->desire_datas[current_idx];
}
