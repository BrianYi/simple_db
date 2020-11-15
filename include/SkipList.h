#include "public.h"
#include <bits/stdint-intn.h>
#include <strings.h>

struct RawSLNode
{
    int64_t data;
    char indexFileName[64];
    char dataFileName[64];
    int64_t offset;
};

struct SLNode
{
    SLNode()
    {
        memset(next, 0, sizeof(struct SLNode *) * MAX_LEVEL);
    }
    ~SLNode()
    {
    }
    int64_t data;
    string indexFileName;
    string dataFileName;
    int64_t offset;
    int maxlevel;
    struct SLNode *next[MAX_LEVEL];
};

class SkipList
{
    public:
        SkipList(int index);
        ~SkipList();
        void InsertNode(int64_t val, string dataFileName, int64_t offset);
        vector<SLNode*> SearchNode(int64_t val);
        int DeleteNode(int64_t val);
        string FileName();
        void WriteToFile();
        void ReadFromFile();
    private:
        int RandomLevel();
    private:
        SLNode *fHead;
        string fFileName;
        int fMaxlevel;
};
