#ifndef RESULT_HPP
#define RESULT_HPP

#include <exception>

namespace Utilis {

// class for wrapping results and sending exceptions out of function
template <typename T>
class Result {
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

    T getData() const { return data; }
    void setData(const T& data_) { data = data_; }

    std::exception getE() const { return e; }
    void setE(const std::exception& e_) { e = e_; }

    bool getIsSuccess() const { return isSuccess; }
    void setIsSuccess(const bool& isSuccess_) { isSuccess = isSuccess_; }
};

} // namespace Utilis

#endif // RESULT_HPP
