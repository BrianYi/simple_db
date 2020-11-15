#pragma once
#include <cstdlib>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <assert.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ename.h"
using namespace std;

const string IndexFileDir = "index/";
const string DataFileDir = "data/";
const string IndexFileExt = ".index";
const string DataFileExt = ".data";

const int MAX_ATTR_NUM = 100;
const int MAX_INDEX_NUM = MAX_ATTR_NUM;
const int DATA_FILE_MAX_ROW_NUM = 10000;    // 数据文件最多存放10000条数据
const int MAX_LEVEL = 16;
//const int DATA_ENGINE_BUF_ROW_NUM = 10000; // 10000条记录写入一次文本
//const int MAX_ROWS_NUM = 10;
//typedef int64_t RowData[MAX_ATTR_NUM];

struct RowData
{
    public:
        int64_t data[MAX_ATTR_NUM];
//    public:
//        string IndexFileName() { return indexFileName; }
//        string DataFileName() { return dataFileName; }
//        int64_t Offset() { return offset; }
//        int64_t *Data() { return data;  }
//    private:
//        string indexFileName;   // 传出来的
        string dataFileName;    // 传出来的
        int64_t offset;         // 传出来的
};


inline uint64_t get_timestamp_ms()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


inline static void
outputError(bool useErr, int err, bool flushStdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");
    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);
#if __GNUC__ >= 7
#pragma GCC diagnostic pop
#endif

    if (flushStdout)
        fflush(stdout);       /* Flush any pending stdout */
    fputs(buf, stderr);
    fflush(stderr);           /* In case stderr is not line-buffered */
}

inline void errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno;

    savedErrno = errno;       /* In case we change it here */

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    errno = savedErrno;
}

inline void errExit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    exit(EXIT_FAILURE);
//    terminate(true);
}
