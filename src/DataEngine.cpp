#include "DataEngine.h"
#include "public.h"
#include <dirent.h>

DataEngine::DataEngine()
{
    fDataFilePtr = nullptr;
}

DataEngine::~DataEngine()
{
    if (fDataFilePtr != nullptr)
    {
        fclose(fDataFilePtr);
        fDataFilePtr = nullptr;
    }
}

bool DataEngine::Initialize()
{
    //
    ScanDataFiles();

    return true;
}

void DataEngine::Append(RowData& rowdata)
{
}

void DataEngine::AppendWithoutBuffer(RowData& rowdata)
{
    if (fDataFilePtr == nullptr)
        fDataFilePtr = fopen(fDataFileName.c_str(), "ab+");
    assert(fDataFilePtr != nullptr);
    fseek(fDataFilePtr, 0, SEEK_END);
    int64_t offset = ftell(fDataFilePtr);
    if (offset >= sizeof(rowdata.data) * DATA_FILE_MAX_ROW_NUM)
    {
        if (fDataFilePtr != nullptr)
            fclose(fDataFilePtr);
        int64_t curTime = get_timestamp_ms();
        string dataFileName = DataFileDir + to_string(curTime) + DataFileExt;
        fDataFileName = dataFileName;
        fDataFilePtr = fopen(fDataFileName.c_str(), "ab+");
        offset = 0;
    }
    int64_t writtenNum = fwrite((void *)rowdata.data, sizeof(rowdata.data[0]), MAX_ATTR_NUM, fDataFilePtr);
    assert(writtenNum == MAX_ATTR_NUM);
    rowdata.dataFileName = fDataFileName;
    rowdata.offset = offset;
    if (fDataFilePtr != nullptr)
    {
        fclose(fDataFilePtr);
        fDataFilePtr = nullptr;
    }
}

int filter(const struct dirent *dirp)
{
    string dataFileName = string(dirp->d_name);
    if (dataFileName == "." || dataFileName == "..")
        return 0;
    string dataFileExt = dataFileName.substr(dataFileName.size() - DataFileExt.size());
    if (dataFileExt == DataFileExt)
        return 1;
    return 0;
}

void DataEngine::ScanDataFiles()
{
    struct dirent **namelist;
    string dataFileName = "";
    if (access(DataFileDir.c_str(), F_OK) != 0 && mkdir(DataFileDir.c_str(), 0776) != 0)
        errExit("ScanDataFiles mkdir failed");

    int n = scandir(DataFileDir.c_str(), &namelist, filter, alphasort);
    if (n == -1)
    {
        errExit("scandir");
    } else if (n == 0) {
        int64_t curTime = get_timestamp_ms();
        dataFileName = DataFileDir + to_string(curTime) + DataFileExt;
    } else {
        dataFileName = DataFileDir + string(namelist[n-1]->d_name);
        free(namelist);
    }
    fDataFileName = dataFileName;
    fDataFilePtr = fopen(fDataFileName.c_str(), "ab+");
}
