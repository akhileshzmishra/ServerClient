/*
 * XLThreadUtility.cpp
 *
 *  Created on: 11-Oct-2015
 *      Author: akhilesh
 */

#include "XLThreadUtility.h"
#include "XLThreadDependency.h"

XLThreadUtility::XLThreadUtility()
{
	// TODO Auto-generated constructor stub

}

XLThreadUtility::~XLThreadUtility()
{
	// TODO Auto-generated destructor stub
}


void XLThreadUtility::Sleep(unsigned int millsec)
{

#ifdef __WINDOWS_OS_
	::Sleep(millsec);
#else
	sleep((unsigned int)(millsec/1000));
#endif
}
