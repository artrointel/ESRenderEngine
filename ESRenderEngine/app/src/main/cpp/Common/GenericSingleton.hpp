//
// Created by we.kim on 2017-07-17.
//
#ifndef __GENERIC_SINGLETON_HPP__
#define __GENERIC_SINGLETON_HPP__

// (T = class) To make any class Singleton, declare this macro on the class.
#define MAKE_SINGLETON(T) friend class GenericSingleton<T>; \
public: static T* GetInstance() { return GenericSingleton<T>::GetInstance(); }


template<class T>
class GenericSingleton
{
private:
    static T* mInstance;

public:
    ~GenericSingleton()
    {
        if(mInstance != nullptr)
        {
            delete mInstance;
            mInstance = nullptr;
        }
    }
    static T* GetInstance()
    {
        if (mInstance == nullptr)
        {
            mInstance = new T;
        }
        return mInstance;
    }
};
template<class T>
T* GenericSingleton<T>::mInstance = nullptr;

#endif