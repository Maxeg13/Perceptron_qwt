#include "perceptron.h"
perceptron::perceptron(vector<int>& constr)
{

    N=constr.size();

    lr=new layer*[N];
    lrh=NULL;
    for(int i=0;i<(N-1);i++)
    {
        lr[i]=new layer(constr[i],1,lrh);
        lrh=lr[i];
    }
    lr[N-1]=new layer(constr[N-1],2,lrh);

    out=new float*[constr[N-1]];
    for(int i=0;i<constr[N-1];i++)
        out[i]=&(lr[N-1]->n[i].state);


}

float perceptron::reset_w()
{
    for (int i=1;i<(N);i++)
        lr[i]->reset_w();
}

float** perceptron::refresh(float* x)
{

    lr[0]->set(x);
    for(int i=1;i<N;i++)
        lr[i]->refresh();

    return out;
}

void perceptron::learn1(float* x,float* t)
{
    refresh(x);
    lr[N-1]->getErr(t);
    for(int i=(N-1);i>1;i--)
    {
        lr[i]->pushErr();
    }
    for(int i=1;i<N;i++)
        lr[i]->refreshW();
}
void perceptron::learnFunc()
{}
