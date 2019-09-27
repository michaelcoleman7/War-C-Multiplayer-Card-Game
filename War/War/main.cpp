/*
The aim of this app is to create the card game war

Produced on visual studio 2015

Coded by Michael Coleman
*/

#include<stdio.h>
#include<conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int playerNumber;
	char suit[13][3] = {"2","3","4","5","6","7","8","9","10","j","q","k","a"};
}player ;

typedef struct {
	int highestId;
	int roundScore;
}highest;

typedef struct {
	int loadPlayerCount;
	int loadCardCount;
	int loadRoundCount;
	int loadPLayerPoints[10];
	player loadPlayer[10];

	
}loadDetails;

//Functions
const char * ChooseCard(int playerId, char cardChosen[3], player *playerStruct, int cardCount);
void DisplayHand(int cardCount, player player);
void DisplayRound(const char *cardsPlayed[13][3],int count);
highest calculateWinner(const char *cardsPlayed[13][3], int count, highest calcHighest[10]);
void OutputGameStatus(int playerPoints[], int count);
void SaveGame(int playerPoints[], int cardCount,int playerCount, int roundCount, player player);
loadDetails LoadGame(int playerPoints[], int cardCount, int playerCount, int roundCount, player player, loadDetails loadPlayerDetails);

//main
void main() {
	//variables
	int option;
	int playerCount=0;
	int i, j, k;
	player players[10];
	int randomNumber;
	int randSuit[13];
	char cardChosen[3];
	int cardCount = 13;
	const char *cardsPlayed[10][3];
	int playerPoints[10] = { 0,0,0,0,0,0,0,0,0,0 };
	highest highscores[10];
	int rounds = 13;
	int roundCount = 0;
	int u = 0;
	loadDetails load;

	//for random number generation
	srand(time(NULL));

	//valid to verify player Count is valid
	bool valid = false;

	//options menu
	printf("1. New game\n2. Load existing game \n3. Exit Application \nEnter value from above:\n");
	scanf("%d", &option);
	while (option != NULL)
	{
		if (option != 1 && option != 2 && option != 3)
		{
			printf("Invalid input, please choose from these options\n");
			printf("1. New game\n2. Load existing game \n3. Exit Application \nEnter value from above:\n");
			scanf("%d", &option);
		}
		else {
			if (option == 1)
			{
				while (!valid) {
					printf("How many players would you like to have? (max 10)\n");
					scanf("%d", &playerCount);

					//if less than 10 
					if (playerCount <= 10) {

						//less than 10 and less than two 
						if (playerCount<2) {
							//invalid
							printf("Invalid input - Must be less than 10 players\n");
						}//if
						else {
							//valid
							valid = true;
						}//else
					}//if
					else if (playerCount>10) {
						//invalid
						printf("Invalid input - Must be less than 10 players\n");
					}//else If
				}//while

				//check roundCount to see if not = 13, if not enter loop.
				if (roundCount != 13) {

					for (u; u < rounds; u++) {
						roundCount++;

						//sentinal value
						const char *cardUsed = "99";

						//loop while cardUsed is equal to 99(sentinal value)
						do {
							DisplayHand(cardCount, players[0]);
							// Get card and validate if in array
							printf("Which card would you like to play - Must be from above list\n");
							scanf(" %s", &cardChosen);

							//if invalid card is chosen function returns 99 - if valid exits loop as wont be 99
							cardUsed = ChooseCard(0, cardChosen, players, cardCount);
						} while (cardUsed == "99");

						//add card
						cardsPlayed[0][3] = cardUsed;

						//for every player
						for (int x = 1; x < playerCount; x++) {
							//cardsPlayed[x][x] = "99";

							//variables for "Computer" cards
							const char *tempCard;
							int tempCardChosenInt = 0;
							char *tempCardChosen[3];

							//get random number less than 13
							tempCardChosenInt = (rand() % 13);

							//get card from players hand
							tempCardChosen[3] = players[x].suit[tempCardChosenInt];

							ChooseCard(x, tempCardChosen[3], players, cardCount);

							//add to array
							cardsPlayed[x][3] = tempCardChosen[3];
						}

						//decrease card count
						cardCount--;

						//display hand
						DisplayRound(cardsPlayed, playerCount);

						//Decide winner
						highscores[10] = calculateWinner(cardsPlayed, playerCount, highscores);

						//add score to the player score array
						playerPoints[highscores[10].highestId] += highscores[10].roundScore;

						printf("\nEnd of Round %d\n\n", roundCount);



						//switch to decide user options after each round. if roundCount=13, game finishes, so no need to enter switch.
						if (roundCount != 13) {
							int endRoundOption;

							printf("1. Complete Next Round\n2. Save Game \n3. Output Game status \n4. Exit Application without Saving \nEnter value from above:\n");
							scanf("%d", &endRoundOption);
							switch (endRoundOption) {
							case 1:
								//continue to next round - leave switch
								break;
							case 2:
								//Save game, then run main options
								SaveGame(playerPoints, cardCount,playerCount, roundCount, players[0]);
								main();
								break;
							case 3:
								//output current status of the game
								OutputGameStatus(playerPoints, playerCount);
								break;
							case 4:
								//exit to main options
								printf("Exit Without Saving\n");
								main();
								break;
							default:
								printf("Invalid entry - Your game has been saved and if you want to continue the game, select load game.\n");
								SaveGame(playerPoints, cardCount, playerCount, roundCount, players[0]);
								main();
								break;
							}//switch
						}//if
						else {
							//Display that the Game is over, then back to main options
							printf("\n-----------------------------\nGame Over\n");
							OutputGameStatus(playerPoints, playerCount);
							
							main();
						}
					}//for
				}//if for loop
			}//if
			else if (option==2) {
				//Load Game from struct returned by loadGame function

				//initialise main variables with struct that is returned
				load = LoadGame(playerPoints, cardCount,playerCount, roundCount, players[0], load);
				cardCount = load.loadCardCount;
				playerCount = load.loadPlayerCount;
				roundCount = load.loadRoundCount;
				for (int h = 0; h < cardCount; h++)
				{
					*players[0].suit[h] = *load.loadPlayer[0].suit[h];
				}

				for (int q = 0; q < playerCount; q++)
				{
					playerPoints[q] = load.loadPLayerPoints[q];
				}

				//set option to 1 to continue game and valid = true to bypass entering number of players again.
				option = 1;
				valid = true;
			}
			else if (option == 3) {
				//exit application
				exit(EXIT_SUCCESS);
			}

		}//else
	}//while
}//main

//function to allow players to choose a card
const char *ChooseCard(int playerId, char cardChosen[3], player *players, int cardCount)
{
	const char *matched;
	int length = 0;
	int i = 0, j = 0;
	length = cardCount;

	//loops for every element of array e.g. 13 times, then 12,11.. etc..
	for (i = 0; i < length; i++)
	{
		//compares stings for player with chosen card 
		matched = strstr(players[playerId].suit[i], cardChosen);

		//if matched is not null then array element isnt chosen card
		if (matched != NULL) {

			//remove element from array
			for (j = i; j < length-1; j++)
			{
				//overwrite the matched card so as to move each array item.
				strcpy(players[playerId].suit[j], players[playerId].suit[j + 1]);
			}

			return cardChosen;
		}//if
	}//for
	return "99";
}

//prints users hand
void DisplayHand(int cardCount,player player) {
	printf("This is your hand\n");
	for (int k = 0; k < cardCount; k++)
	{
		printf("%s ", player.suit[k]);
	}//for
	printf("\n");
}

//prints all cards played in round
void DisplayRound(const char *cardsPlayed[13][3],int count) {
	printf("Cards Played\n");
	
	for (int k = 0; k < count; k++)
	{
		printf("%s ", cardsPlayed[k][3]);
	}//for
	printf("\n");
}

//calculates the highest card and allocates points to each player
highest calculateWinner(const char *cardsPlayed[13][3], int count, highest calcHighest[10]) {
	int highestCard=0;
	int HighestCardId=0;
	bool HadEquals = false;
	int scoreArray[10];
	char tempCard;
	int roundScore = 0;
	int highEquals;

	for (int k = 0; k < count; k++)
	{
		tempCard = *cardsPlayed[k][3];

		switch (tempCard)
		{
			//add to scoreArray - used as card used are chars, so changed to int array to calculate highest card played.
			case '2':
				scoreArray[k] = 2;
				roundScore = roundScore + 2;
				break;

			case '3':
				scoreArray[k] = 3;
				roundScore = roundScore + 3;
				break;

			case '4':
				scoreArray[k]= 4;
				roundScore = roundScore + 4;
				break;

			case '5':
				scoreArray[k] = 5;
				roundScore = roundScore + 5;
				break;

			case '6':
				scoreArray[k] = 6;
				roundScore = roundScore + 6;
				break;

			case '7':
				scoreArray[k] = 7;
				roundScore = roundScore + 7;
				break;

			case '8':
				scoreArray[k] = 8;
				roundScore = roundScore + 8;
				break;

			case '9':
				scoreArray[k] = 9;
				roundScore = roundScore + 9;
				break;

			case '10':
				scoreArray[k] = 10;
				roundScore = roundScore + 10;
				break;

			case 'j':
			case 'J':
				scoreArray[k] = 11;
				roundScore = roundScore + 11;
				break;

			case 'q':
			case 'Q':
				scoreArray[k] = 12;
				roundScore = roundScore + 12;
				break;

			case 'k':
			case 'K':
				scoreArray[k] = 13;
				roundScore = roundScore + 13;
				break;

			case 'a':
			case 'A':
				scoreArray[k] = 14;
				roundScore = roundScore + 14;
				break;

			default:
				break;
		}//switch

		//calculate highest card and player id.
		if (scoreArray[k] > highestCard) {
			highestCard = scoreArray[k];
			HighestCardId = k;
		}

	}//for

	//output highest and round score
	printf("\nHighest Score is: %d\n", highestCard);
	printf("Player with highest card is: Player %d\n", HighestCardId+1);
	printf("Total Round Score is: %d\n", roundScore);

	//add highest details to struct and send struct back to main
	calcHighest[10].highestId = HighestCardId;
	calcHighest[10].roundScore = roundScore;
	return calcHighest[10];
}

//function which outputs the current game status
void OutputGameStatus(int playerPoints[], int count) {
	printf("Current Score Standings are:\n");
	printf("----------------------------\n");

	for (int o = 0; o < count; o++)
	{
		printf("player %d: %d \n",o+1, playerPoints[o]);
	}
	printf("----------------------------\n");
}

//function allows for saving of game
void SaveGame(int playerPoints[], int cardCount, int playerCount, int roundCount, player player) {
	FILE* filep;

	//open file
	filep = fopen("SaveHand.txt", "w");

	//if file cannot be opened
	if (filep == NULL)
	{
		printf("The Save Game file could not be opened\n");
	}

	else
	{
		//store roundcount to file
		fprintf(filep, "%d ", roundCount);

		//store card count and player hand
		fprintf(filep, "%d ", cardCount);

		for (int x = 0; x < cardCount; x++) {
			fprintf(filep, "%s ", player.suit[x]);
		}

		fprintf(filep, "\n");

		//close file
		fclose(filep);

		filep = fopen("SavePoints.txt", "w");
		//store current score standings and player count
		fprintf(filep, "%d ", playerCount);

		for (int y = 0; y < playerCount; y++) {
			fprintf(filep, "%d ", playerPoints[y]);
		}
	}
	//close files
	fclose(filep);
}

//function which loads the save from a file.
loadDetails LoadGame(int playerPoints[], int cardCount, int playerCount, int roundCount, player player, loadDetails loadPlayerDetails) {
	FILE* filep;

	//open file
	filep = fopen("SaveHand.txt", "r");

	//if file cannot be loaded
	if (filep == NULL)
	{
		printf("The Save Game file could not be loaded\n");
		exit(EXIT_SUCCESS);
	}

	else
	{
		while (!feof(filep)) {

			fscanf(filep, "%d ", &roundCount);
			loadPlayerDetails.loadRoundCount=roundCount;

			//load player count and player points and save to struct.
			fscanf(filep, "%d ", &cardCount);
			loadPlayerDetails.loadCardCount = cardCount;

			for (int x = 0; x < cardCount; x++) {
				fscanf(filep, "%s ", &player.suit[x]);
				*loadPlayerDetails.loadPlayer[0].suit[x] = *player.suit[x];
			}
			printf("\n");

		}
	}

	filep = fopen("SavePoints.txt", "r");

	if (filep == NULL)
	{
		printf("The Save file could not be opened\n");
	}

	else
	{
		while (!feof(filep)) {
			//load player count and player points and save to struct
			fscanf(filep, "%d ", &playerCount);
			loadPlayerDetails.loadPlayerCount = playerCount;

			for (int y = 0; y < playerCount; y++) {
				fscanf(filep, "%d ", &playerPoints[y]);
				loadPlayerDetails.loadPLayerPoints[y] = playerPoints[y];
			}
		}
	}
	//close file
	fclose(filep);

	//return struct
	return loadPlayerDetails;
}



