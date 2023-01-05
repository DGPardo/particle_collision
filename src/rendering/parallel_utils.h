#ifndef PARALLEL_UTILS_H_
#define PARALLEL_UTILS_H_


#include <mutex>


namespace render
{

static std::mutex mtx{};

}


#endif