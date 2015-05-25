#pragma once

extern "C"
{
#include "core.h"
}

#include <memory>

struct wif_delete
{
	void operator()(struct wif* wi)
	{
		wi_close(wi);
	}
};

typedef std::unique_ptr<struct wif, wif_delete> wif_ptr;