#pragma once

#include <cstddef>

#ifdef DLL_EXPORT
    #define PROJECT_API __declspec(dllexport)
#else
    #define PROJECT_API __declspec(dllimport)
#endif

namespace async {

using handle_t = void *;


PROJECT_API handle_t connect(std::size_t bulk);
PROJECT_API void receive(handle_t handle, const char *data, std::size_t size);
PROJECT_API void disconnect(handle_t handle);

}
