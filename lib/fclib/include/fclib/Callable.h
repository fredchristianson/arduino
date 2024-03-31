#ifndef _FCLIB_CALLABLE_H_
#define _FCLIB_CALLABLE_H_
#include <functional>

namespace FCLIB
{
    using SimpleCallable = std::function<void()>;

    template <typename T>
    using Callback = std::function<void(T)>;

    template <typename T>
    using TestCallback = std::function<bool(T)>;
}
#endif