#include "common_lib.h"
#include "syscall_if.h"
#include "um_lib_helper.h"

STATUS
__main(
    DWORD       argc,
    char**      argv
)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    QWORD* orphan_threads;

    SyscallGetNumberOfOrphanThreads(&orphan_threads);

    LOG("Number of Orphan Threads: 0x%x\n", orphan_threads);

    return STATUS_SUCCESS;
}