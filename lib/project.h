#ifndef PROJECT_H_INCLUDED
#define PROJECT_H_INCLUDED
#include <vector>
#include <fstream>
#include "../lib/mnist.h"
#include <time.h>
#include <stdlib.h>
/*train first num examples from dataset
 @param num - number of examples
 */
void trainNetwork(int num = -1)
{
    try
    {

        string winname="data/big_w1.txt";
        string woutname="data/big_w1.txt";
        string logname="data/log1.txt";
        cout << "Enter input weights file name: ";
        cin >> winname;
        cout << "Enter output weights file name: ";
        cin >> woutname;

        cout << "Enter log file name: ";
        cin >> logname;
        cout << "Enter number of training examples: ";
        cin >> num;
        ofstream out(logname.c_str());
        if (!out)
        {
            string err="Error. In function trainNetwork: Can't open log file.";
            throw err;
        }
        mnistImage mimg;
        mimg.open("data/mnist/train-images.idx3-ubyte");
        mnistLabel mlbl;
        mlbl.open("data/mnist/train-labels.idx1-ubyte");
        mnistArr *iarr=new mnistArr;
        vector<int> *larr=new vector<int>;
        int count=0;
        if (num==-1) num=mimg.getSize();
        int k=1;
        for (int i=0; i<mimg.getSize(); i++)
        {
            int lbl=mlbl.getLabel();
            mnistImg *img=mimg.getImage();
            //if (lbl != 0  && lbl!=1) continue;
            img=mimg.compressImage(img,k);
            iarr->push_back(img);
            larr->push_back(lbl);
            count++;
            if (count>=num) break;
        }
        out << iarr->size() << endl;
        network net;
        net.setX(iarr);
        net.setY(larr);
        //net.initWeights();
        net.readWeights(winname.c_str());
        double c=net.cost();
        out << c << endl;
        cout << c << endl;

        for (int i=0; i<50; i++)
        {
          //  cout << i << endl;
            net.train(5);
            c=net.cost();
            out << "time: " << time(NULL) << " # of iterations: " << i*5+5 << " cost: " << c << endl;
            cout << "time: " << time(NULL) << " # of iterations: " << i*5+5 << " cost: " << c << endl;

            net.saveWeights(woutname.c_str());
        }
    }
    catch (string err)
    {
        err="Error. Called from trainNetwork():\n"+err;
        throw err;
    }
}
struct NetworkTest
{
    int example_num;
    int guessed_num;
    double accuracy;
    double digit_acc[10];
    double digit_all[10];
    double digit_good[10];
};
NetworkTest * testNetwork(string w_file,int scale=1,int num=10000)
{
    try
    {
        mnistImage mimg;
        mimg.open("../data/mnist/t10k-images.idx3-ubyte");
        //mimg.open("../data/mnist/train-images.idx3-ubyte");

        mnistLabel mlbl;
        mlbl.open("../data/mnist/t10k-labels.idx1-ubyte");
        //mlbl.open("../data/mnist/train-labels.idx1-ubyte");

        mnistArr *iarr=new mnistArr;
        vector<int> *larr=new vector<int>;
        iarr=mimg.getImages(num);
        larr=mlbl.getLabels(num);
        for (int i=0; i<iarr->size(); i++)
        {
            (*iarr)[i]=mimg.compressImage((*iarr)[i],scale);
        }
        network net;
        net.readWeights(w_file);
        int good=0;
        //cerr << "ok" <<endl;
        int digit_good[10];
        int digit_all[10];
        memset(digit_good,0,sizeof(digit_good));
        memset(digit_all,0,sizeof(digit_all));
        for (int i=0; i<iarr->size(); i++)
        {
            int pred=net.predict((*iarr)[i]);
            if (pred==(*larr)[i])
            {
                good++;
                digit_good[pred]++;
            }
            digit_all[pred]++;
        }
        //cerr << "ok" <<endl;

        NetworkTest *test= new NetworkTest;
        test->example_num=num;
        test->guessed_num=good;
        test->accuracy=double(good)/num*100;
        for (int i=0; i<10; i++)
        {
            test->digit_acc[i]=double(digit_good[i])/digit_all[i]*100;
            test->digit_all[i]=digit_all[i];
            test->digit_good[i]=digit_good[i];

        }
        return test;
    }
    catch (string err)
    {
        err="Error. Called from testNetwork():\n"+err;
        throw err;
    }
}
struct ImageSample
{
    matrix<unsigned char> *img;
    int label;
    int pred;
};

ImageSample *getImageSample(mnistArr *arr, vector<int> *y, string w_file, int scale=1)
{
    try
    {
        srand(time(NULL));
        int num=rand()%arr->size();
        ImageSample *smpl= new ImageSample;
        network net;
        net.readWeights(w_file);
        mnistImage mimg;
        mnistImg *cimg=mimg.compressImage((*arr)[num],scale,28,28);
        smpl->pred=net.predict(cimg);
        smpl->label=(*y)[num];
        smpl->img=((*arr)[num]);
        return smpl;
    }
    catch (string err)
    {
        err="Error. Called from getImageSample():\n" + err;
        throw err;
    }
}


#endif // PROJECT_H_INCLUDED
