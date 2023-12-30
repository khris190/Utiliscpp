#include "Result.hpp"

namespace Utilis {

template <typename T>
Result<T>::Result(/* args */)
{
}

template <typename T>
Result<T>::Result(T data)
{
    this->data = data;
}

template <typename T>
Result<T> Result<T>::setFaliure(T data)
{
    this->data = data;
    this->isSuccess = false;
    this->e = nullptr;
    return this;
}

template <typename T>
Result<T> Result<T>::setFaliure(std::exception e)
{
    this->isSuccess = false;
    this->e = e;
    return this;
}

template <typename T>
Result<T> Result<T>::setFaliure(T data, std::exception e)
{
    this->data = data;
    this->isSuccess = false;
    this->e = e;
    return this;
}

template <typename T>
Result<T> Result<T>::newSuccess(T data)
{
    Result<T> res = Result<T>();
    res->data = data;
    res->isSuccess = true;
    res->e = nullptr;
    return res;
}

template <typename T>
Result<T> Result<T>::newFaliure(T data)
{
    Result<T> res = Result<T>();
    res->data = data;
    res->isSuccess = false;
    res->e = nullptr;
    return res;
}

template <typename T>
Result<T> Result<T>::newFaliure(std::exception e)
{
    Result<T> res = Result<T>();
    res->isSuccess = false;
    res->e = e;
    return res;
}

template <typename T>
Result<T> Result<T>::newFaliure(T data, std::exception e)
{
    Result<T> res = Result<T>();
    res->data = data;
    res->isSuccess = false;
    res->e = e;
    return res;
}

template <typename T>
Result<T>::~Result()
{
}
} // namespace Utilis
