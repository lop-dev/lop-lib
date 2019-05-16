//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/behaviorTree/handler.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_BEHAVIORTREE_HANDLER_H__
#define __MWLIB_BEHAVIORTREE_HANDLER_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/assert.h>

namespace MWLib
{
namespace BehaviorTree
{
/** @file */

/**
 * Handler class.
 * This class encapsulates an identifier of a given type. Handler class
 * is virtual, so one of the derivated classes must be used.
 */
class Handler
{
public:

    //!Constructor initializes member variables.
    Handler()
    {
    }

    /**
     * This function compares two handlers. Returns -1 if 'this' is first,
     * 0 if they are equals and 1 if 'this' is last.
     * @param a_other Handler to compare with.
     * @return -1 if 'this' is first, 0 if they are equals and 1 if 'this' is last.
     */
    virtual int compareTo(Handler* a_other)
    {
        return 0;
    }

    /**
     * This function creates a copy of this handler. It returns a new handler
     * with the same identifier.
     * @return An instance copy of this handler.
     */
    virtual Handler* getCopy()
    {
        return 0;
    }

    /**
     * This function returns the INT value of the identifier. If the identifier
     * type is not compatible, it will return 0. This function must be overriden
     * in the derivated class if the type is compatible.
     * @return The int value.
     */
    virtual int getValueInt() const
    {
        return 0;
    }

    /**
     * This function returns the UNSIGNED INT value of the identifier. If the identifier
     * type is not compatible, it will return 0. This function must be overriden
     * in the derivated class if the type is compatible.
     * @return The unsigned int value.
     */
    virtual unsigned int getValueUInt() const
    {
        return 0;
    }

    /**
     * This function returns the STRING value of the identifier. If the identifier
     * type is not compatible, it will return an empty string. This function must
     * be overriden in the derivated class if the type is compatible.
     * @return The string value.
     */
    virtual std::string getValueString() const
    {
        return "";
    }

    virtual void* getValuePoint() const
    {
        return NULL;
    }
};


/**
 * IntHandler class.
 * This class inherits from Handler, implementing an int identifier.
 */
class IntHandler : public Handler
{
protected:

    //! Int identifier of this handler.
    int m_id;

public:

    //!Constructor initializes member variables.
    IntHandler()
    {
        m_id = 0;
    }

    /**
     * Constructor initializes member variables..
     * @param id identifier value
     */
    IntHandler(int id)
    {
        m_id = id;
    }

    /**
     * Constructor initializes member variables..
     * @param a_other handler to copy its identifier from.
     */
    IntHandler(Handler a_other)
    {
        m_id = a_other.getValueInt();
    }

    /**
     * Override of the equal operator.
     * @param a_other reference Handler to equal to.
     */
    void operator=(const IntHandler& a_other)
    {
        this->m_id = a_other.getValueInt();
    }

    /**
     * Override of the equal operator.
     * @param a_other pointer Handler to equal to.
     */
    void operator=(IntHandler* a_other)
    {
        BCLIB_ASSERT(a_other != 0);
        m_id = a_other->getValueInt();
    }

    /**
     * This function creates a copy of this handler. It returns a new handler
     * with the same identifier.
     * @return An instance copy of this handler.
     */
    Handler* getCopy();

    /**
     * This function compares two handlers. Returns -1 if 'this' is first,
     * 0 if they are equals and 1 if 'this' is last.
     * @param a_other Handler to compare with.
     * @return -1 if 'this' is first, 0 if they are equals and 1 if 'this' is last.
     */
    int compareTo(Handler* a_other);

    /**
     * This function returns the INT value of the identifier.
     * @return The int value.
     */
    int getValueInt() const
    {
        return m_id;
    }
};


/**
 * UIntHandler class.
 * This class inherits from Handler, implementing an unsigned int identifier.
 */
class UIntHandler : public Handler
{
protected:

    //! Unsigned int identifier of this handler.
    unsigned int m_id;

public:

    //!Constructor initializes member variables.
    UIntHandler()
    {
        m_id = 0;
    }

    /**
     * Constructor initializes member variables..
     * @param id identifier value
     */
    UIntHandler(unsigned int id)
    {
        m_id = id;
    }

    /**
     * Constructor initializes member variables..
     * @param a_other handler to copy its identifier from.
     */
    UIntHandler(Handler a_other)
    {
        m_id = a_other.getValueUInt();
    }

    /**
     * Override of the equal operator.
     * @param a_other reference Handler to equal to.
     */
    void operator=(const IntHandler& a_other)
    {
        this->m_id = a_other.getValueUInt();
    }

    /**
     * Override of the equal operator.
     * @param a_other pointer Handler to equal to.
     */
    void operator=(IntHandler* a_other)
    {
        BCLIB_ASSERT(a_other);
        m_id = a_other->getValueUInt();
    }

    /**
     * This function creates a copy of this handler. It returns a new handler
     * with the same identifier.
     * @return An instance copy of this handler.
     */
    Handler* getCopy();

    /**
     * This function compares two handlers. Returns -1 if 'this' is first,
     * 0 if they are equals and 1 if 'this' is last.
     * @param a_other Handler to compare with.
     * @return -1 if 'this' is first, 0 if they are equals and 1 if 'this' is last.
     */
    int compareTo(Handler* a_other);

    /**
     * This function returns the UNSIGNED INT value of the identifier.
     * @return The unsigned int value.
     */
    unsigned int getValueUInt() const
    {
        return m_id;
    }
};



/**
 * StringHandler class.
 * This class inherits from Handler, implementing a string identifier.
 */
class StringHandler : public Handler
{
protected:

    //! String identifier of this handler.
    std::string m_id;

public:

    //!Constructor initializes member variables.
    StringHandler()
    {
        m_id = "";
    }

    /**
     * Constructor initializes member variables..
     * @param id identifier value
     */
    StringHandler(std::string id)
    {
        m_id = id;
    }

    /**
     * Constructor initializes member variables..
     * @param a_other handler to copy its identifier from.
     */
    StringHandler(Handler a_other)
    {
        m_id = a_other.getValueString();
    }

    /**
     * Override of the equal operator.
     * @param a_other reference Handler to equal to.
     */
    void operator=(const StringHandler& a_other)
    {
        m_id = a_other.getValueString();
    }

    /**
     * Override of the equal operator.
     * @param a_other pointer Handler to equal to.
     */
    void operator=(StringHandler* a_other)
    {
        BCLIB_ASSERT(a_other != 0);
        m_id = a_other->getValueString();
    }

    /**
     * This function creates a copy of this handler. It returns a new handler
     * with the same identifier.
     * @return An instance copy of this handler.
     */
    Handler* getCopy();

    /**
     * This function compares two handlers. Returns -1 if 'this' is first,
     * 0 if they are equals and 1 if 'this' is last.
     * @param a_other Handler to compare with.
     * @return -1 if 'this' is first, 0 if they are equals and 1 if 'this' is last.
     */
    int compareTo(Handler* a_other);

    /**
     * This function returns the STRING value of the identifier.
     * @return The string value.
     */
    std::string getValueString() const
    {
        return m_id;
    }
};
}//BehaviorTree
}//MWLib

#endif//__MWLIB_BEHAVIORTREE_HANDLER_H__
