#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

#define DOONCE(Flag)\
	if(Flag == false)\
	{Flag = true

#define DOONCE_END }

#define DOONCE_RESET(Flag)\
	Flag = false;

#define DON(Counter, N)\
if ( Counter != N ||  Counter < N){\
	 Counter++;

#define DON_END }

#define DON_RESET(Counter) Counter = 0;

#define PRINTONE(DebugMsg) if(GEngine){GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green,DebugMsg);}

#define PRINT(Key, TimeToDisplay, FColor, DebugMsg) if(GEngine)  {GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, FColor,DebugMsg);}

#define PRINTPERFRAME(DebugMsg) if(GEngine){GEngine->AddOnScreenDebugMessage(1, 0, FColor::Green,DebugMsg);}

#define DRAWDEBUGSPHERE(CenterLocationVector)DrawDebugSphere(this->GetWorld(), CenterLocationVector ,40.f, 12, FColor::Green, false, 2.f);

#define DRAWDEBUGSPHERECUSTOM(UWorld, CenterVector, Radius, Segments, Color, PresistentLines, LifeTime)
