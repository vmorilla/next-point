#ifndef _TESTS_H_
#define _TESTS_H_

#ifdef DEBUG

#define run_tests() _run_tests()
void _run_tests(void);

#else

#define run_tests() ((void)0)

#endif

#endif