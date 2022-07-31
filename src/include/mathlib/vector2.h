//
// Created by Zaiqiang Wu on 2022/07/31.
//

#ifndef PHYSICALENGINE_VECTOR2_H
#define PHYSICALENGINE_VECTOR2_H
#include <cmath>
#include<iostream>
#include<string>

//#include "nlohmann/json.hpp"


//using json = nlohmann::json;

using namespace std;

///
/// Template class for three element vectors.
///
template<class T>
class vector2
{
public:
    vector2();
    vector2(const T x, const T y);
    vector2(const vector2<T>& v);

    // utility operations
    vector2<T>& zero();
    vector2<T>& set(const T x, const T y);
    vector2<T>& normalize();

    // math operations
    const T norm() const;
    const T sum() const;
    const T dot(const vector2<T>&) const;
    //const vector2<T> cross(const vector2<T>&) const;
    const vector2<T> abs() const;

    // operators
    vector2<T>& operator= (const vector2<T>& v);        // assignment

    const T operator[] (const int i) const;             // indexing
    T& operator[] (const int i);                        // indexing

    const vector2<T> operator-();                       // unary negate

    vector2<T>& operator+=(const T s);                  // scalar addition
    vector2<T>& operator-=(const T s);                  // scalar subtraction
    vector2<T>& operator*=(const T s);                  // scalar multiplication
    vector2<T>& operator/=(const T s);                  // scalar division

    vector2<T>& operator+=(const vector2<T>& v);        // vector addition
    vector2<T>& operator-=(const vector2<T>& v);        // vector subtraction
    vector2<T>& operator*=(const vector2<T>& v);        // element-wise multiplication
    vector2<T>& operator/=(const vector2<T>& v);        // element-wise division

    const vector2<T> operator < (const T s);            // compare each element with s, return vector of 1 or 0 based on test
    const vector2<T> operator > (const T s);

    const vector2<T> operator < (const vector2<T>& v);  // element-wise less than comparion, return vector of 1 or 0 based on test
    const vector2<T> operator > (const vector2<T>& v);  // element-wise greater than comparion, return vector of 1 or 0 based on test

    bool operator == (const vector2<T>& v);             // test vector for equality
    bool operator != (const vector2<T>& v);             // test vector for inequality

    T* ptr() { return _v; }                                // return reference to array (use with caution)

    /*void assign(json item)
    {
        //cout <<"item" <<item << endl;
        _v[0] = item["z"].get<float>();
        _v[1] = item["y"].get<float>();
        _v[2] = item["x"].get<float>();
    }*/


    void PrintInfo()
    {
        cout << "x:" <<_v[0]<< " y:" <<_v[1]<< endl;
    }

private:
    T _v[2];
};


//
// Binary operations
//
/*
inline const vector2<T> operator+ (const vector2<T>& v, const T& s);  // scalar operations
inline const vector2<T> operator- (const vector2<T>& v, const T& s);
inline const vector2<T> operator* (const vector2<T>& v, const T& s);
inline const vector2<T> operator/ (const vector2<T>& v, const T& s);
inline const vector2<T> operator+ (const vector2<T>& v1, const vector2<T>& v2);  // element-wise addition
inline const vector2<T> operator- (const vector2<T>& v1, const vector2<T>& v2);  // element-wise subtraction
inline const vector2<T> operator* (const vector2<T>& v1, const vector2<T>& v2);  // dot product
inline const vector2<T> operator^ (const vector2<T>& v1, const vector2<T>& v2);  // cross product
*/

//
// Function definitions
//


template <class T>
vector2<T>::vector2()
{
    _v[0] = 0.0;
    _v[1] = 0.0;
}


template <class T>
vector2<T>::vector2(const vector2<T>& v)
{
    _v[0] = v[0];
    _v[1] = v[1];
}


template <class T>
vector2<T>::vector2(const T x, const T y)
{
    _v[0] = x;
    _v[1] = y;
}


template <class T>
vector2<T>& vector2<T>::zero()
{
    _v[0] = 0.0;
    _v[1] = 0.0;
    return *this;
}


template <class T>
vector2<T>& vector2<T>::set(const T x, const T y)
{
    _v[0] = x;
    _v[1] = y;
    return *this;
}


template <class T>
inline const T vector2<T>::operator [] (const int i) const
{
    return _v[i];
}


template <class T>
T& vector2<T>::operator [] (const int i)
{
    return _v[i];
}


template <class T>
inline const vector2<T> vector2<T>::abs() const
{
    return vector2<T>(std::abs(_v[0]), std::abs(_v[1]));
}


template <class T>
inline const T vector2<T>::sum() const
{
    return _v[0] + _v[1];
}


template <class T>
inline const T vector2<T>::dot(const vector2<T>& v) const
{
    return _v[0] * v[0] + _v[1] * v[1];
}


/*template <class T>
inline const vector2<T> vector2<T>::cross(const vector2<T>& v) const
{
    return vector2<T>(
            (_v[1] * v[2]) - (_v[2] * v[1]),
            (_v[2] * v[0]) - (_v[0] * v[2]),
            (_v[0] * v[1]) - (_v[1] * v[0])
    );
}*/


template <class T>
inline const T vector2<T>::norm() const
{
    return (T)sqrt(dot(*this)); // cast to type
}


template <class T>
vector2<T>& vector2<T>::normalize()
{
    T n = norm();
    if (n) {
        _v[0] /= n;
        _v[1] /= n;
    }
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator= (const vector2<T>& v)
{
    _v[0] = v[0];
    _v[1] = v[1];
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator += (const vector2<T>& v)
{
    _v[0] += v[0];
    _v[1] += v[1];
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator += (T v)
{
    _v[0] += v;
    _v[1] += v;
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator -= (const vector2<T>& v)
{
    _v[0] -= v[0];
    _v[1] -= v[1];
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator -= (T v)
{
    _v[0] -= v;
    _v[1] -= v;
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator *= (T v)
{
    _v[0] *= v;
    _v[1] *= v;
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator *= (const vector2<T>& v)
{
    _v[0] *= v[0];
    _v[1] *= v[1];
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator /= (T v)
{
    _v[0] /= v;
    _v[1] /= v;
    return *this;
}


template <class T>
vector2<T>& vector2<T>::operator /= (const vector2<T>& v)
{
    _v[0] /= v[0];
    _v[1] /= v[1];
    return *this;
}


template<class T>
inline const vector2<T> vector2<T>::operator < (const T s)
{
    return vector2<T>(_v[0] < s, _v[1] < s);
}


template<class T>
inline const vector2<T> vector2<T>::operator > (const T s)
{
    return vector2<T>(_v[0] > s, _v[1] > s);
}


template<class T>
inline const vector2<T> vector2<T>::operator < (const vector2<T>& v)
{
    return vector2<T>(_v[0] < v[0], _v[1] < v[1]);
}


template<class T>
inline const vector2<T> vector2<T>::operator > (const vector2<T>& v)
{
    return vector2<T>(_v[0] > v[0], _v[1] > v[1]);
}


template<class T>
inline const vector2<T> vector2<T>::operator - ()
{
    return vector2<T>(-_v[0], -_v[1]);
}


template <class T>
inline bool vector2<T>::operator == (const vector2<T>& v)
{
    return    _v[0] == v[0]
              && _v[1] == v[1];
}


template <class T>
inline bool vector2<T>::operator != (const vector2<T>& v)
{
    return    _v[0] != v[0]
              || _v[1] != v[1];
}



template<class T>
inline const vector2<T> operator && (const vector2<T>& v1, const vector2<T>& v2)
{
    return vector2<T>(v1[0] && v2[0], v1[1] && v2[1]);
}


template<class T>
inline const vector2<T> operator || (const vector2<T>& v1, const vector2<T>& v2)
{
    return vector2<T>(v1[0] || v2[0], v1[1] || v2[1]);
}


template <class T>
inline const vector2<T> operator + (const vector2<T>& v, const T& s)
{
    return vector2<T>(v) += s;
}


template <class T>
inline const vector2<T> operator - (const vector2<T>& v, const T& s)
{
    return vector2<T>(v) -= s;
}


template <class T>
inline const vector2<T> operator * (const vector2<T>& v, const T& s)
{
    return vector2<T>(v) *= s;
}


template <class T>
inline const vector2<T> operator / (const vector2<T>& v, const T& s)
{
    return vector2<T>(v) /= s;
}


template <class T>
inline const vector2<T> operator + (const vector2<T>& v1, const vector2<T>& v2)
{
    return vector2<T>(v1) += v2;
}


template <class T>
inline const vector2<T> operator - (const vector2<T>& v1, const vector2<T>& v2)
{
    return vector2<T>(v1) -= v2;
}


template <class T>
inline const T operator * (const vector2<T>& v1, const vector2<T>& v2)
{
    return v1.dot(v2);
}


template <class T>
inline const vector2<T> operator ^ (const vector2<T>& v1, const vector2<T>& v2)
{
    return v1.cross(v2);
}
#endif //PHYSICALENGINE_VECTOR2_H
