#pragma once
#include "Toolkit/AssetGeneration/AssetTypeGenerator.h"
#include "UserDefinedStructGenerator.generated.h"

UCLASS(MinimalAPI)
class UUserDefinedStructGenerator : public UAssetTypeGenerator {
	GENERATED_BODY()
protected:
	virtual UPackage* CreateAssetPackage() override;
	virtual void OnExistingPackageLoaded() override;
	void PopulateStructWithData(class UUserDefinedStruct* Struct);
	bool IsStructUpToDate(class UUserDefinedStruct* Struct) const;
	virtual void FinalizeAssetCDO() override;
public:
	virtual void PopulateStageDependencies(TArray<FAssetDependency>& OutDependencies) const override;
	virtual FName GetAssetClass() override;
};