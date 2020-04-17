// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString &PlayerInput) override;
	void SetupGame();
	void EndGame();
	void LossLife();
	void ProcessGuess(const FString &PlayerGuess);
	bool bIsIsogram(const FString &Word) const;
	FString GetWord(const TArray<FString> &WordList);
	TArray<FString> GetValidWords(const TArray<FString> &WordList, int32 Letters) const;
	FBullCowCount GetBullCows(const FString &PlayerGuess) const;

	// Your declarations go below!
private:
	TArray<FString> IsogramsList;
	TArray<FString> UsedWordList;
	FString HiddenWord;
	int32 Lives;
	int32 Level = 1;
	bool bGameOver;
	bool bWon;
};
