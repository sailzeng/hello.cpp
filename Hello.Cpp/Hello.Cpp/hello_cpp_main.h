
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

