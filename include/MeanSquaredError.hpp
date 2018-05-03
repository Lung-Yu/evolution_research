#ifndef TYGR_SQUARED_ERROR_HPP
#define TYGR_SQUARED_ERROR_HPP

#include <vector>
#include <iostream>
#include <cmath>

class MeanSquaredError{

    private:
        void calculate();

        double mse(double predit,double desire);
        double differential_mse(double predit,double desire);
    protected:
        std::vector<double> predits;
        std::vector<double> desires;

        std::vector<double> losses;
        std::vector<double> errors;
        double loss;
    public:
        MeanSquaredError(std::vector<double> predit,std::vector<double> desire);
        ~MeanSquaredError();
        double getloss();
        std::vector<double> getLosses();    //用於各個評估其損失函數
        std::vector<double> getErrors();    //用於神經修正為 loss func 的導函數
};

#endif
