#ifndef __RESULT_H__
#define __RESULT_H__

#include <exception>

namespace Utilis
{

    // class for wrapping results and sending exceptions out of function
    template <typename T>
    class Result
    {
    private:
        bool isSuccess;
        T data;
        std::exception e;

    public:
        Result(/* args */);
        Result(T data);
        Result<T> setFaliure(T data);
        Result<T> setFaliure(std::exception e);
        Result<T> setFaliure(T data, std::exception e);
        static Result<T> newSuccess(T data);
        static Result<T> newFaliure(T data);
        static Result<T> newFaliure(std::exception e);
        static Result<T> newFaliure(T data, std::exception e);

        ~Result();

        bool getIsSuccess() const { return isSuccess; }
        void setIsSuccess(bool isSuccess_) { isSuccess = isSuccess_; }

        T getData() const { return data; }
        void setData(const T &data_) { data = data_; }

        std::exception getE() const { return e; }
        void setE(const std::exception &e_) { e = e_; }
    };

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
    }

    template <typename T>
    Result<T> Result<T>::setFaliure(std::exception e)
    {
        this->isSuccess = false;
        this->e = e;
    }

    template <typename T>
    Result<T> Result<T>::setFaliure(T data, std::exception e)
    {
        this->data = data;
        this->isSuccess = false;
        this->e = e;
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

#endif // __RESULT_H__
