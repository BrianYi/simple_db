#include "public.h"
#include "SkipList.h"

class IndexEngine
{
    public:
        IndexEngine();
        ~IndexEngine();
        bool Initialize();
        vector<SLNode*> SearchNode(int index, int64_t val);
        void InsertNode(int index, int64_t val, string dataFileName, int64_t offset);
        int64_t DeleteNode(int index, int64_t val);
    protected:
        void ScanIndexFiles();
        void SyncToIndexFiles();
    private:
        void BuildSkipList();
    private:
        SkipList* fSList[MAX_INDEX_NUM];
};
