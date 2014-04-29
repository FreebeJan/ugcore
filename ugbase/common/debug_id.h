/* 
 * File:   DebugID.h
 * Author: Martin Rupp
 *
 * Created on 22. Oktober 2012, 13:55
 */

#ifndef __H__UG__COMMON__DEBUG_ID_H
#define	__H__UG__COMMON__DEBUG_ID_H
#include <vector>
#include <map>
#include <string>
#include "common/util/crc32.h"
#include <sstream>

namespace ug{

/// \addtogroup ugbase_common
/// \{

class DebugIDManager;



// DebugID
/**
 * A DebugID is an object to control the debug level of parts of the code.
 * It can be used in
 * - UG_SET_DEBUG_LEVEL
 * - UG_DEBUG_BEGIN/END (debugID, level)
 * - IF_DEBUG
 * - UG_DLOG(debugID, level, msg)
 *
 * ug4's standard DebugIDs are
 * APP, LIB_GRID, LIB_GRID_REFINER, LIB_DISC, LIB_DISC_ASSEMBLE, LIB_DISC_D3F,
 *		LIB_DISC_MULTIGRID, LIB_DISC_NEWTON, LIB_DISC_LINKER, LIB_DISC_TRANSFER,
 *		LIB_DISC_DISCRETE_FUNCTION, LIB_DISC_OUTPUT, LIB_DISC_OPERATOR_INVERSE,
 *		LIB_ALG_LINEAR_OPERATOR, LIB_ALG_LINEAR_SOLVER, LIB_ALG_VECTOR,
 *		LIB_ALG_MATRIX, LIB_ALG_AMG, LIB_PCL
 *
 * They are managed with \sa DebugIDManager.
 * You can define your own DebugID the following way:
 * In a cpp file:
 *
 \code{.cpp}
 DebugID DID_MYAPP("MyApp")
 DebugID DID_MYAPP_PARALLELIZATION("MyApp.Parallelization")
 \endcode{.cpp}
 *
 * Now to use it in other files you need to add there
 *
 \code{.cpp}
 extern DebugID DID_MYAPP, DID_MYAPP_PARALLELIZATION;
 ...
 UG_DLOG(DID_MYAPP, 3, "bla");
 \endcode{.cpp}
 *
 * Note that the DebugID OBJECT is called DID_MYAPP, but its string identifier
 * is "MyApp" (this is used in the script system). In LUA script you can do
 *
 * GetLogAssistant():set_debug_level("MyApp", 3)
 * or, with completion (UGIDE)
 * SetDebugLevel(debugID.MyApp, 3)
 *
 * \note The actual DebugID level is stored in DebugIDManager.
 *
 */
class DebugID
{
public:
/// registers the DebugID at DebugIDManager
	DebugID(const char *str);
	
	
	DebugID(uint32 hash) { m_hash = hash; }
/// returns the debug level via GetDebugIDManager.
	inline int get_debug_level() const;

/// sets the debug level via GetDebugIDManager.
	inline bool set_debug_level(int level);

	friend class DebugIDManager;
private:
	uint32 m_hash;
};

// the list of ug4's standard DebugIDs.
extern DebugID MAIN,
		APP,
		LIB_GRID,
		LIB_GRID_REFINER,
		LIB_DISC,
		LIB_DISC_ASSEMBLE,
		LIB_DISC_D3F,
		LIB_DISC_MULTIGRID,
		LIB_DISC_NEWTON,
		LIB_DISC_LINKER,
		LIB_DISC_TRANSFER,
		LIB_DISC_DISCRETE_FUNCTION,
		LIB_DISC_OUTPUT,
		LIB_DISC_OPERATOR_INVERSE,
		LIB_ALG_LINEAR_OPERATOR,
		LIB_ALG_LINEAR_SOLVER,
		LIB_ALG_VECTOR,
		LIB_ALG_MATRIX,
		LIB_ALG_AMG,
		LIB_PCL;


/**
 * The DebugIDManager.
 * get, set debug level, get a list of available DebugIDs etc.
 */
class DebugIDManager
{
private:
	/// returns the debug level of debugIDhash, -1 if not found
		inline int get_debug_level(uint32 debugIDhash) const
		{
			std::map<uint32, int>::const_iterator it = m_dbgLevels.find(debugIDhash);
			if(it == m_dbgLevels.end()) return -1;
			else return (*it).second;
		}
		
		/// sets the debug level of debugIDhash if registered to 'level'
		inline bool set_debug_level(uint32 debugIDhash, int level)
		{
			std::map<uint32, int>::iterator it = m_dbgLevels.find(debugIDhash);
			if(it == m_dbgLevels.end()) return false;
			else
			{
				(*it).second = level;
				return true;
			}
		}
public:
	///	returns a reference to the single instance of LogAssistant
		static DebugIDManager& instance();

	/// returns true if there is a DebugID with name debugIDname
		inline bool debug_id_registered(const char *debugIDname) const
		{
			return m_dbgLevels.find(crc32(debugIDname)) != m_dbgLevels.end();
		}
	/// returns the debug level of debugID, -1 if not found
		inline int get_debug_level(const DebugID &debugID) const
		{
			return get_debug_level(debugID.m_hash);
		}
	/// returns the debug level of debugID, -1 if not found
		inline int get_debug_level(const char *debugID) const
		{
			return get_debug_level(crc32(debugID));
		}
		
	/// 
		inline DebugID get_debug_id(const char *debugID) const
		{
			return DebugID(crc32(debugID));
		}
		
		inline DebugID get_or_create_debug_id(const char *debugID)
		{
			if(debug_id_registered(debugID)==false)
				register_debug_id(debugID);
			return DebugID(crc32(debugID));
		}
				

	/// sets the debug level of debugID if registered to 'level'
		inline bool set_debug_level(DebugID &debugID, int level)
		{
			return set_debug_level(debugID.m_hash, level);
		}
	/// sets the debug level of debugID if registered to 'level'
		bool set_debug_level(const char *debugID, int level);

	
	/// sets the debug level of all registered debugIDs to 'lev'
		bool set_debug_levels(int lev);

	/// returns a string describing all registered debugIDs.
		std::string get_registered_debug_IDs() const
		{
			std::stringstream str;
			str << "DebugIDs:\n";
			for(size_t i=0; i<m_dbgLevelIdentifiers.size(); i++)
				str << m_dbgLevelIdentifiers[i] << " : Level " << get_debug_level(m_dbgLevelIdentifiers[i].c_str()) << "\n";
			return str.str();
		}

		const std::vector<std::string> &get_registered_debug_IDs_arr() const
		{
			return m_dbgLevelIdentifiers;
		}



	bool register_debug_id(const char *debugID);

private:
	std::vector<std::string> m_dbgLevelIdentifiers;
	std::map<uint32, int> m_dbgLevels;
};

inline DebugIDManager& GetDebugIDManager()
{
	return DebugIDManager::instance();
}

inline int DebugID::get_debug_level() const
{
	return GetDebugIDManager().get_debug_level(*this);
}
inline bool DebugID::set_debug_level(int level)
{
	return GetDebugIDManager().set_debug_level(*this, level);
}

// TemporaryDebugLevel
/// a helper object to temporary set debug levels in a given scope
/**
 * A object of this class can be used to change a DebugID. The DebugID will
 * be re-set to the value before at destruction of the object.
 * This is especially useful if algorithms are calling other functions which
 * also have special DebugIDs levels. And you want to do a "debug debug".
 * example:
\code{.cpp}
  UG_SET_DEBUG_LEVEL(LIB_GRID, 1);

  // DebugID LIB_GRID is now at level 1
  {
  	  TemporaryDebugLevel tdl(LIB_GRID, 5);

  	  // DebugID LIB_GRID is now at level 5
  	  MyFunction();
  }

  // DebugID LIB_GRID is now again at 1.
\endcode
 * You can also specify differences:
 *
 \code{.cpp}
  TemporaryDebugLevel tdl(LIB_GRID, MY_DEBUG_ID, +1);
  // now LIB_GRID has the temporary debug level of MY_DEBUG_ID +1.
 */
class TemporaryDebugLevel
{
	DebugID &did;
	int m_prevDebugLevel;
public:
	TemporaryDebugLevel(DebugID &debugID) : did(debugID)
	{
#ifdef UG_ENABLE_DEBUG_LOGS
		m_prevDebugLevel = did.get_debug_level();
#endif
	}

	/// change the debugID level in this scope
	TemporaryDebugLevel(DebugID &debugID, int temporaryDebugLevel) : did(debugID)
	{
#ifdef UG_ENABLE_DEBUG_LOGS
		m_prevDebugLevel = did.get_debug_level();
		did.set_debug_level(temporaryDebugLevel);
#endif
	}

	/// change the debugID level in this scope to another debugID plus some diff.
	TemporaryDebugLevel(DebugID &debugID, DebugID &debugSrc, int diff=0) : did(debugID)
	{
#ifdef UG_ENABLE_DEBUG_LOGS
		m_prevDebugLevel = did.get_debug_level();
		did.set_debug_level(debugSrc.get_debug_level()+diff);
#endif
	}

	/// resets the debugID to the previous value.
	~TemporaryDebugLevel()
	{
#ifdef UG_ENABLE_DEBUG_LOGS
		did.set_debug_level(m_prevDebugLevel);
#endif
	}

	void set(int temporaryDebugLevel)
	{
#ifdef UG_ENABLE_DEBUG_LOGS
		did.set_debug_level(temporaryDebugLevel);
#endif
	}
};

// end group ugbase_common
/// \}

}	// end namespace ug
#endif	/* __H__UG__COMMON__DEBUG_ID_H */

