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
    {
        if (dirp->d_reclen == 40)
        {
            files.push_back(string(dirp->d_name)); //將資料夾和檔案名放入vector
        }
    }
    closedir(dp); //關閉資料夾指標
    return files;
}

inline vector<double> get_real_data(string full_file_path)
{
    fstream fin;
    fin.open(full_file_path, ios::in);

    vector<double> datas;
    char buffer_line[5];
    while (fin.getline(buffer_line, sizeof(buffer_line), ','))
    {
        double val = atof(buffer_line);
        datas.push_back(val / 255); 
    }

    return datas;
}

inline string get_current_folder_name(bool is_train_mode)
{
    if(is_train_mode)
//       return string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train/");
	return string("/tmp/train/");
    else
	return string("/tmp/test/");
//        return string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/test/");
}

DataHelper::DataHelper()
{
    this->current_idx = 0;
    this->TrainingMode();
}

DataHelper::~DataHelper()
{
}

void DataHelper::TrainingMode()
{
    this->file_names.clear();
    this->isTrain = true;

//    string dir = string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train");
    string dir = string("/tmp/train");
    this->file_names = getdir(dir);
}

void DataHelper::InferenceMode()
{
    this->file_names.clear();
    this->isTrain = false;

//    string dir = string("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/test");
    string dir = string("/tmp/test");
    this->file_names = getdir(dir);
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

    string filename = this->file_names[current_idx];
    string folder_path = get_current_folder_name(this->isTrain);

    string full_path = folder_path + filename;
    vector<double> datas = get_real_data(full_path);

    return datas;
}

std::vector<double> DataHelper::getOutputs()
{
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
