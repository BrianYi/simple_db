#include "public.h"

class Test
{
    public:
        ~Test();
        static Test* GetInstance();
        static void Initialize(unsigned seed = 1234);
        // test simple db
        static vector<int> GenerateValues(int64_t num, int64_t low, int64_t up);
    private:
        Test();
};
