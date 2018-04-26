#include <iostream>
#include <vector>
#include <memory>
#include "bmp_reader.h"

using namespace std;

int main()
{
    // auto img_reader = make_unique<CBmpReader>();
    // img_reader->Load("data.bmp");
    CBmpReader img_reader;
    img_reader.Load("/media/lungyu/9e06a864-1801-4ef8-8b5a-d45c2ded9305/MNIST_data/train/train_0_1.txt");
    // CBmpReader reader;
    // reader.Load("data.bmp");

    for (int i = 0; i < img_reader.GetHeight(); i++)
    {
        for (int j = 0; j < img_reader.GetWidth(); j++)
        {
            cout << "[" << i << "," << j << "]" << img_reader.GetBlueAt(i, j) << endl;
        }
    }

    return 0;
}