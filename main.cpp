#include <iostream>
#include "lib/network.h"
#include "lib/mnist.h"
#include "lib/project.h"
#include <string.h>

using namespace std;

int main()
{
    mnistImage img;
    try
    {
        //trainNetwork(100);
        ofstream out("data/net_data2.txt");
        NetworkTest *t=testNetwork("data/w7.txt",1,10000);
        out << t-> example_num << " " << t->guessed_num << " " << t->accuracy << endl;
        out.precision(3);
        for (int i=0; i<10; i++)
        {
            //out << fixed << i << ": " << t->digit_good[i] << " " << t->digit_all[i] << " " << t->digit_acc[i] << endl;
            out << int(t->digit_acc[i]) << endl;
        }
        /*mnistImage mimg;
        mimg.open("../data/mnist/train-images.idx3-ubyte");
        mnistLabel mlbl;
        mlbl.open("../data/mnist/train-labels.idx1-ubyte");
        mnistArr *iarr=new mnistArr;
        vector<int> *larr=new vector<int>;
        iarr=mimg.getImages(1000);
        int k=2;
        for (int i=0; i<iarr->size(); i++)
        {
            (*iarr)[i]=mimg.compressImage((*iarr)[i],k);
        }
        larr=mlbl.getLabels(1000);
        cerr << "ok" << endl;
        ImageSample * smpl=getImageSample(iarr,larr,"data/w1.txt");
        cout << smpl-> pred << " " << smpl->label <<endl;
        //smpl->img->printMatrix(cout);
        for (int i=0; i<mimg.getHeight()/k; i++)
        {
            for (int j=0; j<mimg.getWidth()/k; j++)
                cout << ((smpl->img->get(i*mimg.getWidth()/k+j,0)) ? 1 : 0);
            cout << endl;

        }
        /*ifstream in("data/test.txt");
        int n,m;
        int tnum=0;
        while (!in.eof())
        {
            in >> n >> m;
            //cout << n << endl;
            mnistArr *tarr= new mnistArr;
            vector<int> *y= new vector<int>;
            for (int cnt=0; cnt<n;cnt++)
            {
                matrix<unsigned char> *test= new matrix<unsigned char>(m,1);

                for (int i=0; i<m; i++)
                {
                    int c;
                    in >> c;
                    //cout<< c << endl;
                    test->set(i,0,(unsigned char)c);
                }
                tarr->push_back(test);
                int l;
                in>> l;
                y->push_back(l);
            }

            network p;
            p.setX(tarr);
            p.setY(y);
            //return 0;
            p.initWeights();
            //p.readWeights("data/weights4.txt");
            cout << p.cost() << endl;
            for (int i=0; i<1;i++)
            {
                p.train(200);
                //cout << p.cost() << endl;
            }
            cout << p.cost() << endl;
            //cout << p.predict(tarr[0]) << " "  << y[0] << endl;
            int good=0;
            for (int i=0; i<n; i++)
            {
                if (p.predict((*tarr)[i])==(*y)[i]) good++;
                //cout << p.predict((*tarr)[i]) << " " << (*y)[i] << endl;
            }
            cout << "Test" << ++tnum << ":\n";
            cout << "Guessed " << good << " of " << n << "\nAccuracy: ";

            cout << double(good)/n*100 << endl << endl;
            p.saveWeights("data/weights4.txt");
        }
        //char c=p.predict(tarr[0]);

        /*img.open("../data/mnist/train-images.idx3-ubyte");
        mnistArr arr1=img.getImages(10);
        network p;
        p.setX(&arr1);
        p.initWeights();
        char c=p.predict(arr1[0]);
       // p.saveWeights();
        */
        /*mnistImage img;
        img.open("../data/mnist/train-images.idx3-ubyte");
        mnistImg img1=img.getImage();
        mnistArr x=img.getImages(1);
        //x.push_back(img1);
        mnistLabel lbl;
        lbl.open("../data/mnist/train-labels.idx1-ubyte");
        vector<int> y=lbl.getLabels(100);

        network p;
        p.setX(&x);
        p.setY(y);
        p.initWeights();
        p.train();
        cout << p.predict(img1) << endl;
    */}
    catch (string err)
    {
        cout << err << endl;
        cin >> err;
        return 0;
    }

    return 0;
}
