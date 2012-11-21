/*
 * assert.h
 *
 *  Created on: 10.03.2010
 *      Author: andreasvogel
 */

#ifndef __H__COMMON__ASSERT__
#define __H__COMMON__ASSERT__

#include <cassert>
#include "common/log.h"
#include "ug_config.h"

#define UG_TO_STRING1(x)  #x
#define UG_TO_STRING(x)  UG_TO_STRING1(x)

////////////////////////////////////////////////////////////////////////
//	UG_ASSERT
///	Checks an expression at runtime and raises a runtime-error if the expression equals 0.
/**
 * UG_ASSERT is only active if NDEBUG is not defined.
 *
 * \param expr: an arbitrary expression that can be converted to bool.
 * \param msg: this message will be part of the error-message. Intended to give
 * 				more information about the checked condition and possible errors.
 *
 * example:		UG_ASSERT( num_dof >= 0, "Number of Degrees of Freedom can not be negative");
 *
 * output:
 *   UG_ASSERT failed:
 *     Condition:   num_dof >= 0
 *     Description: Number of Degrees of Freedom can not be negative
 *     File:        debug.cpp
 *     Line:        130
 *
 */
UG_API void ug_assert_failed();

#ifndef NDEBUG

#define UG_ASSERT(expr, msg)  {if(!(expr)) \
								{ \
									UG_LOG_ALL_PROCS(	"\n  UG_ASSERT failed:\n"\
														"    Condition:   " << UG_TO_STRING(expr) << "\n"\
														"    Description: " << msg << "\n"\
														"    File:        " << __FILE__ << "\n"\
														"    Line:        " << __LINE__ << "\n\n"); \
									ug_assert_failed(); \
									assert(expr);\
								}}
#else /* NDEBUG */
#define UG_ASSERT(expr, msg) {}
#endif

#endif /* __H__COMMON__ASSERT__ */
