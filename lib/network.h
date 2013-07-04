#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include "matrix.h"
#include "mnist.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>
class network
{
    private:
        vector<Matrix<double> > X;
        vector<Matrix<double> > Y;

        double range;//range of weights' values
        double rate;//learing rate
        int hiddenLayersNum;
        vector<Matrix<double> > weights;
        Matrix<double> makeX(Matrix<unsigned char> *_X);


    public:
        network(int _hiddenLayersNum=1);
        //forwardProp();
        void readWeights(string filename);
        void initWeights();
        void saveWeights(string filename);
        void setX(mnistArr *_X);
        void setY(vector<int> *_Y);
        Matrix<double>* predict(Matrix<double> *_X);
        double cost();
        void backProp(bool f);
        void train(int num=1);
        int predict(Matrix<unsigned char> *_X);
};
/*
 * constructor
 */
network::network(int _hiddenLayersNum)
{
    hiddenLayersNum=_hiddenLayersNum;
    range=1;
    rate=1;
    weights.resize(hiddenLayersNum);
}
/*
 * making from char Matrix double Matrix and adding bias term
 @param _X - input Matrix
 */
Matrix<double> network::makeX(Matrix<unsigned char> *_X)
{
    Matrix<double> cur(_X->getHeight()+1,1);
    cur(0,0)=1; // adding bias term
    for (int j=1; j<_X->getHeight()+1; j++)
        cur(j,0)=double((*_X)(j-1,0));
    return cur;
}
/*
 * make input set
 @param *_X - vector of examples
 */
void network::setX(mnistArr *_X)
{
    if (_X->empty())
    {
        string err="In function network::setX(): Error. Input vector is empty.";
        throw err;
    }

    for (int i=0; i<_X->size(); i++)
    {
        X.push_back(makeX((*_X)[i]));
    }
}
/*
 * making st of answers from labels
 */

void network::setY(vector<int> *_Y)
{
    int maxl=*max_element(_Y->begin(),_Y->end());
    //cerr << maxl<< endl;
    int minl=*min_element(_Y->begin(),_Y->end());
    //cerr << minl << endl;
    int num_labels=maxl-minl+1;
    Y.resize(_Y->size());
    for (int i=0; i< Y.size(); i++)
    {
        Y[i]= Matrix<double>(num_labels,1);
        Y[i]((*_Y)[i]-minl,0)=1;
    }

}

/*
 * read weights from file
 @param filenema - name of input file
 */
void network::readWeights(string filename)
{
    ifstream in(filename.c_str());
    if (!in)
    {
        string err="in function network::readWeigths(): Error. Can't open input file.";
        throw err;
    }
    for (int i=0; i<weights.size(); i++)
    {
        Matrix<double> a(1,1);
        in >> weights[i];
    }
}

/*
 * init weights with random values
 */
void network::initWeights()
{
    if (X.empty())
    {
        string err="In function network::initWeights(): Error. Features vector is empty.";
        throw err;
    }
    if (Y.empty())
    {
        string err="In function network::initWeights(): Error. Labels vector is empty.";
        throw err;
    }
    int xnum=X[0].getHeight();
    int ynum=Y[0].getHeight();
    int hnum=xnum/10+1;//number of elements in hidden layer
    for (int i=0; i<weights.size(); i++)
    {
        if (!i) weights[i]=Matrix<double>(xnum,hnum);
        else if (i==weights.size()-1) weights[i]=Matrix<double>(hnum,ynum);
        else weights[i]=Matrix<double>(hnum,hnum);
        weights[i].randomize(-range,range);
    }
}
/*
 * save weights to file
 */
void network::saveWeights(string filename)
{
    ofstream out(filename.c_str());
    if (!out)
    {
        string err="In function network::saveWeights(): Error. Can't open output file.";
        throw err;
    }
    for (int i=0; i<weights.size(); i++)
        out << weights[i];
    out.close();
}
/*
 * predict output given input features _X
 @param _X - input features
 @return - probability ouput equal 1
 */
Matrix<double>* network::predict(Matrix<double> *_X)
{
    try
    {
        Matrix<double> start(1,1);
        (start)=_X->transpose();//makeX(_X);

        //start->printMatrix(cout);
        Matrix<double> middle(1,1);

        (middle)=(start)*theta1->transpose();//[1xmiddle_layer_num]

        for (int i=0; i<middle.getWidth(); i++)
        {
            middle.set(0,i,1.0/(1+pow(M_E,-middle.get(0,i))));
        }
        middle.set(0,0,1);
        //middle.printMatrix(cout);
        Matrix<double> *final= new Matrix<double>(1,1);
        (*final)=(middle)*theta2->transpose();//[1xlabel_num]
        //final->printMatrix(cout);
        for (int i=0; i<final->getWidth(); i++)
        {
            final->set(0,i,1.0/(1+pow(M_E,-final->get(0,i))));
        }

        //cerr << "finale ok" << endl;
        //middle->~matrix();
        //start->~matrix();
        (*final)=final->transpose();
        return final;
    }
    catch (string err)
    {
        err="Error. Called from network::predict():\n"+err;
        throw err;
    }
}
/*
 * computing cost function
 */
double network::cost()
{
    if (X->size() != Y->size())
    {
        string err="in function network::train(): Error. Number of features does not match the number of results.";
        throw err;
    }
    try
    {
        double cost=0;
        int lnum=(*Y)[0]->getHeight();
        for (int i=0; i<X->size(); i++)
        {
            matrix<double> *pred=predict((*X)[i]);
            for (int j=0; j<lnum; j++)
            {
                double cpred=pred->get(j,0);
                double cy=(*Y)[i]->get(j,0);
                cost+=cy*log(cpred)+(1-cy)*log(1-cpred);
            }
            pred->~Matrix();
        }

        return -cost/X->size();
    }
    catch (string err)
    {
        err="Error. Called from network::cost():\n"+err;
        throw err;
    }
}
/*
 * updating weights, such way that they better fit data
 @param _X - input features
 @param y - expected result
 */
void network::backProp(bool f)
{
    try
    {
        //matrix containing error of each weight
        //delta1= new matrix<double>(theta1->getHeight(),theta1->getWidth());
        //delta2= new matrix<double>(theta2->getHeight(),theta2->getWidth());
        (*delta1)=0.0*(*theta1);
        (*delta2)=0.0*(*theta2);
        //theta1->printMatrix(cout);
        //cerr << delta1-> getHeight() << " " << delta1-> getWidth() <<endl;
        //cout << f << endl;
        //cerr << "ok\n";
        for (int count=0; count<X->size(); count++)
        {
            //if (f && count==204) cr=1;
            //if (cr) cout << "crash: \n";
            matrix<double> _x(1,1);
            _x=(*X)[count]->transpose();//[1x xnum]
            Matrix<double> _y(1,1);
            _y=(*Y)[count]->transpose();

            //_y->printMatrix(cout);
            //if (cr) _x->printMatrix(cout);
            //middle layer of network
            matrix<double> middle(1,1);
            middle=_x*theta1->transpose();//[1xmiddle_layer_num]
            //cerr << "middle ok" <<endl;

            for (int i=0; i<middle.getWidth(); i++)
            {
                middle.set(0,i,1.0/(1+pow(M_E,-middle.get(0,i))));
            }
            middle.set(0,0,1);
            //middle.printMatrix(cout);
            Matrix<double> final(1,1);
            final=middle*theta2->transpose();//[1xlabel_num]
            //final->printMatrix(cout);
            for (int i=0; i<final.getWidth(); i++)
            {
                final.set(0,i,1.0/(1+pow(M_E,-final.get(0,i))));
            }

            //error of ouput layer
            Matrix<double> d3(1,1);//[1 x ynum]
            (d3)=(_y)-final;
            //for (int i=0; i<d3->getHeight(); i++)
            //   d3->set(0,0,(_y->get(i,0)-final->get(i,0))*_y->get(i,0)*(1-_y->get(i,0)));
            //cerr << "d3 ok\n";
            //d3->printMatrix(cout,5);

            //error of middle layer
            Matrix<double> d2(1,1);
            //cerr << "d2 ok\n";
            //cerr << _x->getWidth() << " " << _x->getHeight() << " " << theta1->getWidth() << " " << theta1-> getHeight() << endl;
            (d2)=(d3)*(*theta2);//[1 x 26]
            //cerr << "d2 ok\n";
            for (int i=0; i<d2.getWidth(); i++)
            {
                d2.set(0,i,d2.get(0,i)*middle.get(0,i)*(1-middle.get(0,i)));
            }
            //cerr << "d2 ok\n";

            //d2->printMatrix(cout,10);
            //calculating errors
            //if (cr){
                // Matrix<double> *test=((rate/X->size())*(*((*d2)*(*_x->transpose()))));
               //if (cr) cout << "test ok\n";
              //  test->printMatrix(cout);
               // }
            Matrix<double> derr1(1,1);
            (derr1)=d2.transpose()*(_x);
            (*delta1)=(*delta1)+(derr1);
            //if (cr) cout << "delta1 ok\n";

            (*delta2)=(*delta2)+d3.transpose()*(middle);
            //if (f) cout << count << " ok\n";
            //delta1->printMatrix(cout,10);
            //delta2->printMatrix(cout,10);
            //delete[] middle;
            //_x->free();
            //middle->free();
            //final->free();
            //d2->free();
            //d3->free();
            //derr1->free();
            //_x->free();
            //_y->free();

        }
        //if (f) cout << "end: ok" << endl;
//        delta1->printMatrix(cout,10);
//        delta2->printMatrix(cout,10);

        (*delta1)=rate/X->size()*(*delta1);
        (*delta2)=rate/X->size()*(*delta2);
        //cout << endl;
        //updating weights
        //theta1=(*theta1)+*(rate*(*delta1));
        //theta2=(*theta2)+*(rate*(*delta2));
        //theta1->printMatrix(cout,5);
        //theta2->printMatrix(cout,5);
    }
    catch (string err)
    {
        err="Error. Called from network::backProp()\n"+err;
        throw err;
    }
}
/*
 * update weights with function backProp to fit dataset well
 @param num - number of iterations
 */
void network::train(int num)
{

    if (X->size() != Y->size())
    {
        string err="in function network::train(): Error. Number of features does not match the number of results.";
        throw err;
    }
    try
    {
        for (int count=0; count<num; count++)
        {
            //cout << "step: " << count << endl;
            //bool f=0;
            //if (count==4) f=1;

            backProp(0);
            //cerr << endl;
            //delta1->printMatrix(cout,10);
            //delta2->printMatrix(cout,10);

            //theta1=(*theta1)+(*delta1);
            //theta2=(*theta2)+(*delta2);

            //if (count<=3)
            {

             //   cerr << cost() << endl;
            (*theta1)=(*theta1)+(*delta1);
            (*theta2)=(*theta2)+(*delta2);
            //theta1->printMatrix(cout,5);
            //theta2->printMatrix(cout,5);


            }
        }
    }
    catch (string err)
    {
        err="Error. Called from network::train():\n"+err;
        throw err;
    }
}
/*
 * compute wether answer for given features is 1 or 0
 @param _X - given features
 */
int network::predict(Matrix<unsigned char> *_X)
{
    try
    {
        Matrix<double> *input=makeX(_X);
        Matrix<double> *pred=predict(input);
        int mnum=0;
        double mprob=pred->get(0,0);
        for (int i=1; i<pred->getHeight(); i++)
        {
            double cprob=pred->get(i,0);
            if (cprob>mprob)
            {
                mprob=cprob;
                mnum=i;
            }
        }
        pred->~Matrix();
        input->~Matrix();
        return mnum;
    }
    catch (string err)
    {
        throw err;
    }
}
#endif // NETWORK_H_INCLUDED
