#pragma  once

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS  1
#endif
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#endif

// 用于告诉VC++，我不强迫使用_s类型的API，_CRT_SECURE_NO_DEPRECATE是_CRT_SECURE_NO_WARNINGS的老版本
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS   1
#endif
#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE  1
#endif

int hello_lambda(int argc, char* argv[]);

int hello_chrono_duration(int argc, char* argv[]);
int hello_chrono_clock(int argc, char* argv[]);
int hello_chrono_timepoint(int argc, char* argv[]);

int hello_align(int argc, char* argv[]);

int hello_progress_timer(int argc, char* argv[]);

int test_unique_ptr(int argc, char* argv[]);

int test_any(int argc, char* argv[]);
int test_any2(int argc, char* argv[]);

int test_variant(int argc, char* argv[]);
int test_variant2(int argc, char* argv[]);

int test_priority_queue(int argc, char* argv[]);

int test_bind_2(int argc, char* argv[]);
int test_bind_3(int argc, char* argv[]);

int test_variadic(int argc, char* argv[]);
int test_variadic2(int argc, char* argv[]);
int test_variadic3(int argc, char* argv[]);
int test_variadic4(int argc, char* argv[]);
int test_variadic5(int argc, char* argv[]);

int hello_atomic_rings(int argc, char* argv[]);
int hello_atomic_islockfree(int argc, char* argv[]);
int hello_atomic_size(int argc, char* argv[]);

int test_thread_local(int argc, char* argv[]);

int hello_share_ptr_1(int argc, char* argv[]);
int hello_share_ptr_2(int argc, char* argv[]);

int hello_weak_ptr(int argc, char* argv[]);

int hello_perfect_forward(int argc, char* argv[]);

int hello_random(int argc, char* argv[]);

int test_coro_main(int argc, char* argv[]);
int test_coro_main2(int argc, char* argv[]);
int test_coro_main3(int argc, char* argv[]);

int hello_right_value_001(int argc, char* argv[]);
int hello_right_value_002(int argc, char* argv[]);
int test_share_ptr_pool(int argc, char* argv[]);

int hello_semaphore(int argc, char* argv[]);