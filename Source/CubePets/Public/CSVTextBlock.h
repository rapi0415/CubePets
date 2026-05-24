// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "CSVTextBlock.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FCubePetsTextData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TextData")
	FText SystemText;
};

UCLASS()
class CUBEPETS_API UCSVTextBlock : public UTextBlock
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CSV Data")
	UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CSV Data")
	FName CSVRowName;
	
protected:

	virtual void SynchronizeProperties() override;

public:

	UFUNCTION(BlueprintCallable)
	void UpdateTextID(FName NewTextID);

};
