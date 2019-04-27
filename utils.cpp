#include <windows.h>

#include "utils.h"

void raise_process_priority(void)
{
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
}
