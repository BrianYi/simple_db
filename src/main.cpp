#include "SimpleDB.h"
#include "public.h"
#include <stdlib.h>

vector<struct RowData> RandomRowData(size_t num, int64_t low, int64_t up)
{
    vector<struct RowData> rowdatas;
    low = min(low, up);
    up = max(low, up);
    int64_t length = up - low;
    struct RowData rowdata;
    for (size_t i = 0; i < num; ++i)
    {
        for (size_t j = 0; j < MAX_ATTR_NUM; ++j)
            rowdata.data[j] = low + rand() % length;
        rowdatas.push_back(rowdata);
    }
    return rowdatas;
}

int main()
{
    SimpleDB simpledb;
    simpledb.Initialize(1234);
    vector<struct RowData> rowdatas = RandomRowData(10001, 0, 10);
    for (size_t i = 0; i < rowdatas.size(); ++i)
    {
        simpledb.Insert(rowdatas[i]);
    }
    vector<RowData*> rowdatasets = simpledb.SearchRange(0, 0, 10);
    printf("%-2s | %-5s", "id", "value");
    for (size_t i = 0; i < MAX_ATTR_NUM; ++i)
        printf("%-5s ", "value");
    printf(" %-30s%-5s\r\n","DataFile", "Offset");
    for (size_t i = 0; i < rowdatasets.size(); ++i)
    {
        printf("%02lu | ", i);
        for (size_t j = 0; j < MAX_ATTR_NUM; ++j)
            printf("%-5ld ", rowdatasets[i]->data[j]);
        printf("%-30s%-5ld", rowdatasets[i]->dataFileName.c_str(), rowdatasets[i]->offset);
        printf("\r\n");
        delete rowdatasets[i];
    }
//    exit(EXIT_SUCCESS);
}
