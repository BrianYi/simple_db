#include "SimpleDB.h"
#include <algorithm>

SimpleDB::SimpleDB()
{
    fIndexEnginePtr = new IndexEngine;
    fDataEnginePtr = new DataEngine;   
}

SimpleDB::~SimpleDB()
{
    if (fIndexEnginePtr)
    {
        delete fIndexEnginePtr;
        fIndexEnginePtr = nullptr;
    }
    if (fDataEnginePtr)
    {
        delete fDataEnginePtr;
        fDataEnginePtr = nullptr;
    }
}

bool SimpleDB::Initialize(unsigned seed)
{
    srand(seed);
    if (!fIndexEnginePtr->Initialize())
        errExit("Initialize");
    if (!fDataEnginePtr->Initialize())
        errExit("Initialize");
    return true;
}

// insert one row data
void SimpleDB::Insert(RowData& rowdata)
{
    int attrnum = sizeof(rowdata.data) / sizeof(rowdata.data[0]);

    // insert data
    fDataEnginePtr->AppendWithoutBuffer(rowdata);

    // insert index
    for (int attrid = 0; attrid < attrnum; ++attrid)
        fIndexEnginePtr->InsertNode(attrid, rowdata.data[attrid], rowdata.dataFileName, rowdata.offset);
}

// delete
int64_t SimpleDB::DeleteRange(int attrid, int64_t low, int64_t up)
{
    low = min(low, up);
    up = max(low, up);
    int64_t numDeleted = 0;
    for (int64_t val = low; val != up; ++val)
        numDeleted += Delete(attrid, val);
    return numDeleted;
}

int64_t SimpleDB::Delete(int attrid, int64_t val)
{
    // delete from index
    return fIndexEnginePtr->DeleteNode(attrid, val);
}

// search
vector<RowData*> SimpleDB::SearchRange(int attrid, int64_t low, int64_t up)
{
    low = min(low, up);
    up = max(low, up);
    vector<RowData*> rowdataset;
    vector<RowData*> rowdatasets;
    for (int64_t val = low; val != up; ++val)
    {
        rowdataset = Search(attrid, val);
        if (!rowdataset.empty())
            rowdatasets.insert(rowdatasets.end(), rowdataset.begin(), rowdataset.end());
    }
    return rowdatasets;
}

vector<RowData*> SimpleDB::Search(int attrid, int64_t val)
{
    vector<RowData*> rowdatas;
    // search index
    vector<SLNode*> nodes = fIndexEnginePtr->SearchNode(attrid, val);
    if (nodes.empty()) return rowdatas;
    
    /* search data */
    // sort file name, then read data by file name
    sort(nodes.begin(), nodes.end(), [&](SLNode *n1, SLNode* n2){
            if (n1->dataFileName == n2->dataFileName)
                return n1->data < n2->data;
            return n1->dataFileName < n2->dataFileName;
            });
    string curDataFileName = "";
    FILE *fp = nullptr;
    for (auto node : nodes)
    {
        struct RowData* rowdata = new RowData;
        if (curDataFileName != node->dataFileName)
        {
            if (fp != nullptr)
            {
                fclose(fp);
                fp = nullptr;
            }
            curDataFileName = node->dataFileName;
            fp = fopen(curDataFileName.c_str(), "rb");
            if (fp == nullptr)
                errMsg("fopen '%s' failed", curDataFileName.c_str());
        }
        int64_t rowdataOffset = (node->offset / sizeof(rowdata->data)) * sizeof(rowdata->data);
        fseek(fp, rowdataOffset, SEEK_SET);
        uint32_t readNum = fread((void *)rowdata->data, sizeof(rowdata->data), 1, fp);
        assert(readNum == 1);
        //            rowdata->indexFileName = node->indexFileName;
        rowdata->dataFileName = curDataFileName;
        rowdata->offset = node->offset;
        rowdatas.push_back(rowdata);
    }
    if (fp != nullptr)
    {
        fclose(fp);
        fp = nullptr;
    }
    return rowdatas;
}

