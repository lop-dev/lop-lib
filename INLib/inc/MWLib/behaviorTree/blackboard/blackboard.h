//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/blackboard/blackboard.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_BLACKBOARD_BLACKBOARD_H__
#define __MWLIB_BEHAVIORTREE_BLACKBOARD_BLACKBOARD_H__
/** @file */

#include <BCLib/utility/baseDef.h>

namespace MWLib
{
namespace BehaviorTree
{
/*! Hashtable of <K,V>:<String,Int>. */
typedef std::map<std::string, int> IntBlackboard;

/*! Hashtable of <K,V>:<String,String>. */
typedef std::map<std::string, std::string> StringBlackboard;

/**
 * Blackboard class.
 * This is a basic implementation of a blackboard, that allows to store int and string values.
 * New blackboards with more functionality are suggested to be implemented, inheriting from this class.
 */
class CBlackboard
{
protected:
    //! Blackboard of ints.
    IntBlackboard m_intBlackboard;

    //! Blackboard of Strings
    StringBlackboard m_strBlackboard;

public:
    //! Constructor initializes member variables.
    CBlackboard();

    //! Destructor frees member variables.
    virtual ~CBlackboard();

    //! This function clears the blackboards of this class.
    virtual void clean();


    // GETTERS //
    /**
     * Returns the int value from the int blackboard. Returns -1 if the key supplied is not found.
     * @param a_key String key to search into the int blackboard.
     * @return The int value in the blackboard
     */
    int getValueInt(std::string a_key);

    /**
     * Returns the string value from the string blackboard. Returns an empty string if the key supplied is not found.
     * @param a_key String key to search into the string blackboard.
     * @return The string value in the blackboard
     */
    std::string getValueStr(std::string a_key);

    // SETTERS //

    /**
     * Inserts a new pair <K,V>=<String,Int> into the int blackboard.
     * @param a_key String key of the pair.
     * @param a_value Value of the pair.
     */
    void setValueInt(std::string a_key, int a_value);

    /**
     * Inserts a new pair <K,V>=<String,String> into the string blackboard.
     * @param a_key String key of the pair.
     * @param a_value Value of the pair.
     */
    void setValueStr(std::string a_key, std::string a_value);
};
}//BehaviorTree
}//MWLib
#endif//__MWLIB_BEHAVIORTREE_BLACKBOARD_BLACKBOARD_H__
