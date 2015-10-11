/*
 * XLThreadUtility.h
 *
 *  Created on: 11-Oct-2015
 *      Author: akhilesh
 */

#ifndef XLTHREADUTILITY_H_
#define XLTHREADUTILITY_H_
#include "XLThreadDependency.h"

class XLThreadUtility
{
public:
	XLThreadUtility();
	virtual ~XLThreadUtility();
	static void Sleep(unsigned int milliseconds);
};

#endif /* XLTHREADUTILITY_H_ */
