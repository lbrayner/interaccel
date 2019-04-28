#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

void raise_process_priority(void);
int is_process_running(char const * const process_name);

#ifdef __cplusplus
}
#endif

#endif
