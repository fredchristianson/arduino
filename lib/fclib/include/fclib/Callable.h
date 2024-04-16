#ifndef _FCLIB_CALLABLE_H_
#define _FCLIB_CALLABLE_H_
#include <functional>

namespace FCLIB
{
    using SimpleCallable = std::function<void()>;

    template <typename T>
    using Callback = std::function<void(T)>;

    template <typename T>
    using BoolCallback = std::function<bool(T)>;

    template <typename T>
    using Calculate = std::function<T(T)>;
}
#endif