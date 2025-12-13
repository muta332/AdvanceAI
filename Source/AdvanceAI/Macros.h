#pragma once

#include "CoreMinimal.h"

#define DO_ONCE (_bool_)\
	{(!flag)   {flag = true  ;        \
     
#define DO_ONCE_END }

#define DO_N(N, Counter)\
	\
	if(!Counter > N)\
	{\
	Counter++;\
	

#define DON_END }

#define PRINT(Key, TimeToDisplay, FColor, DebugString){if(GEngine) {GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, FColor, DebugString);}}

#define PRINTONE(DebugString){if(GEngine) {GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, DebugString);}}

