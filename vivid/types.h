#pragma once

#ifdef __cplusplus
#define STATIC_ASSERT(cond, name, msg) static_assert(cond, msg)
#define STATIC_ASSERT_NMSG(cond, name) static_assert(cond, #name)
#elif defined(_C1X)
#define STATIC_ASSERT(cond, name, msg) _Static_assert(cond, msg)
#define STATIC_ASSERT_NMSG(cond, name) _Static_assert(cond, #name)
#else
#define STATIC_ASSERT_NMSG(cond, name)
#define STATIC_ASSERT(cond, name, msg)
#endif

#ifdef __cplusplus
#define LFW_DECLARE_ENUM(x, type) enum x : type
#else
#define LFW_DECLARE_ENUM(x, type) enum x
#endif