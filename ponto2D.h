#ifndef PONTO2D_H
#define PONTO2D_H
#include <initializer_list>

using namespace std;

initializer_list<int> il;

template <typename T>
class ponto2D
{
    public:
        ponto2D();
        ponto2D(T a, T b);
        ponto2D& operator=(const ponto2D& other);
        ponto2D& operator=(initializer_list<T> il);
        T x;
        T y;
};

template <typename T>
ponto2D<T>::ponto2D()
{
    x = 0;
    y = 0;
}

template <typename T>
ponto2D<T>::ponto2D(T a, T b)
{
    x = a;
    y = b;
}

template <typename T>
ponto2D<T>& ponto2D<T>::operator=(const ponto2D<T>& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    x = rhs.x;
    y = rhs.y;

    //assignment operator
    return *this;
}

template <typename T>
ponto2D<T>& ponto2D<T>::operator=(initializer_list<T> il)
{
	auto iter = il.begin();
    x = *iter;
    iter++;
    y = *iter;
    //assignment operator
    return *this;
}




#endif
