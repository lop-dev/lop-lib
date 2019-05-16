//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/hashMap/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string>
//#include "main.h"
#include <BCLib/utility/hashMap.h>


// testHashMap::testHashMap()
// {
//     m_map = new BCLib::Utility::CHashMap<int, char>();
// }
struct SData
{
    int m_ID;
    std::string m_name;
    int m_score;

    SData(int id = 0, const std::string& name = "", int score = 0)
    :m_ID(id)
    ,m_name(name)
    ,m_score(score)
    {
    }
    SData(const SData& data)
    :m_ID(data.m_ID)
    ,m_name(data.m_name)
    ,m_score(data.m_score)
    {
    }

    ~SData()
    {
        printf("释放 SData id[%d]\n", m_ID);
    }
};

bool dataScoreDec(const int& id, SData*& pData, int num)
{
    if(pData == NULL)
    {
        return true;
    }
    pData->m_score -= num;
    if(pData->m_score < 0)
    {
        pData->m_score = 0;
    }
    return true;
}

class CData
{
public:
    CData()
    {
    }
    ~CData()
    {
        clear();
    }

    bool setData(int id, const std::string& name, int score)
    {
        SData* pData = new SData(id, name, score);
        if(pData == NULL)
        {
            return false;
        }
        m_dataList.setValue(pData->m_ID, pData);
        return true;
    }

    bool setData(SData& data)
    {
        SData* pData = new SData(data);
        if(pData == NULL)
        {
            return false;
        }
        m_dataList.setValue(pData->m_ID, pData);
        return true;
    }

    bool getData(int id, std::string& name, int& score)
    {
        SData* pData = m_dataList.getValue(id);
        if(pData == NULL)
        {
            return false;
        }
        name = pData->m_name;
        score = pData->m_score;
        return true;
    }

    bool getData(int id, SData& data)
    {
        SData* pData = m_dataList.getValue(id);
        if(pData == NULL)
        {
            return false;
        }
        data = *pData;
        return true;
    }

    void remove(int id)
    {
        SData* pData = m_dataList.getValue(id);
        if(pData == NULL)
        {
            return ;
        }
        m_dataList.remove(id);
        BCLIB_SAFE_DELETE(pData);
    }

    void showAll()
    {
        class CShowAllCallback : public BCLib::Utility::CHashMap<int, SData*>::CCallback
        {
        public:
            virtual bool exec(const int& id, SData*& data)
            {
                if(data == NULL)
                {
                    return true;
                }
                printf("id[%d] name[%s] score[%d]\n", data->m_ID, data->m_name.c_str(), data->m_score);
                return true;
            }
        };

        CShowAllCallback cb;
        m_dataList.traversal(cb);
    }

    void scoreAdd(int num)
    {
        BCLib::Utility::CFunctionObject<bool, CData, const int&, SData*&, int> fun(&CData::_scoreAdd, this);
        m_dataList.traversal(fun, num);
    }

    void scoreDec(int num)
    {
        BCLib::Utility::CFunction<bool, const int&, SData*&, int> fun(dataScoreDec);
        m_dataList.traversal(fun, num);
    }

    void clear()
    {
        BCLib::Utility::CHashMap<int, SData*>::iterator it = m_dataList.begin();
        for(; it != m_dataList.end(); ++it)
        {
            SData* data = it->second;
            if(data != NULL)
            {
                BCLIB_SAFE_DELETE(data);
            }
        }
        m_dataList.clear();
    }

private:
    bool _scoreAdd(const int& id, SData*& pData, int num)
    {
        if(pData == NULL)
        {
            return true;
        }
        pData->m_score += num;
        return true;
    }

private:
    BCLib::Utility::CHashMap<int, SData*> m_dataList;
};


int main(int argc, char* argv[])
{
    CData testData;
    testData.setData(1, "张三", 110);
    testData.setData(2, "李四", 120);
    testData.setData(3, "王五", 130);
    printf("-------------------------------------\n");
    testData.showAll();

    printf("-------------------------------------\n");
    testData.scoreAdd(50);
    testData.showAll();
    printf("-------------------------------------\n");
    testData.scoreDec(170);
    testData.showAll();
    printf("-------------------------------------\n");
    testData.remove(2);
    testData.showAll();
    printf("-------------------------------------\n");
    testData.clear();
    printf("-------------------------------------\n");
}