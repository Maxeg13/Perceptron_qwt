#ifndef PERCEPTRON_H
#define PERCEPTRON_H


class neuron
{
public:
    float state_in;
    float state;
    float err_in;
    float err;

    virtual float act(float x);
    virtual float actDer(float x);

    neuron();
};

class integrNeuron:public neuron
{
public:
    float act(float x)
    {
        return x;
    }
    float actDer(float x)
    {
        return 1;
    }
};



class layer
{
public:
    layer* inLayer;
    int size;
    int size_inp;
    neuron *n;
    float** w;
    float a;
    layer();
    layer(int N,int mode,layer*);

    void reset_w();
    void set(float* x);
    void refresh();
    void getErr(float* t);
    void pushErr();
    void refreshW();
};





#endif // PERCEPTRON_H
