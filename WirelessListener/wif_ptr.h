#include "free_deleter.h"

extern "C"
{
#include "core.h"
}

#include <memory>

typedef std::unique_ptr<struct wif, free_delete<struct wif>> wif_ptr;