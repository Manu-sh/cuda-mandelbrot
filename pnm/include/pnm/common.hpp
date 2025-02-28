#pragma once

// i could use template specialization and sfinae to force inlining
// for some of these global function, but it's unnecessarily
// complicated and less flexible

#ifdef FORCED
    #error "FORCED() macro already defined"
#endif

#define FORCED(_UNUSED_) __attribute__((always_inline))
