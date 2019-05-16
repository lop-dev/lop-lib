//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/leafFactory.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_LEAFFACTORY_H__
#define __MWLIB_BEHAVIORTREE_LEAFFACTORY_H__


#include <MWLib/behaviorTree/handler.h>
#include <MWLib/behaviorTree/classLeafNode.h>


namespace MWLib
{
namespace BehaviorTree
{
class Handler;
class CClassLeafNode;

#define REGISTER_LEAF(name, call) do { std::string strName(name); MWLib::BehaviorTree::CBTLeafFactory::GetInstance()->Register(strName, call); } while(false)

/**
 * BTLeafFactory class. This class represents an associative array where
 * leaf classes pointers (ClassLeafNode) are associated with unique strings
 * Can return a pointer, taking the string as argument.
 * It implements the Singleton Pattern.
 */
class CBTLeafFactory
{
public:
    //!Callback for class creations
    typedef CClassLeafNode* (*CreateLeafCallback) (CBTNode* parent, Handler* a_owner);

    //!Entries in the leaf factory.
    std::map<std::string, CreateLeafCallback> m_Entries;

    //! Singleton access: Unique access point. Constructor is private.
    static CBTLeafFactory* GetInstance();

    //! Frees member variables. Destructor is private.
    static void Destroy();

    /**
     * Introduces a new entry in the function pointer register.
     * @param a_Name Name of the class creator. This name must be used in the node properties to
     * define which class should be executed to perform the node functionality.
     * @param a_creator Function pointer, that when called, creates the class that codifies the node functionality.
     */
    void Register(std::string& a_Name, CreateLeafCallback a_creator)
    {
        m_Entries[a_Name] = a_creator;
    }

    /**
     * Deletes one entry from the register of function pointers.
     * @param a_Name Name of the class creator to delete.
     */
    void UnRegister(std::string& a_Name)
    {
        m_Entries[a_Name] = 0;
    }

    //! Clears the register of function pointers.
    void UnRegisterAll()
    {
        m_Entries.clear();
    }

    /**
     * Checks the presence of one entry from the register of function pointers.
     * @param a_Name Name of the class creator to check.
     * @return true if the function pointer is stored in the register.
     */
    bool IsRegistered(std::string& a_Name)
    {
        if(m_Entries[a_Name] != 0)
        {
            return true;
        }

        return false;
    }

    /**
     * Queries for one entry from the register of function pointers.
     * @param a_Name Name of the class creator to get.
     * @return The function pointer requested. It does not check if the entry exists (use IsRegistered() for that).
     * @see IsRegistered().
     */
    CreateLeafCallback GetCreator(std::string& a_Name)
    {
        return m_Entries[a_Name];
    }

private:
    //! Pointer to the unique instance of this class.
    static CBTLeafFactory* m_instance;

    /**
     * Private constructor. To create the leaf factory, use GetInstance() instead.
     * @see GetInstance()
     */
    CBTLeafFactory()
    {
    }

    /**
     * Private destructor. To delete the leaf factory, use Destroy() instead.
     * @see Destroy()
     */
    ~CBTLeafFactory()
    {
    }

    /**
     * Include here the leaf class creation calls.
     */
    void _registerLeafNodes();
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_LEAFFACTORY_H__
