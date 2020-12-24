// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PathVisualiser/PathVisualiserGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePathVisualiserGameModeBase() {}
// Cross Module References
	PATHVISUALISER_API UClass* Z_Construct_UClass_APathVisualiserGameModeBase_NoRegister();
	PATHVISUALISER_API UClass* Z_Construct_UClass_APathVisualiserGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_PathVisualiser();
// End Cross Module References
	void APathVisualiserGameModeBase::StaticRegisterNativesAPathVisualiserGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_APathVisualiserGameModeBase_NoRegister()
	{
		return APathVisualiserGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_APathVisualiserGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APathVisualiserGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_PathVisualiser,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APathVisualiserGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "PathVisualiserGameModeBase.h" },
		{ "ModuleRelativePath", "PathVisualiserGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_APathVisualiserGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APathVisualiserGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_APathVisualiserGameModeBase_Statics::ClassParams = {
		&APathVisualiserGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_APathVisualiserGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_APathVisualiserGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_APathVisualiserGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_APathVisualiserGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(APathVisualiserGameModeBase, 2314008138);
	template<> PATHVISUALISER_API UClass* StaticClass<APathVisualiserGameModeBase>()
	{
		return APathVisualiserGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_APathVisualiserGameModeBase(Z_Construct_UClass_APathVisualiserGameModeBase, &APathVisualiserGameModeBase::StaticClass, TEXT("/Script/PathVisualiser"), TEXT("APathVisualiserGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APathVisualiserGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
