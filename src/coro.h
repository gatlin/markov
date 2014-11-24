/***
 * coro.h
 *
 * Simple coroutine mechanism based on
 *
 *    http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
 */

#ifndef __CORO_H
#define __CORO_H

#include "markov.h"

#define coro_start static int __coro_state=0; switch(__coro_state) { case 0:
#define yield(X) do { __coro_state=__LINE__; return X; \
case __LINE__:; } while (0)
#define coro_finish }

#endif /* __CORO_H */
