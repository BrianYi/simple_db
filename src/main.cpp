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

void TestInsert(SimpleDB& simpledb, size_t num, int64_t low, int64_t up)
{
    printf("================ TestInsert ================\r\n");
    vector<struct RowData> rowdatas = RandomRowData(num, low, up);
    for (size_t i = 0; i < rowdatas.size(); ++i)
    {
        simpledb.Insert(rowdatas[i]);
    }
    printf("Inserted %ld records.\r\n", rowdatas.size());
    printf("\r\n");
}

void TestSearch(SimpleDB& simpledb, int attrid, int64_t low, int64_t up)
{
    printf("================ TestSearch ================\r\n");
    vector<RowData*> rowdatasets = simpledb.SearchRange(0, 500, 600);
    printf("%-2s | ", "id");
    for (size_t i = 0; i < MAX_ATTR_NUM; ++i)
        printf("%-5s", "val");
    printf("%-30s%-5s\r\n","DataFile", "Offset");
    for (size_t i = 0; i < rowdatasets.size(); ++i)
    {
        printf("%02lu | ", i);
        for (size_t j = 0; j < MAX_ATTR_NUM; ++j)
            printf("%-5ld", rowdatasets[i]->data[j]);
        printf("%-30s%-5ld", rowdatasets[i]->dataFileName.c_str(), rowdatasets[i]->offset);
        printf("\r\n");
        delete rowdatasets[i];
    }
    printf("Found %ld records.\r\n", rowdatasets.size());
    printf("\r\n");
}

void TestDelete(SimpleDB& simpledb, int attrid, int64_t low, int64_t up)
{
    printf("================ TestDelete ================\r\n");
    int64_t numDeleted = simpledb.DeleteRange(attrid, low, up);
    printf("Deleted %ld records.\r\n", numDeleted);
    printf("\r\n");
}

int main()
{
    SimpleDB simpledb;
    simpledb.Initialize(1234);
    TestInsert(simpledb, 10000, 0, 1000);
    TestSearch(simpledb, 0, 500, 600);
    TestDelete(simpledb, 0, 500, 600);
}
