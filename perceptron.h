#ifndef NET_H
#define NET_H
#include "headers.h"
#include "layer.h"
#include <QDebug>
#include <vector>
using namespace std;
class perceptron
{
public:

    int N;
    float **out;
    layer **lr;
    layer *lrh;
    perceptron(vector<int>& constr);

    float reset_w();

    float** refresh(float* x);

    void learn1(float* x,float* t);
    void learnFunc();
};
#endif // NET_H
