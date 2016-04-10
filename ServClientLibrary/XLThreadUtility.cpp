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
#ifdef _C_11_COMPILER
    std::chrono::milliseconds ms(millsec);
    std::this_thread::sleep_for(ms);
#else
#ifdef __WINDOWS_OS_
	::Sleep(millsec);
#else
    //unsigned int nanoSeconds = millsec*1000000;
    //::usleep(nanoSeconds);
    int sec = millsec/1000;
    if(sec > 0)
    {
        sleep((unsigned int)(sec));
    }
    else
    {
        sleep((unsigned int)(1));
    }
#endif //__WINDOWS_OS_
#endif // _C_11_COMPILER
    
}
