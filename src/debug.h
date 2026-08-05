#pragma once
#define EMU_DEBUG

#ifdef EMU_DEBUG
    #define DEBUG_CHECK(cond, msg) \
        if ((cond)) throw std::runtime_error(msg)
#else
    #define DEBUG_CHECK(cond, msg) ((void)0)
#endif
