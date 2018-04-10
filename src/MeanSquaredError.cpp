#include "MeanSquaredError.hpp"

MeanSquaredError::MeanSquaredError(std::vector<double> y, std::vector<double> y_)
{
    this->predits = y;
    this->desires = y_;
    this->loss = 0;

    this->calculate();

    // std::cout << "hello mse" << std::endl;
}

MeanSquaredError::~MeanSquaredError()
{
}

void MeanSquaredError::calculate()
{
    this->errors.clear();
    this->losses.clear();
    int size = (this->predits.size() < this->desires.size()) ? this->predits.size() : this->desires.size();
    double loss_sum = 0;
    for (int i = 0; i < size; i++)
    {
        // double loss = (this->desires[i] - this->predits[i]) * (this->desires[i] - this->predits[i]); //MSE func
        // double error = this->desires[i] - this->predits[i];                                          //differential mse func

        double loss = this->mse(predits[i],desires[i]);
        double error = this->differential_mse(predits[i],desires[i]);

        loss_sum += loss;

        this->errors.push_back(error);
        this->losses.push_back(loss); //將每個一位置的loss記錄下來
    }

    //儲存loss值
    this->loss = (0.5 * loss_sum);
}

double MeanSquaredError::mse(double predit, double desire)
{
    double loss = (desire - predit)*(desire - predit);
    return loss;
}

double MeanSquaredError::differential_mse(double predit, double desire)
{
    double error = (desire - predit);
    return error;
}
//用於顯示評估其損失函數
std::vector<double> MeanSquaredError::getLosses()
{
    return this->losses;
}
//用於神經修正為 loss func 的導函數
std::vector<double> MeanSquaredError::getErrors()
{
    return this->errors;
}

double MeanSquaredError::getloss(){
    return this->loss;
}