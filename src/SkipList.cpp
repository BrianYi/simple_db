#include "SkipList.h"
#include "public.h"
#include <string>

SkipList::SkipList(int index)
{
    fHead = new SLNode;
    fFileName = IndexFileDir + std::to_string(index) + IndexFileExt;
    fMaxlevel = 0;
}

SkipList::~SkipList()
{
    delete fHead;
}

void SkipList::InsertNode(int64_t val, string dataFileName, int64_t offset)
{
    SLNode *node = fHead;
    int level = RandomLevel();
    SLNode *newNode = new SLNode;
    newNode->data = val;
    newNode->maxlevel = level;
    newNode->indexFileName = fFileName;
    newNode->dataFileName = dataFileName;
    newNode->offset = offset;
    SLNode *updateNode[MAX_LEVEL];
    for (int i = level; i >= 0; --i)
    {
        while (node->next[i] != nullptr && node->next[i]->data < val)
            node = node->next[i];
        updateNode[i] = node;
    }    
    for (int i = 0; i <= level; ++i)
    {
        newNode->next[i] = updateNode[i]->next[i];
        updateNode[i]->next[i] = newNode;
    }
    if (fMaxlevel < level) fMaxlevel = level;
}

vector<SLNode*> SkipList::SearchNode(int64_t val)
{
    vector<SLNode*> nodes;
    SLNode *node = fHead;
    for (int i = fMaxlevel; i >= 0; --i)
    {
        while (node->next[i] != nullptr && node->next[i]->data < val)
            node = node->next[i];
    }
    while (node->next[0] != nullptr && node->next[0]->data == val)
    {
        nodes.push_back(node->next[0]);
        node = node->next[0];
    }
    return nodes;
}

int SkipList::DeleteNode(int64_t val)
{
    int deleteNodeNum = 0;
    SLNode *preNodes[MAX_LEVEL];
    SLNode *node = fHead;
    int level = 0;
    for (int i = fMaxlevel; i >= 0; --i)
    {
        while (node->next[i] != nullptr && node->next[i]->data < val)
            node = node->next[i];
        level = max(level, i);
        preNodes[i] = node;
    }
    SLNode *deleteNode = nullptr;
    for (int i = level; i >= 0; --i)
    {
        while (preNodes[i]->next[i] != nullptr && preNodes[i]->next[i]->data == val)
        {
            deleteNode = preNodes[i]->next[i];
            preNodes[i]->next[i] = deleteNode->next[i];
            if (i == 0) 
            {
                delete deleteNode;
                deleteNodeNum++;
            }
        }
    }
    return deleteNodeNum;
}

string SkipList::FileName()
{
    return fFileName;
}

void SkipList::WriteToFile()
{
    FILE *fp = fopen(fFileName.c_str(), "wb");
    int64_t numNode = 0;
    int64_t writtenNum = 0;
    fseek(fp, sizeof(int64_t), SEEK_SET);
    struct SLNode *node = fHead->next[0];
    struct RawSLNode rawNode;
    while (node)
    {
        // write to file
        rawNode.data = node->data;
        strcpy(rawNode.dataFileName, node->dataFileName.c_str());
        strcpy(rawNode.indexFileName, node->indexFileName.c_str());
        rawNode.offset = node->offset;
        writtenNum = fwrite((void *)&rawNode, sizeof(struct RawSLNode), 1, fp);
        assert(writtenNum == 1);
        
        // next node
        node=node->next[0];
        numNode++;
    }
    fseek(fp, 0, SEEK_SET);
    writtenNum = fwrite((void *)&numNode, sizeof(int64_t), 1, fp);
    assert(writtenNum == 1);
    if (fp != nullptr)
        fclose(fp);
}

void SkipList::ReadFromFile()
{
    FILE *fp = fopen(fFileName.c_str(), "rb");
    int64_t numNode = 0;
    size_t readNum = fread((void *)&numNode, sizeof(int64_t), 1, fp);
    assert(readNum == 1);
    struct RawSLNode* rawNodes = new RawSLNode[numNode];
    readNum = fread((void *)rawNodes, sizeof(struct RawSLNode), numNode, fp);
    assert(readNum == numNode);
    for (int i = 0; i < numNode; ++i)
        this->InsertNode(rawNodes[i].data, rawNodes[i].dataFileName, rawNodes[i].offset);
    delete [] rawNodes;
    fclose(fp);
}

int SkipList::RandomLevel()
{
    return rand() % MAX_LEVEL;
}
