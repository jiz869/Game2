//
//  Utilities.cpp
//  App42CPPSDK
//
//  Created by Rajeev Ranjan on 07/10/13.
//
//

#include "Utilities.h"
#include <stdio.h>
#include <stdarg.h>
#include "Signing.h"

void app42Trace(char* str, ...)
{
	char buf[512];
	va_list args;
	
	va_start(args, str);
	vsprintf(buf, str, args);
	va_end(args);
	printf("App42Trace:%s: %s",buf,Util::getTimeStamp().c_str());
}
