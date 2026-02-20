// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GAIGameMode.h"
#include "VisualLogger/VisualLogger.h"

void AGAIGameMode::StartPlay()
{
	
#if ENABLE_VISUAL_LOG
	FVisualLogger& VLog = FVisualLogger::Get();
	if (!VLog.IsRecording())
	{
		VLog.SetIsRecording(true);
	}
	
#endif
	
	Super::StartPlay();
	
}

void AGAIGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	
#if ENABLE_VISUAL_LOG
	FVisualLogger& VLog = FVisualLogger::Get();
	if (VLog.IsRecording())
	{
		VLog.SetIsRecording(false);
	}
	
#endif
	Super::EndPlay(EndPlayReason);
}
