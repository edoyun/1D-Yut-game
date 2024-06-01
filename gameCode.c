#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//게임 설정 최소,최대값 지정
#define MIN_BOARD_SIZE 10
#define MAX_BOARD_SIZE 20
#define MIN_HORSE_SIZE 1
#define MAX_HORSE_SIZE 5
#define MIN_PLAYER_SIZE 2
#define MAX_PLAYER_SIZE 10

//게임 설정 함수
void gameSetup(int* boardSize, int* horseCount, int* playerCount, int playerHorse[], int boardPlayer[], int boardHorse[], int goalStatus[]) {
	printf("1차원 윷놀이 게임\n게임 설정을 시작합니다.\n");
	// 플레이어 수 입력, 입력 확인
	do {
		printf("플레이어 수를 입력해주세요. (최소 %d, 최대 %d): ", MIN_PLAYER_SIZE, MAX_PLAYER_SIZE);
		scanf_s("%d", playerCount);
		if (*playerCount >= MIN_PLAYER_SIZE && *playerCount <= MAX_PLAYER_SIZE) {
			break;
		}
		else {
			printf("입력값 %d 가 허용된 범위가 아닙니다. 다시 입력해주세요.\n\n", *playerCount);
		}
	} while (1);
	// 보드 크기 입력, 입력 확인
	do {
		printf("보드 사이즈를 입력해주세요. (최소 %d, 최대 %d): ", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
		scanf_s("%d", boardSize);
		if (*boardSize >= MIN_BOARD_SIZE && *boardSize <= MAX_BOARD_SIZE) { //보드 초기화 플레이어는 -1 말은 0 (플레이어 번호가 0부터 있으므로 -1로 처리)
			for (int loop = 0; loop < *boardSize; loop++) {
				boardPlayer[loop] = -1;
				boardHorse[loop] = 0;
			}
			break;
		}
		else {
			printf("입력값 %d 가 허용된 범위가 아닙니다. 다시 입력해주세요.\n\n", *boardSize);
		}
	} while (1);
	// 말 갯수 입력 ,입력 확인
	do {
		printf("말 갯수를 입력해주세요. (최소 %d, 최대 %d): ", MIN_HORSE_SIZE, MAX_HORSE_SIZE);
		scanf_s("%d", horseCount);
		if (*horseCount >= MIN_HORSE_SIZE && *horseCount <= MAX_HORSE_SIZE) {
			for (int loop = 0; loop < *playerCount; loop++) { //골인 여부, 플레이어 보유 말 수 초기화
				goalStatus[loop] = 0;
				playerHorse[loop] = *horseCount;
			}
			break;
		}
		else {
			printf("입력값 %d 가 허용된 범위가 아닙니다. 다시 입력해주세요.\n\n", *horseCount);
		}
	} while (1);
	printf("게임 설정이 완료되었습니다.\n\n");
}
//말 옮기는 함수
void moveHorse(int mode, int playerHorse[], int boardPlayer[], int boardHorse[], int playerIndex, int playerInput, int moveDistance, int boardSize) {
	int targetIndex; //이동 후 보드 위치
	if (mode == 0) { //말 새로 보내는 경우
		targetIndex = moveDistance - 1; 
		playerHorse[playerIndex] -= 1;
		if (boardPlayer[targetIndex] != -1) { //대상 칸에 누가 있을때
			if (boardPlayer[targetIndex] != playerIndex) { //상대 말 잡았을때
				printf("플레이어 %d가 플레이어 %d의 말 %d개를 잡았습니다!\n", playerIndex + 1, boardPlayer[targetIndex] + 1, boardHorse[targetIndex]);
				playerHorse[boardPlayer[targetIndex]] += boardHorse[targetIndex]; //잡힌 말 회수
				boardHorse[targetIndex] = 1;
			}
			else {//말 업었을때
				boardHorse[targetIndex] += 1;
				printf("말을 업었습니다. %d마리가 함께 움직입니다.", boardHorse[targetIndex]);
			}
		}
		else { //빈 칸 일때
			boardHorse[targetIndex] = 1;
		}
		boardPlayer[targetIndex] = playerIndex;
	}
	else { //보드에 있는 말 옮길 때
		targetIndex = playerInput + moveDistance; //현재 위치 + 이동 거리
		if (targetIndex < 0) { //뺵도 결과가 출발점 전일때
			printf("출발점으로 돌아갔습니다. 말을 회수합니다.\n");
			playerHorse[playerIndex] += boardHorse[playerInput]; //말 회수
		}
		else if (targetIndex >= boardSize) { //범위 넘어간 경우(골인)
			printf("\x1b[31m플레이어%d 말 %d개를 골인 했습니다!\x1b[0m\n",playerIndex+1,boardHorse[playerInput]); //골인 메시지 출력(색깔 강조)
		}
		else {
			if (boardPlayer[targetIndex] != -1) { //대상 칸에 누가 있을때
				if (boardPlayer[targetIndex] != playerIndex) { //상대 말 잡았을때
					int catchPlayerIndex = boardPlayer[targetIndex];
					printf("플레이어 %d가 플레이어 %d의 말 %d개를 잡았습니다!\n", playerIndex + 1, catchPlayerIndex + 1, boardHorse[targetIndex]);
					playerHorse[catchPlayerIndex] += boardHorse[targetIndex]; //잡힌 말 회수
					boardHorse[targetIndex] = boardHorse[playerInput];
				}
				else {//말 업었을때
					boardHorse[targetIndex] += boardHorse[playerInput];
					printf("말을 업었습니다. %d마리가 함께 움직입니다.", boardHorse[targetIndex]);
				}
				boardPlayer[targetIndex] = playerIndex;
			}
			else { //빈 칸 일때
				boardHorse[targetIndex] = boardHorse[playerInput];
				boardPlayer[targetIndex] = playerIndex;
			}
		}
		//옮기기 전 보드 칸 초기화
		boardPlayer[playerInput] = -1;
		boardHorse[playerInput] = 0; 
	}
}
//플레이어 골인 여부 확인 함수
void checkGoal(int playerIndex, int boardSize, int boardPlayer[], int playerHorse[], int goalStatus[]) {
	int leftHorseCount = playerHorse[playerIndex]; //남은 말 수 확인(출발 전 말 수로 초기화)
	for (int loop = 0; loop < boardSize; loop++) { //보드 칸에 플레이어의 말 있는지 확인
		if (boardPlayer[loop] == playerIndex) {
			leftHorseCount++;
		}
	}
	if (leftHorseCount == 0) { //남은 말 수가 없을 때
		goalStatus[playerIndex] = 1;
		printf("플레이어 %d의 모든 말이 골인했습니다.\n", playerIndex + 1);
	}
}

int main() {
	srand(time(NULL));
	int boardSize, horseCount, playerCount; //게임 설정 변수
	int playerHorse[MAX_PLAYER_SIZE], boardPlayer[MAX_BOARD_SIZE], boardHorse[MAX_BOARD_SIZE], goalStatus[MAX_PLAYER_SIZE]; //플레이어 말 보유수, 보드 칸에 있는 플레이어 번호, 보드 칸에 있는 말 수, 플레이어별 골인 여부
	int gameEnd = 0; //게임 끝 여부
	char* toKorean[6] = { "빽도","도","개","걸","윷","모" }; //윷 결과 한글 리스트
	int distance[6] = {-1, 1, 2, 3, 4, 5}; //윷 결과 이동 거리 리스트
	//게임 설정
	gameSetup(&boardSize, &horseCount, &playerCount, playerHorse, boardPlayer, boardHorse, goalStatus);
	//게임 시작
	int index = 0;
	int playerIndex, playerInput; //현재 차례인 플레이어 번호, 보낼 말 번호 입력
	system("cls"); //창 클리어
	printf("게임 설정이 완료되었습니다.\n플레이어 수 : %d, 보드 칸 수 : %d, 말 갯수 : %d\n게임을 시작합니다.\n\n",playerCount,boardSize,horseCount); //게임 설정 결과 출력
	do { //게임 끝날때 까지 반복
		int random = rand() % 6;
		playerIndex = index % playerCount;
		printf("\n============================================================================================================\n");
		if (goalStatus[playerIndex]==1) { //골인시 차례 건너뜀
			printf("플레이어 %d는 골인하였으므로 차례를 건너뜁니다.", playerIndex + 1);
			index++;
			continue;
		}
		else if (random == 0 && playerHorse[playerIndex] == horseCount) { //보낼 말 없고 빽도 나왔을 때 차례 건너뜀
			printf("빽도가 나왔습니다. 움직일 말이 없습니다. 차례를 건너뜁니다.\n\n");
			index++;
			continue;
		}
		else {
			printf("플레이어 %d \n보유 말 갯수(출발전) : %d\n윷 굴린 결과: %s(%d)\n", playerIndex + 1, playerHorse[playerIndex], toKorean[random], distance[random]);
			//보드 출력
			printf("보드\n");
			for (int loop1 = 0; loop1 < boardSize; loop1++) {
				printf("   %2d   |", loop1 + 1);
			}
			printf("\n");
			for (int loop2 = 0; loop2 < boardSize; loop2++) {
				if (boardPlayer[loop2] == -1) {
					printf("        |");
				}
				else {
					if (boardPlayer[loop2] == playerIndex) {
						printf("\x1b[32mP%2d : %2d\x1b[0m|", boardPlayer[loop2] + 1, boardHorse[loop2]); //현재 차례 플레이어의 말 위치 색깔 강조
					}
					else {
						printf("P%2d : %2d|", boardPlayer[loop2] + 1, boardHorse[loop2]);
					}
				}
			}
			//보드 출력 끝
			printf("\n움직일 말을 선택해주세요. 0(새로 보낼 말),n(움직일 말의 위치) : ");
			do {
				scanf_s("%d", &playerInput);
				playerInput--;
				if (playerInput == -1) { //입력 0(새로 출발)일때
					if (playerHorse[playerIndex] == 0) { //새로 보낼 말이 없을때
						printf("새로 보낼 말이 없습니다. 다시 입력해주세요.\n");
					}
					else if (random == 0) { //빽도 일때
						printf("빽도는 말을 새로 보낼 수 없습니다. 다시 입력해주세요.\n");
					}
					else {
						moveHorse(0, playerHorse, boardPlayer, boardHorse, playerIndex, playerInput, distance[random], boardSize); //말 이동
						break;
					}
				}
				else if (boardPlayer[playerInput] == playerIndex) { //보드에 있는 말 선택 했을때
					moveHorse(1, playerHorse, boardPlayer, boardHorse, playerIndex, playerInput, distance[random], boardSize); //말 이동
					break;
				}
				else { //입력값 오류 처리
					printf("입력이 잘못 되었습니다. 다시 입력해주세요.\n");
				}
			} while (1);
			checkGoal(playerIndex, boardSize, boardPlayer,playerHorse,goalStatus); //말 옮긴 후 플레이어 골 여부 체크
		}
		//게임 종료 여부 확인
		int leftPlayerCount=0; //남은 플레이어
		for (int loop = 0; loop < playerCount; loop++) {
			if (goalStatus[loop] == 0) { //골 안했을 경우
				leftPlayerCount++;
			}
		}
		if (leftPlayerCount == 1) { //한 명 남았을 때
			gameEnd = 1;
		}
		index++;
	} while (gameEnd == 0);
}
