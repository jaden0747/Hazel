#pragma once

#include <memory>

#include "PlatformDetection.h"

#ifdef DEBUG
#if defined(HZ_PLATFORM_WINDOWS)
#define HZ_DEBUGBREAK() __debugbreak()
#elif defined(HZ_PLATFORM_LINUX)
#include <signal.h>
#define HZ_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#else
#define HZ_DEBUGBREAK()
#endif

#ifndef HZ_ENABLE_ASSERTS
#define HZ_ASSERT(x, ...)                                                                                              \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                            \
            HZ_DEBUGBREAK();                                                                                           \
        }                                                                                                              \
    }
#define HZ_CORE_ASSERT(x, ...)                                                                                         \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                       \
            HZ_DEBUGBREAK();                                                                                           \
        }                                                                                                              \
    }
#else
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn)                                                                                           \
    [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace hazel
{

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> createScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> createRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace hazel
