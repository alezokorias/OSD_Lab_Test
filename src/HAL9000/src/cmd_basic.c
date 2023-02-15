#include "HAL9000.h"
#include "cmd_fs_helper.h"
#include "display.h"
#include "dmp_io.h"
#include "print.h"
#include "iomu.h"
#include "test_common.h"
#include "strutils.h"
#include "thread_internal.h"
#include "strutils.h"

void
CmdPrintVolumeInformation(
    IN      QWORD           NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    printColor(MAGENTA_COLOR, "%7s", "Letter|");
    printColor(MAGENTA_COLOR, "%17s", "Type|");
    printColor(MAGENTA_COLOR, "%10s", "Mounted|");
    printColor(MAGENTA_COLOR, "%10s", "Bootable|");
    printColor(MAGENTA_COLOR, "%17s", "Offset|");
    printColor(MAGENTA_COLOR, "%17s", "Size|");
    printColor(MAGENTA_COLOR, "\n");

    IomuExecuteForEachVpb(DumpVpb, NULL, FALSE);
}

#pragma warning(push)

// warning C4717: '_CmdInfiniteRecursion': recursive on all control paths, function will cause runtime stack overflow
#pragma warning(disable:4717)
void
CmdInfiniteRecursion(
    IN      QWORD           NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    CmdInfiniteRecursion(NumberOfParameters);
}
#pragma warning(pop)

void
CmdRtcFail(
    IN      QWORD           NumberOfParameters
    )
{
    char buffer[] = "Alex is a smart boy!\n";

    ASSERT(NumberOfParameters == 0);

    strcpy(buffer, "Alex is a very dumb boy!\n");
}

void
CmdRangeFail(
    IN      QWORD           NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    perror("Cannot implement! :(\n");
}

void
(__cdecl CmdBiteCookie)(
    IN      QWORD           NumberOfParameters
    )
{
    char buffer[] = "Alex is a smart boy!\n";

    ASSERT(NumberOfParameters == 0);

    strcpy(buffer + sizeof(buffer) + sizeof(PVOID), "Alex is a very dumb boy!\n");
}

void
(__cdecl CmdLogSetState)(
    IN      QWORD           NumberOfParameters,
    IN      char*           LogState
    )
{
    ASSERT(NumberOfParameters == 1);

    LogSetState(stricmp(LogState, "ON") == 0);
}

void
(__cdecl CmdSetLogLevel)(
    IN      QWORD           NumberOfParameters,
    IN      char*           LogLevelString
    )
{
    LOG_LEVEL logLevel;

    ASSERT(NumberOfParameters == 1);

    atoi32(&logLevel, LogLevelString, BASE_TEN);

    if (logLevel > LogLevelError)
    {
        perror("Invalid log level %u specified!\n", logLevel);
        return;
    }

    printf("Will set logging level to %u\n", logLevel);
    LogSetLevel(logLevel);
}

void
(__cdecl CmdSetLogComponents)(
    IN      QWORD           NumberOfParameters,
    IN      char*           LogComponentsString
    )
{
    LOG_COMPONENT logComponents;

    ASSERT(NumberOfParameters == 1);

    atoi32(&logComponents, LogComponentsString, BASE_HEXA);

    printf("Will set logging components to 0x%x\n", logComponents);

    LogSetTracedComponents(logComponents);
}

void
(__cdecl CmdClearScreen)(
    IN          QWORD       NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    DispClearScreen();
}

void
(__cdecl CmdRunAllFunctionalTests)(
    IN          QWORD       NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    TestRunAllFunctional();
}

void
(__cdecl CmdRunAllPerformanceTests)(
    IN          QWORD       NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    TestRunAllPerformance();
}

void
(__cdecl CmdPrintChildren)(
    IN          QWORD      NumberOfParameters,
    IN          char*      Param
    )
{
    ASSERT(NumberOfParameters == 1);

    DWORD aux;
    atoi(&aux, Param, 10, FALSE);
    TID Tid = (TID)aux;
    
    LOG("Displying children for Thread with TID=%6x%c", Tid);

    PTHREAD pThread = _ThreadReferenceByTid(Tid);

    LIST_ITERATOR it_Children;
    ListIteratorInit(&pThread->ChildrenList, &it_Children);

    PLIST_ENTRY child;
    while ((child = ListIteratorNext(&it_Children)) != NULL) {
        PTHREAD chThread = CONTAINING_RECORD(child, THREAD, ChildListElement);
        LOG("%6x%c", chThread->Id);
    }
}