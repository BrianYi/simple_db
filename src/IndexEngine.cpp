#include "IndexEngine.h"
#include "public.h"

IndexEngine::IndexEngine()
{
    memset((void *)fSList, 0, sizeof(SkipList *) * MAX_INDEX_NUM);
}

IndexEngine::~IndexEngine()
{
    SyncToIndexFiles();

    for (int i = 0; i < MAX_INDEX_NUM; ++i)
        delete fSList[i];
}

bool IndexEngine::Initialize()
{
    //
    ScanIndexFiles();

    return true;
}

vector<SLNode*> IndexEngine::SearchNode(int index, int64_t val)
{
    return fSList[index]->SearchNode(val);
}

void IndexEngine::InsertNode(int index, int64_t val, string dataFileName, int64_t offset)
{
    if (fSList[index] == nullptr)
        fSList[index] = new SkipList(index);
    fSList[index]->InsertNode(val, dataFileName, offset);
}

int64_t IndexEngine::DeleteNode(int index, int64_t val)
{
    return fSList[index]->DeleteNode(val);
}

void IndexEngine::ScanIndexFiles()
{
    // scan index files
    DIR *dirp;
    struct dirent *dp;
    if (access(IndexFileDir.c_str(), F_OK) != 0 && mkdir(IndexFileDir.c_str(), 0776) != 0)
        errExit("ScanIndexFiles mkdir failed");
    dirp = opendir(IndexFileDir.c_str());
    if (dirp == nullptr)
    {
        errMsg("opendir failed on '%s'", IndexFileDir.c_str());
        return;
    }

    for (;;)
    {
        dp = readdir(dirp);
        if (dp == nullptr)
            break;
        string indexFileName = dp->d_name;
        if (indexFileName == "." || indexFileName == "..")
            continue;
        string indexFileExt = indexFileName.substr(indexFileName.size() - IndexFileExt.size());
        if (indexFileExt == IndexFileExt)
        {
            int index = stoi(indexFileName.substr(0, indexFileName.size() - indexFileExt.size()));
            if (fSList[index] == nullptr)
                fSList[index] = new SkipList(index);
            fSList[index]->ReadFromFile();
        }
    }

    if (closedir(dirp) == -1)
        errMsg("closedir");
}

void IndexEngine::SyncToIndexFiles()
{
    for (int i = 0; i < MAX_INDEX_NUM; ++i)
    {
        if (fSList[i])
            fSList[i]->WriteToFile();
    }
}


