#include "DataHelper.hpp"

using namespace std;

inline vector<string> getdir(string dir)
{
    DIR *dp; //創立資料夾指標
    struct dirent *dirp;

    vector<string> files;

    if ((dp = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return files;
    }
    while ((dirp = readdir(dp)) != NULL)
    { //如果dirent指標非空
        // if (dirp->d_reclen != 32)    //32 = folder
        if (dirp->d_reclen == 40)
        {
            files.push_back(string(dirp->d_name)); //將資料夾和檔案名放入vector
            // cout << "[INFO]\t" << dirp->d_name << "\t" << dirp->d_reclen << "." << endl;
        }
    }
    closedir(dp); //關閉資料夾指標
    return files;
}

inline vector<double> get_real_data(string full_file_path)
{
    fstream fin;
    // fin.open("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train/train_2_31281.txt", ios::in);
    fin.open(full_file_path, ios::in);

    vector<double> datas;
    char buffer_line[5];
    while (fin.getline(buffer_line, sizeof(buffer_line), ','))
    {
        datas.push_back(atof(buffer_line));
    }

    return datas;
}

inline string get_current_folder_name(bool is_train_mode)
{
    if(is_train_mode)
        return string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train/");
    else
        return string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/test/");
}

DataHelper::DataHelper()
{
    this->current_idx = 0;

    //預設模式
    this->TrainingMode();
    //資料夾路徑(絕對位址or相對位址)
    // string dir_train = string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train");
    // string dir_test = string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/test");
}

DataHelper::~DataHelper()
{
}

void DataHelper::TrainingMode()
{
    this->file_names.clear();
    this->isTrain = true;

    string dir = string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train");
    this->file_names = getdir(dir);
}
void DataHelper::InferenceMode()
{
    this->file_names.clear();
    this->isTrain = false;

    string dir = string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/test");
    this->file_names = getdir(dir);
    // getdir(dir, this->file_names);
}

int DataHelper::batch_size()
{
    return (int)(this->file_names.size());
}

void DataHelper::move_next()
{
    this->current_idx++;
    if (current_idx >= this->batch_size())
        this->current_idx = 0;
}

int DataHelper::getInputSize()
{
    return 784;
}

int DataHelper::getOutputSize()
{
    return 10;
}

std::vector<double> DataHelper::getInputs()
{
    //須將索引值轉換程資料

    // cout << "get Inputs " << endl;
    // vector<double> datas;
    // for(int i=0;i<784;i++)
    //     datas.push_back(15);

    // cout << "file names size " << this->file_names.size() << endl;

    string filename = this->file_names[current_idx];
    string folder_path = get_current_folder_name(this->isTrain);
    // int folder_string_len = folder_path.size();

    // int folder_string_len = 66;
    // cout << "folder path size = " << folder_string_len << endl;
    // string filename(full_path.begin() + folder_string_len, full_path.end());

    // cout << filename << endl;

    string full_path = folder_path + filename;
    vector<double> datas = get_real_data(full_path);

    return datas;
}

std::vector<double> DataHelper::getOutputs()
{
    //須將索引值轉換程答案
    // int ans_indx;
    // 從檔案名稱中,提取出編號
    string filename = this->file_names[current_idx];
    char label_char = filename[5];
    int label_idx = label_char - '0';
    
    vector<double> labels;
    for(int i=0;i<10;i++){
        if(i == label_idx){
            labels.push_back(1); 
        }else{
            labels.push_back(0); 
        }
    }
    return labels;
}

// #include "DataHelper.hpp"

// DataHelper::DataHelper()
// {
//     this->isTrain = true;
//     this->current_idx = 0;

//     std::vector<double> input_1;
//     input_1.push_back(0);
//     input_1.push_back(0);
//     std::vector<double> output_1;
//     output_1.push_back(0);
//     this->input_datas.push_back(input_1);
//     this->desire_datas.push_back(output_1);

//     std::vector<double> input_2;
//     input_2.push_back(0);
//     input_2.push_back(1);
//     std::vector<double> output_2;
//     output_2.push_back(1);
//     this->input_datas.push_back(input_2);
//     this->desire_datas.push_back(output_2);

//     std::vector<double> input_3;
//     input_3.push_back(1);
//     input_3.push_back(0);
//     std::vector<double> output_3;
//     output_3.push_back(1);
//     this->input_datas.push_back(input_3);
//     this->desire_datas.push_back(output_3);

//     std::vector<double> input_4;
//     input_4.push_back(1);
//     input_4.push_back(1);
//     std::vector<double> output_4;
//     output_4.push_back(0);
//     this->input_datas.push_back(input_4);
//     this->desire_datas.push_back(output_4);

//     int SIZE = 5;
//     char line[SIZE];
// }

// DataHelper::~DataHelper()
// {
// }

// void DataHelper::TrainingMode()
// {
//     this->isTrain = true;
// }
// void DataHelper::InferenceMode()
// {
//     this->isTrain = false;
// }

// int DataHelper::batch_size()
// {
//     return 4;
// }

// void DataHelper::move_next()
// {
//     this->current_idx++;
//     if (current_idx >= this->batch_size())
//         this->current_idx = 0;
// }

// int DataHelper::getInputSize()
// {
//     return 2;
// }

// int DataHelper::getOutputSize()
// {
//     return 1;
// }

// std::vector<double> DataHelper::getInputs()
// {
//     return this->input_datas[this->current_idx];
// }

// std::vector<double> DataHelper::getOutputs()
// {
//     return this->desire_datas[this->current_idx];
// }
