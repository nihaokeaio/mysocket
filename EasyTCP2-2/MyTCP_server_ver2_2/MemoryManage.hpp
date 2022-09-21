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

//�ڴ��
class MemoryBlock
{
public:
    //�ڴ����
    int nID;
    //��һ��λ��
    MemoryBlock *pNext;
    //���ô���
    int Ref;
    //�Ƿ����ڴ����
    bool bPool;
    //�����ڴ��
    MemoryAlloc *pAlloc;

private:
    //����
    char cNull[3];

public:
};

//�ڴ��
class MemoryAlloc
{
protected:
    //�ڴ��Ĵ�С
    size_t _nSize;
    //�ڴ濨������
    size_t _nBlock;
    //�ڴ�ص�ַ
    char *_pBuf;
    //ͷ���ڴ浥Ԫ
    MemoryBlock *_pHeader;
    //����
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
    //�����ڴ�
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
    //�ͷ��ڴ�
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
    //��ʼ��
    void initMemory()
    {
        xPrint("initMemory:_nSzie=%d,_nBlockSzie=%d\n", _nSize, _nBlock);
        //����
        assert(nullptr == _pBuf);
        if (_pBuf)
            return;
        //�����ڴ�صĴ�С
        size_t realSzie = _nSize + sizeof(MemoryBlock);
        size_t bufSize = (realSzie) * _nBlock;
        //��ϵͳ������ڴ�
        _pBuf = (char *)malloc(bufSize);
        //��ʼ��
        _pHeader = (MemoryBlock *)_pBuf;
        _pHeader->bPool = true;
        _pHeader->nID = 0;
        _pHeader->Ref = 0;
        _pHeader->pAlloc = this;
        _pHeader->pNext = nullptr;
        MemoryBlock *pTemp1 = _pHeader;
        //�����ڴ����г�ʼ��
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

//�����ʼ�� 
template <size_t nSize, size_t nBlock>
class MemoryAlloctor : public MemoryAlloc
{
public:
    MemoryAlloctor()
    {
        //�����ڴ����
        const size_t n = sizeof(void *);
        _nSize = (nSize / n) * n + (nSize % n != 0 ? n : 0);
        _nBlock = nBlock;
    }
};

//�ڴ����Ԫ
class MemoryMgr
{
private:
    //��������ģʽ
    MemoryMgr()
    {
        //����0-64�ֽڴ�С���ڴ��
        init_szAlloc(0, 64, &_mem64);
        //����65-128�ֽڴ�С���ڴ��
        init_szAlloc(65, 128, &_mem128);
        /*init_szAlloc(129, 256, &_mem256);
        init_szAlloc(257, 512, &_mem512);
        init_szAlloc(513, 1024, &_mem1024);*/
        xPrint("MemoryMgr\n");
    };
    ~MemoryMgr() {}
    //����400_0000 ��64�ֽڴ�С���ڴ��
    MemoryAlloctor<64, 4000000> _mem64;
    //����200_0000 ��128�ֽڴ�С���ڴ��
    MemoryAlloctor<128, 2000000> _mem128;
    /*MemoryAlloctor<256, 100000> _mem256;
    MemoryAlloctor<512, 100000> _mem512;
    MemoryAlloctor<1024, 100000> _mem1024;*/
    MemoryAlloc *_szAlloc[MAX_MEMORY_SIZE + 1];
    std::mutex _mutex;

private:
    //��ʼ���ڴ��ӳ������
    void init_szAlloc(int nbegin, int nend, MemoryAlloc *pMemA)
    {
        for (int i = nbegin; i <= nend; i++)
        {
            _szAlloc[i] = pMemA;
        }
    }

public:
    //�����ڴ�
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
    //�ͷ��ڴ�
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
