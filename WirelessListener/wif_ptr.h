#include "free_deleter.h"

extern "C"
{
#include "core.h"
}

#include <memory>

typedef std::unique_ptr<wif, free_delete> wif_ptr;