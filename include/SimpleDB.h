#include "public.h"
#include "IndexEngine.h"
#include "DataEngine.h"

//struct RowData
//{
//    RowData()
//    { memset((void *)data, 0, sizeof(data)); }
//    int64_t data[MAX_ATTR_NUM];
//};

class SimpleDB
{
    public:
        SimpleDB();
        ~SimpleDB();
        bool Initialize(unsigned int seed);
        // insert one row data
        void Insert(RowData& rowdata);
        // delete
        // just delete index
        int64_t DeleteRange(int attrid, int64_t low, int64_t up);
        int64_t Delete(int attrid, int64_t val);
        // search
        vector<RowData*> SearchRange(int attrid, int64_t low, int64_t up);
        vector<RowData*> Search(int attrid, int64_t val);
    private:
        IndexEngine *fIndexEnginePtr;
        DataEngine *fDataEnginePtr;
};
