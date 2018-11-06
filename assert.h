#pragma once

#include <sys/systm.h>

#define ASSERT_STR_HELPER(x) #x
#define ASSERT_STR(x) ASSERT_STR_HELPER(x)

// #define assert(x) ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__),0)))

#define	assert(exp) do {						\
	if (__predict_false(!(exp)))					\
		panic("Assertion failed: " ASSERT_STR(exp) "(" __FILE__ ": " ASSERT_STR(__LINE__) ": " ASSERT_STR(__func__)); \
} while (0)
