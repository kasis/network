#ifndef MNIST_H_INCLUDED
#define MNIST_H_INCLUDED
#include "../lib/matrix.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <stdint.h>
using namespace std;

/*
 * type of array for storing MNIST images
 */
typedef vector<Matrix<unsigned char>* > mnistArr;

/*
 *type for storing MNIST images
 */
typedef Matrix<unsigned char> mnistImg;

template <class T>

static void endswap(T *objp)
{
  unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
  std::reverse(memp, memp + sizeof(T));
}
/*
 * Class for reading images from MNIST database
 */
class mnistImage
{
    private:
        string filename;
        ifstream file;
        string openErr;
        string readErr;
        bool isOpen;
        uint32_t width;
        uint32_t height;
        uint32_t size;
        uint32_t magic;
    public:
        mnistImage();
        void open(string str);
        mnistArr* getImages(int count=-1);
        mnistImg* getImage();
        mnistImg* compressImage(mnistImg *img, int k,int _h=-1, int _w=-1);
        int getSize();
        int getHeight();
        int getWidth();
};
/*
 * Class for reading labels from MNIST database
 */
class mnistLabel
{
    private:
        string filename;
        ifstream file;
        string openErr;
        string readErr;

        bool isOpen;
        uint32_t size;
        uint32_t magic;
    public:
        mnistLabel();
        bool open(string str);
        int getLabel();
        vector<int>* getLabels(int count=-1);
        int getSize();

};
/*
 * initializing parameters
 */
mnistImage::mnistImage()
{
    width=0;
    height=0;
    size=0;
    magic=0;
    isOpen=0;
    openErr="In function 'mnistImage::open()': Error. Can't open file.";
    readErr="In function 'mnistImage::getImage()': Error. Unexpected end of file.";
}
/*
 * open file containing images
 @param str - name of file
 */
void mnistImage::open(string str)
{
    if (isOpen) file.close();
    filename=str;
    file.open(str.c_str(),ios::binary);
    if (!file)
    {
        throw openErr;
    }
    isOpen=1;
    file.read(reinterpret_cast<char*>(&magic), 4);
    endswap(&magic);
    file.read(reinterpret_cast<char*>(&size), 4);
    endswap(&size);
    file.read(reinterpret_cast<char*>(&height), 4);
    endswap(&height);
    file.read(reinterpret_cast<char*>(&width), 4);
    endswap(&width);
}
/*
 * return number of images in the file
 */
int mnistImage::getSize()
{
    return size;
}
/*
 * return height of one image
*/
int mnistImage::getHeight()
{
    return height;
}
/*
 * return width of one image
 */
int mnistImage::getWidth()
{
    return width;
}
/*
 * read current image
 */
mnistImg* mnistImage::getImage()
{
    mnistImg *img= new mnistImg(width*height);
    for (int i=0; i<width*height; i++)
    {
       unsigned char pixel;

        //file >> pixel;
        file.read(reinterpret_cast<char*>(&pixel), 1);

        //cout << pixel << endl;
        (*img)(i,0)=pixel;
    }
    return img;
}
mnistImg* mnistImage::compressImage(mnistImg *img, int k, int _h, int _w)
{
    try
    {
        if (_w==-1)
        {
            _h=height;
            _w=width;
        }
        int n=_h/k;
        int m=_w/k;
        mnistImg *nimg= new mnistImg(n*m);
        int arr[n][m];
        memset(arr,0,sizeof(arr));
        for (int i=0; i<_h; i++)
        {
            for (int j=0; j<_w; j++)
            {
                arr[i/k][j/k]+=(*img)(i*_w+j,0);
            }
        }
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<m; j++)
            {
                (*nimg)(i*m+j,0)=arr[i][j]/k/k;
            }
        }
        return nimg;
    }
    catch(string err)
    {
        err="Error. Called from mnistImage::compressImage():\n"+err;
        throw err;
    }
}
/*
 * read all images in a vector
 */
mnistArr* mnistImage::getImages(int count)
{
    mnistArr *arr=new mnistArr;
    if (count==-1) count=size;
    for (int i=0; i<count; i++)
    {
        try
        {
            arr->push_back(getImage());
        }
        catch (string err)
        {
            throw err;
        }
    }
    return arr;
}

/*
 * initializing parameters
 */
mnistLabel::mnistLabel()
{
    size=0;
    magic=0;
    isOpen=0;
    openErr="In function 'mnistLabel::open()': Error. Can't open file.";

}
/*
 * open file containing labels
 @param str - name of file
 */
bool mnistLabel::open(string str)
{
    if (isOpen) file.close();
    filename=str;
    file.open(str.c_str(),ios::binary);
    if (!file)
    {
        throw openErr;
    }
    isOpen=1;
    file.read(reinterpret_cast<char*>(&magic), 4);
    endswap(&magic);
    file.read(reinterpret_cast<char*>(&size), 4);
    endswap(&size);
    return 1;
}
/*
 * read current label
 */
int mnistLabel::getLabel()
{
    unsigned char byte;
    //file>> byte;
    file.read(reinterpret_cast<char*>(&byte), 1);
    return byte;
}
/*
 * read all labels to vector
 */
vector<int>* mnistLabel::getLabels(int count)
{
    if (count==-1) count=size;
    vector<int>* arr=new vector<int>;
    for (int i=0; i<count; i++)
    {
        arr->push_back(getLabel());
    }
    return arr;
}

/*
 * return number of labels in file
 */
int mnistLabel::getSize()
{
    return size;
}
#endif // MNIST_H_INCLUDED
