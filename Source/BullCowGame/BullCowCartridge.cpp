// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

#include "Count.h"
#include "HiddenWordList.h"

#include "EngineUtils.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();

	Isograms = GetValidWords(Words);

	SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
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
	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];

	Lives = HiddenWord.Len();
	bGameOver = false;

	PrintLine(TEXT("Welcome to the Bull Cows guessing game,"));
	PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
	PrintLine(TEXT("You have %i lives."), Lives);
	PrintLine(TEXT("Please enter something on the board.\n"));
	PrintLine(FString::Printf(TEXT("The HiddenWord is: %s."), *HiddenWord));
}

void UBullCowCartridge::EndGame()
{
	bGameOver = true;
	PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
	if (Guess == HiddenWord)
	{
		PrintLine(TEXT("You have won!"));
		EndGame();
		return;
	}
	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
		return;
	}

	if (!IsIsogram(Guess))
	{
		PrintLine(TEXT("No repeating letters, guess again."));
		PrintLine(TEXT("An isogram has only unique letters."));
		return;
	}
	if (Lives <= 0)
	{
		ClearScreen();
		PrintLine(TEXT("You have no lives left!"));
		PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
		EndGame();
		return;
	}

	FBullCowCount Score = GetBullCows(Guess);
	
	PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

	PrintLine(TEXT("Guess again, You have %i lives remaining."), --Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
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

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordLists) const
{
	TArray<FString> ValidWords;

	for (auto Word : WordLists)
	{
		if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
		{
			ValidWords.Emplace(Word);
		}
	}

	return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	// Initialize variables to prevent problems with out.
	FBullCowCount Count;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
		{
			if (HiddenWord[HiddenWordIndex] == Guess[GuessIndex])
			{
				Count.Cows++;
				break;
			}	
		}
	}

	return Count;
}
