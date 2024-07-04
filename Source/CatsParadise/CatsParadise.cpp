// Fill out your copyright notice in the Description page of Project Settings.

#include "CatsParadise.h"
#include "Modules/ModuleManager.h"

void FCatsParadiseModule::StartupModule()
{
	FString ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("CustomShaders"));
	AddShaderSourceDirectoryMapping("/Project", ShaderDirectory);
}

IMPLEMENT_PRIMARY_GAME_MODULE( FCatsParadiseModule, CatsParadise, "CatsParadise" );
