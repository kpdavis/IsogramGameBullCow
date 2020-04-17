// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput)
{
    /*if game is over then do ClearScreen() and SetupGame()
    * else check Guess*/
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    if (Level == 1)
    {
        PrintLine(TEXT("Hello there player,"));
        PrintLine(TEXT("Welcome to Bull Cows!"));
        PrintLine(TEXT("Bulls = letter in right place,\nCows = letter in word but wrong spot"));
        IsogramsList = GetValidWords(WordsList, 1);
    }
    else if (Level == 4)
    {
        IsogramsList.Empty();
        UsedWordList.Empty();
        IsogramsList += GetValidWords(WordsList, 3);
    }
    else if (Level == 18)
    {

        IsogramsList += GetValidWords(WordsList, 5);
    }
    else if (Level == 32)
    {
        IsogramsList += GetValidWords(WordsList, 7);
    }
    else if (Level == 50)
    {
        IsogramsList += GetValidWords(WordsList, 9);
    }

    HiddenWord = GetWord(IsogramsList);
    if (HiddenWord == TEXT("GAMEOVER"))
    {
        EndGame();
        return;
    }

    Lives = HiddenWord.Len();
    bGameOver = false;
    bWon = false;

    PrintLine(TEXT("Let see if you can guess the Isogram."));
    PrintLine(TEXT("Give it a try and press enter."));
    PrintLine(TEXT("The Isogram is %i letters long.   Lives: %i"), HiddenWord.Len(), Lives);

    // PrintLine(FString::Printf(TEXT("The Isogram is %s "), *HiddenWord)); //TODO Debut Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    ClearScreen();

    if (HiddenWord == TEXT("GAMEOVER"))
    {
        PrintLine(TEXT("You Won! you Guessed all the Isograms\nwe have ready."));

        UsedWordList.Empty();
        Level = 1;
    }
    else if (bWon)
    {
        ++Level;

        PrintLine(TEXT("You Won!\nIsogram was: %s"), *HiddenWord);
    }
    else
    {
        PrintLine(TEXT("Not quite"));
        PrintLine(TEXT("The Isogram was: %s "), *HiddenWord);

        UsedWordList.Empty();
        IsogramsList.Empty();
        Level = 1;
    }
    PrintLine(TEXT("You Made it to Level: %i "), Level);
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString &PlayerGuess)
{
    if (PlayerGuess == HiddenWord)
    {
        bWon = true;
        EndGame();
        return;
    }
    if (PlayerGuess.Len() == HiddenWord.Len())
    {
        if (!bIsIsogram(PlayerGuess))
        {
            PrintLine(TEXT("No repeating letters in an isogram."));
            LossLife();
        }
    }
    else
    {
        LossLife();
    }
    if (!bGameOver)
    {
        FBullCowCount Score = GetBullCows(PlayerGuess);
        PrintLine(TEXT("Bulls: %i   Cows: %i   Lives: %i"), Score.Bulls, Score.Cows, Lives);
    }
}

void UBullCowCartridge::LossLife()
{
    --Lives;
    if (Lives > 0)
    {
        PrintLine(TEXT("The Isogram is %i letters long. Try again."), HiddenWord.Len());
    }
    else
    {
        EndGame();
        return;
    }
}

bool UBullCowCartridge::bIsIsogram(const FString &Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList, int32 Letters) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() > (Letters) && Word.Len() <= (Letters + 2))
        {
            if (bIsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
    }
    return ValidWords;
}

FString UBullCowCartridge::GetWord(const TArray<FString> &WordList)
{
    FString ReturnWord;

    if (UsedWordList == WordList)
    {
        return TEXT("GAMEOVER");
    }

    do
    {
        ReturnWord = WordList[FMath::RandRange(0, WordList.Num() - 1)];

        if (!UsedWordList.Contains(ReturnWord) || UsedWordList.Num() <= 0)
        {
            UsedWordList.Emplace(ReturnWord);
            return ReturnWord;
        }
    } while (true);
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &PlayerGuess) const
{
    FBullCowCount Count;

    FString Guess = PlayerGuess.ToLower();
    FString Word = HiddenWord.ToLower();

    //Check if the FStrings length and set SLenght for high and low lengths
    int32 SLenghtHigh, SLenghtLow;
    if (Word.Len() >= Guess.Len())
    {
        SLenghtHigh = Word.Len();
        SLenghtLow = Guess.Len();

        for (int32 Index = 0; Index < SLenghtLow; Index++)
        {
            if (Guess[Index] == Word[Index])
            {
                Count.Bulls++;
                continue;
            }
            for (int32 HiddenIndex = 0; HiddenIndex < SLenghtHigh; HiddenIndex++)
            {
                if (Guess[Index] == Word[HiddenIndex])
                {
                    Count.Cows++;
                    break;
                }
            }
        }
    }
    else if (Word.Len() < Guess.Len())
    {
        SLenghtHigh = Guess.Len();
        SLenghtLow = Word.Len();

        for (int32 Index = 0; Index < SLenghtLow; Index++)
        {
            if (Guess[Index] == Word[Index])
            {
                Count.Bulls++;
                continue;
            }
            for (int32 HiddenIndex = 0; HiddenIndex < SLenghtHigh; HiddenIndex++)
            {
                if (Guess[HiddenIndex] == Word[Index])
                {
                    Count.Cows++;
                    break;
                }
            }
        }
    }

    return Count;
}