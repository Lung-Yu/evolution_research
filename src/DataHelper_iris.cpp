#include "DataHelper.hpp"

using namespace std;

//字符串分割函数
vector<double> str_split(string str, string pattern)
{
    string::size_type pos;
    vector<double> result;
    str += pattern; //扩展字符串以方便操作
    int size = str.size();

    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            string s = str.substr(i, pos - i);
            result.push_back(stof(s));
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

inline vector<vector<double>> read_file(string full_file_path)
{
    fstream fin;
    fin.open(full_file_path, ios::in);
    vector<vector<double>> datas;
    char buffer_line[25];
    while (fin.getline(buffer_line, sizeof(buffer_line), '\n'))
    {
        //double val = atof(buffer_line);
        string item_str(buffer_line);
        string item_pattern(" ");
        vector<double> item = str_split(item_str, item_pattern);
        datas.push_back(item);
    }

    return datas;
}

DataHelper::DataHelper()
{
    this->isTrain = true;
    this->current_idx = 0;
    this->TrainingMode();
}

DataHelper::~DataHelper()
{
}

void DataHelper::TrainingMode()
{
    this->input_datas.clear();
    this->desire_datas.clear();

    this->isTrain = true;
    string train_file("train_iris.txt");
    vector<vector<double>> datas = read_file(train_file);

    for (auto const &item : datas)
    {
        int size = item.size();
        vector<double> input;
        vector<double> label;
        for (int i = 0; i < size; i++)
        {
            //data
            if (i != (size - 1))
            {
                input.push_back(item[i]);
            }
            else //label
            {
                for (int i = 0; i < 3; i++)
                {
                    if ((item[i] + 0.5) > i && (item[i] - 0.5) < i)
                    {
                        label.push_back(1);
                    }
                    else
                    {
                        label.push_back(0);
                    }
                }
            }
        }

        this->input_datas.push_back(input);
        this->desire_datas.push_back(label);
    }
}

void DataHelper::InferenceMode()
{
    this->input_datas.clear();
    this->desire_datas.clear();

    this->isTrain = false;
    string test_file("test_iris.txt");
    vector<vector<double>> datas = read_file(test_file);

    for (auto const &item : datas)
    {
        int size = item.size();
        vector<double> input;
        vector<double> label;
        for (int i = 0; i < size; i++)
        {
            //data
            if (i != (size - 1))
            {
                input.push_back(item[i]);
            }
            else //label
            {
                for (int i = 0; i < 3; i++)
                {
                    if ((item[i] + 0.5) > i && (item[i] - 0.5) < i)
                    {
                        label.push_back(1);
                    }
                    else
                    {
                        label.push_back(0);
                    }
                }
            }
        }

        this->input_datas.push_back(input);
        this->desire_datas.push_back(label);
    }
}

int DataHelper::batch_size()
{
    int size = min((int)this->input_datas.size(), (int)this->desire_datas.size());
    return size;
}

void DataHelper::move_next()
{
    this->current_idx++;
    if (current_idx >= this->batch_size())
        this->current_idx = 0;
}

int DataHelper::getInputSize()
{
    return 4;
}

int DataHelper::getOutputSize()
{
    return 3;
}

std::vector<double> DataHelper::getInputs()
{
    return this->input_datas[this->current_idx];
}

std::vector<double> DataHelper::getOutputs()
{
    return this->desire_datas[this->current_idx];
}
