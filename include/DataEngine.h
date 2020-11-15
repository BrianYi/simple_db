#include "public.h"

class DataEngine
{
    public:
        DataEngine();
        ~DataEngine();
        bool Initialize();
        // insert one row data
        void Append(RowData& rowdata);
        void AppendWithoutBuffer(RowData& rowdata);
//        // delete
//        void Delete(int attrid, int64_t val);
//        void DeleteRange(int attrid, int64_t low, int64_t up);
    protected:
        void ScanDataFiles();
    private:
        string fDataFileName;
        FILE *fDataFilePtr;
        vector<RowData*> fDataBuffer;
};
