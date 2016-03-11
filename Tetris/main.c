
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <string.h>
#include <Windows.h>

/* array size */
#define TETRIS_SIZE 4

/* arrow key */
#define UP 72
#define DOWN 80
#define RIGHT 77
#define LEFT 75

/* */
#define SPACE 32
#define ESC 27

#define HEIGHT 21 // 20+1
#define WIDTH 12 // 10+2
   
#define PRINT_BLANCK printf("  ");
#define PRINT_BLOCK printf("＝");

/* Tetromino  */
// I, J, L, O, S, T, Z
int tetromino[7][TETRIS_SIZE][TETRIS_SIZE] = {
	// I
	{
		{ 0, 0, 1, 0 }, 
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 1, 0 }
	},
	// J
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	// L
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 1 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	// O
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	// S
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 0, 0 }
	},
	// T
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 1, 1, 1 },
		{ 0, 0, 0, 0 }
	},
	// Z
	{
		{ 0, 0, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 0, 0 }
	}
};

/************************* Functions *************************/

// TESTED
void rotateBlock(int block[][TETRIS_SIZE]) {
	int tempBlock[TETRIS_SIZE][TETRIS_SIZE] = { 0 };
	int i = 0,  j = 0;

	/* 葵 差紫背砧奄 */
	memcpy(tempBlock, block, sizeof(tempBlock));

	/* 噺穿 */
	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			block[j][3 - i] = tempBlock[i][j];
		}
	}

	return;
}

// TESTED
int oneLineFilled(int line[WIDTH]) {
	int i = 0;

	for (i = 0; i < WIDTH; i++) {
		if (line[i] != 1) {
			return 0; // false
		}
	}
	return 1; // true
}

// TESTED
int returnBlockIndexRandomly() {

	srand((unsigned)clock());

	return rand() % 7 ;
}

// TESTED
void printBoard(int gameBoard[][WIDTH]) {
	int i = 0, j = 0;
	COORD initialConsoleCursorPosition = { 0, 3 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), initialConsoleCursorPosition);

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (gameBoard[i][j] == 1) {
				PRINT_BLOCK;
			}
			else {
				PRINT_BLANCK;
			}
		}
		printf("\n");
	}
	return;
}

// TESTED
void makeBasicGameBoard(int gameBoard[][WIDTH]) {
	int i = 0, j = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if ((j == 0 || j == WIDTH - 1) || i == HEIGHT - 1) {
				gameBoard[i][j] = 1;
			}
			else {
				gameBoard[i][j] = 0;
			}
		}
	}
}

// TESTED
int checkWhetherAbleToMove(int gameBoard[][WIDTH], int* downPosition, int* leftAndRightPosition) {
	int i = 0, j = 0;

	for (i = 0; i < TETRIS_SIZE; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			if (gameBoard[*downPosition - i][*leftAndRightPosition + j] > 1) {
				return 0; // can NOT move
			}
			else {
				continue;
			}
		}
	}
	return 1;

	/* TEST code of this function */
	//memcpy(tempBoard, gameBoard, sizeof(gameBoard));
	//memcpy(fallingBlock, tetromino[0], sizeof(fallingBlock));
	//rotateBlock(fallingBlock);
	//addBlockDataToGameBoardData(tempBoard, fallingBlock, downPos, leftAndRightPos);
	//if (checkWhetherAbleToMove(tempBoard, downPos, leftAndRightPos) == 1) {
	//	memcpy(gameBoard, tempBoard, sizeof(gameBoard));
	//}
	//else {
	//	memcpy(tempBoard, gameBoard, sizeof(tempBoard));
	//}
	//printBoard(gameBoard);
	/******************************/
}

// TESTED
void subtractBlockFromGameBoard(int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {
	int i = 0, j = 0;
	int rowCount = 0;

	// 鷺薫 析採幻 蟹臣 凶 (鷺薫 恭嬢走澗 段奄)
	if (*downPosition + 1 < TETRIS_SIZE) {
		rowCount = *downPosition + 1;
	}
	// 鷺薫 穿採 陥 蟹臣 凶
	else {
		rowCount = TETRIS_SIZE;
	}
	// gameboard 鎧 block税 1昔 採歳 0生稽 郊荷奄
	for (i = 0; i < rowCount; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			if (block[TETRIS_SIZE - (i + 1)][j] == 1) {
				gameBoard[*downPosition - i][*leftAndRightPosition + j] = 0;
			}
		}
	}
}

// TESTED
void addBlockDataToGameBoardData(int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {

	int i = 0, j = 0;
	int rowCount = 0;

	// 鷺薫 析採幻 蟹臣 凶 (鷺薫 恭嬢走澗 段奄)
	if (*downPosition + 1 < TETRIS_SIZE) {
		rowCount = *downPosition + 1;
	}
	// 鷺薫 穿採 陥 蟹臣 凶
	else {
		rowCount = TETRIS_SIZE;
	}

	// gameboard data, block data 希馬奄
	for (i = 0; i < rowCount; i++) {
		for (j = 0; j < TETRIS_SIZE; j++) {
			gameBoard[*downPosition - i][*leftAndRightPosition + j] += block[TETRIS_SIZE - (i + 1)][j];
		}
	}
	/* This is TEST code of this function*/
	//int i = 0, j = 0;
	//int gameBoard[HEIGHT][WIDTH];
	//int* downPos;
	//int* leftAndRightPos;
	//int down = 7, leftAndRight = 6;
	//downPos = &down;
	//leftAndRightPos = &leftAndRight;
	//for (i = 0; i < HEIGHT; i++) {
	//	for (j = 0; j < WIDTH; j++) {
	//		if ((j == 0 || j == WIDTH - 1) || i == HEIGHT - 1) {
	//			gameBoard[i][j] = 1;
	//		}
	//		else {
	//			gameBoard[i][j] = 0;
	//		}
	//	}
	//}
	//addBlockDataToGameBoardData(gameBoard, tetromino[0], downPos, leftAndRightPos);
	//printBoard(gameBoard);
	/********************/
	return;
}


// TODO: 
// 1段 角醸聖 凶 益形捜

// NOT COMPLETED
int processUserInput(int arrowKey, int gameBoard[][WIDTH], int block[][TETRIS_SIZE], int* downPosition, int* leftAndRightPosition) {
	int forChangedPosition = 0;
	int* changedPosition = &forChangedPosition;
	int tempBoard[HEIGHT][WIDTH];
	
	/*
	 * 惟績毒 print澗 gameBoard稽幻 廃陥
	 * 惟績毒 data 繕拙精 tempBoard稽幻 廃陥
	 */
	memcpy(tempBoard, gameBoard, sizeof(tempBoard));

	switch (arrowKey) {

	case UP: /////////////////////////////////////////////////////////////// 混拭 亨 細嬢辞 rotate 馬檎 混拭 亨 含虞 細製 せせせせせせせせせせせせせせせせせせせせせ
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		rotateBlock(block);
		addBlockDataToGameBoardData(tempBoard, block, downPosition, leftAndRightPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, leftAndRightPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	case DOWN:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		*changedPosition = *downPosition + 1;
		addBlockDataToGameBoardData(tempBoard, block, changedPosition, leftAndRightPosition);
		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			*downPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
			return 1; // 鷺薫 陥 恭嬢然製(true) => I think this is not good
		}

		break;

	case RIGHT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		*changedPosition = *leftAndRightPosition + 1;
		addBlockDataToGameBoardData(tempBoard, block, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			*leftAndRightPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	case LEFT:
		if (*downPosition != 0) {
			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
		}
		*changedPosition = *leftAndRightPosition - 1;
		addBlockDataToGameBoardData(tempBoard, block, downPosition, changedPosition);
		if (checkWhetherAbleToMove(tempBoard, downPosition, changedPosition) == 1) {
			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
			*leftAndRightPosition = *changedPosition;
		}
		else {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
		}
		break;

	//case SPACE:
	//	while (1) {
	//		if (*downPosition != 0) {
	//			subtractBlockFromGameBoard(tempBoard, block, downPosition, leftAndRightPosition);
	//		}
	//		*changedPosition = *downPosition + 1;
	//		addBlockDataToGameBoardData(tempBoard, block, changedPosition, leftAndRightPosition);
	//		if (checkWhetherAbleToMove(tempBoard, changedPosition, leftAndRightPosition) == 1) {
	//			memcpy(gameBoard, tempBoard, sizeof(tempBoard));
	//			*downPosition = *changedPosition;
	//		}
	//		else {
	//			memcpy(tempBoard, gameBoard, sizeof(tempBoard));
	//			break;
	//		}
	//	}
	//	break;

	default :
		break;
	}

	return 0; // 鷺薫 陥 照恭嬢像, 域紗 繕拙 閤聖呪 赤製
}



// FUNCTION FOR TEST
void functionForTest() {
	int i = 0, j = 0;
	int gameBoard[HEIGHT][WIDTH];
	int indexDown = 0, indexLeftAndRight = 4;
	int* downPosition;
	int* leftAndRightPosition;

	int fallingBlock[TETRIS_SIZE][TETRIS_SIZE];
	int tempBoard[HEIGHT][WIDTH]; // oneLineFilled 析凶 紫遂

	int userInputKeyCode; 

	int mustReturnNewBlock = 1;

	downPosition = &indexDown;
	leftAndRightPosition = &indexLeftAndRight;

	makeBasicGameBoard(gameBoard);
	
	printf("************\n");
	printf("THIS IS TEST\n");
	printf("************\n\n");

	while (1) {
		printBoard(gameBoard);
		
		/* 鷺薫戚 鎧形身 */
		if (mustReturnNewBlock) {
			memcpy(fallingBlock, tetromino[returnBlockIndexRandomly()], sizeof(fallingBlock));
			//memcpy(fallingBlock, tetromino[0], sizeof(fallingBlock)); // tetromino 1, 2 ERROR // 固購析凶 // J L 亨 益 乞丞析凶幻 ////////////////////////////////////////////////////
			mustReturnNewBlock = 0;
			*downPosition = 0;
			*leftAndRightPosition = 4;
		}
		
		/* 紫遂切 繕拙 */
		// TODO: SPACE 郊 嬢胸惟 脊径?
		if (_kbhit()) {
			if (_getch() != 0xE0) {}
			userInputKeyCode = _getch();
			
			mustReturnNewBlock = processUserInput(userInputKeyCode, gameBoard, fallingBlock, downPosition, leftAndRightPosition);
		}
		
		/* 陥獣 益軒奄 */
		printBoard(gameBoard);

		/* if => 鷺薫 郊韓拭 亀鐸 */
		if (mustReturnNewBlock == 1) {
			memcpy(tempBoard, gameBoard, sizeof(tempBoard));

			for (i = 0; i < HEIGHT-1; i++) {
				/* 廃 匝 走随走 源走 端滴 */
				if (oneLineFilled(tempBoard[i]) == 1) {
					for (j = 0; (i - j) > 0 ; j++) {
						memcpy(tempBoard[i - j], gameBoard[i - j - 1], sizeof(int) * WIDTH);
					}
					memset(tempBoard[0], 0, sizeof(int) * WIDTH);
					tempBoard[0][0] = 1;
					tempBoard[0][11] = 1;
					memcpy(gameBoard, tempBoard, sizeof(tempBoard));
					printBoard(gameBoard);
				}
			}
		}
		
		userInputKeyCode = -1;
	}

	printf("\n************\n");
	return;
}

/* main function */
int main(void) {

	int i = 0, j = 0;
	
	functionForTest();

	//int* leftAndRightPosition = 0;
	//int* downPosition = 0;

	//int gameBoard[HEIGHT][WIDTH] = { 0, };
	//while (1) {
	//	
	//	printBoard(gameBoard);

	//	/* 鷺薫戚 鎧形身 */


	//	/* 紫遂切 繕拙 */

	//	/* 陥獣 益軒奄 */

	//	/* if => 鷺薫 郊韓拭 亀鐸 */

	//	/* 廃 匝 走随走 源走 端滴 */
	//	//for (i = 0; i < HEIGHT; i++) {
	//	//	if (oneLineFilled(gameBoard[i])) {
	//	//	}
	//	//	else {
	//	//	}
	//	//}
	//}
	


	return 0;
}