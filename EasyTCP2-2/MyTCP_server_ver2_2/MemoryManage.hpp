#ifndef _MemoryMgr_H_
#define _MemoryMgr_H_
#include <stdlib.h>
#include <assert.h>
#include<mutex>




#define MAX_MEMORY_SIZE 128
class MemoryAlloc;
class MemoryBlock;
template <size_t nSize, size_t nBlock>
class MemoryAlloctor;
class MemoryMgr;
//  class MemoryAlloctor;
//
#ifdef _DEBUG
#include <stdio.h>
    #define xPrint(...) printf(__VA_ARGS__)
#else
    #define xPrint(...)
#endif // DEBUG

//内存块
class MemoryBlock
{
public:
    //内存块编号
    int nID;
    //下一个位置
    MemoryBlock *pNext;
    //引用次数
    int Ref;
    //是否在内存池中
    bool bPool;
    //所属内存块
    MemoryAlloc *pAlloc;

private:
    //保留
    char cNull[3];

public:
};

//内存池
class MemoryAlloc
{
protected:
    //内存块的大小
    size_t _nSize;
    //内存卡的数量
    size_t _nBlock;
    //内存池地址
    char *_pBuf;
    //头部内存单元
    MemoryBlock *_pHeader;
    //加锁
    std::mutex _mutex;

public:
    MemoryAlloc()
    {
        _nSize = 0;
        _nBlock = 0;
        _pBuf = nullptr;
        _pHeader = nullptr;
        xPrint("MemoryAlloc\n");
    }
    ~MemoryAlloc()
    {
        if (!_pBuf)
        {
            free(_pBuf);
        }
    }
    //申请内存
    void *allocMemory(size_t nSize)
    {
        std::lock_guard<std::mutex> lg(_mutex);
        if (_pBuf == nullptr)
        {
            initMemory();
        }
        MemoryBlock *pReturn = nullptr;
        if (nullptr == _pHeader)
        {
            pReturn = (MemoryBlock *)malloc(nSize + sizeof(MemoryBlock));  
            pReturn->bPool = false;
            pReturn->nID = -1;
            pReturn->Ref = 1;
            pReturn->pAlloc = nullptr;
            pReturn->pNext = nullptr;
        }
        else
        {
            pReturn = _pHeader;
            _pHeader = _pHeader->pNext;
            assert(0 == pReturn->Ref);
            pReturn->Ref = 1;
        }
        //xPrint("allocMem: %llx, id= %d,size=%d \n", pReturn, pReturn->nID, nSize);
        //printf("allocMem: %llx, id= %d, size=%d \n", pReturn, pReturn->nID, nSize);
        return (char *)pReturn + sizeof(MemoryBlock);
    }
    //释放内存
    void freeMemory(void *pMem)
    {
        MemoryBlock *pBlock = (MemoryBlock *)((char *)pMem - sizeof(MemoryBlock));
        // xPrint("freeMem: %llx, id=%d\n", pBlock, pBlock->nID);
        assert(1 == pBlock->Ref);       
        if (pBlock->bPool)
        {
            std::lock_guard<std::mutex> lg(_mutex);
            if (--pBlock->Ref != 0)
            {
                return;
            }
            pBlock->pNext = _pHeader;
            _pHeader = pBlock;
        }
        else
        {
            if (--pBlock->Ref != 0)
            {
                return;
            }
            free(pBlock);
            return;
        }
    }
    //初始化
    void initMemory()
    {
        xPrint("initMemory:_nSzie=%d,_nBlockSzie=%d\n", _nSize, _nBlock);
        //断言
        assert(nullptr == _pBuf);
        if (_pBuf)
            return;
        //计算内存池的大小
        size_t realSzie = _nSize + sizeof(MemoryBlock);
        size_t bufSize = (realSzie) * _nBlock;
        //向系统申请池内存
        _pBuf = (char *)malloc(bufSize);
        //初始化
        _pHeader = (MemoryBlock *)_pBuf;
        _pHeader->bPool = true;
        _pHeader->nID = 0;
        _pHeader->Ref = 0;
        _pHeader->pAlloc = this;
        _pHeader->pNext = nullptr;
        MemoryBlock *pTemp1 = _pHeader;
        //遍历内存块进行初始化
        for (size_t n = 1; n < _nBlock; n++)
        {
            MemoryBlock *pTemp2 = (MemoryBlock *)(_pBuf + (n * realSzie));
            pTemp2->bPool = true;
            pTemp2->nID = n;
            pTemp2->Ref = 0;
            pTemp2->pAlloc = this;
            pTemp2->pNext = nullptr;
            pTemp1->pNext = pTemp2;
            pTemp1 = pTemp2;
        }
    }
};

//方便初始化 
template <size_t nSize, size_t nBlock>
class MemoryAlloctor : public MemoryAlloc
{
public:
    MemoryAlloctor()
    {
        //保持内存对齐
        const size_t n = sizeof(void *);
        _nSize = (nSize / n) * n + (nSize % n != 0 ? n : 0);
        _nBlock = nBlock;
    }
};

//内存管理单元
class MemoryMgr
{
private:
    //创建单例模式
    MemoryMgr()
    {
        //管理0-64字节大小的内存块
        init_szAlloc(0, 64, &_mem64);
        //管理65-128字节大小的内存块
        init_szAlloc(65, 128, &_mem128);
        /*init_szAlloc(129, 256, &_mem256);
        init_szAlloc(257, 512, &_mem512);
        init_szAlloc(513, 1024, &_mem1024);*/
        xPrint("MemoryMgr\n");
    };
    ~MemoryMgr() {}
    //申请400_0000 块64字节大小的内存块
    MemoryAlloctor<64, 4000000> _mem64;
    //申请200_0000 块128字节大小的内存块
    MemoryAlloctor<128, 2000000> _mem128;
    /*MemoryAlloctor<256, 100000> _mem256;
    MemoryAlloctor<512, 100000> _mem512;
    MemoryAlloctor<1024, 100000> _mem1024;*/
    MemoryAlloc *_szAlloc[MAX_MEMORY_SIZE + 1];
    std::mutex _mutex;

private:
    //初始化内存池映射数组
    void init_szAlloc(int nbegin, int nend, MemoryAlloc *pMemA)
    {
        for (int i = nbegin; i <= nend; i++)
        {
            _szAlloc[i] = pMemA;
        }
    }

public:
    //申请内存
    void *allocMem(size_t nSize)
    {
        if (nSize >0&&nSize <= MAX_MEMORY_SIZE)
        {
            return _szAlloc[nSize]->allocMemory(nSize);
        }
        else
        {
            std::lock_guard<std::mutex> lg(_mutex);
            MemoryBlock *pReturn = (MemoryBlock *)malloc(nSize + sizeof(MemoryBlock));
            if (nullptr != pReturn)
            {
                pReturn->bPool = false;
                pReturn->nID = -1;
                pReturn->Ref = 1;
                pReturn->pAlloc = nullptr;
                pReturn->pNext = nullptr;
                //xPrint("Not in allocMem: %llx, id=%d,size=%d \n", pReturn, pReturn->nID, nSize);
            }

            return (char *)(pReturn) + sizeof(MemoryBlock);
        }
    }
    //释放内存
    void freeMem(void *pMem)
    {
        MemoryBlock *pBlock = (MemoryBlock *)((char *)pMem - sizeof(MemoryBlock));
        //xPrint("freeMem: %llx, id=%d\n", pBlock, pBlock->nID);
        if (pBlock->bPool)
        {
            pBlock->pAlloc->freeMemory(pMem);
        }
        else
        {
            if (--pBlock->Ref == 0)
            {
                free(pBlock);
            }
        }
        
    }
    void addRef(void *pMem)
    {
        MemoryBlock *pBlock = (MemoryBlock *)((char *)pMem - sizeof(MemoryBlock));
        ++pBlock->Ref;
    }
    static MemoryMgr &Instance()
    {
        static MemoryMgr mgr;
        return mgr;
    }
};

#endif // !_MemoryMgr_H_
