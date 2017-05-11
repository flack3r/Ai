#include "stdafx.h"
/*
■아래에 팀정보를 기재하세요.
대학명: 경북대학교
팀  명: 오목의신
팀  원: 김영진(IT대학 컴퓨터학부 4학년)
제출일: 2017.5.10

■작성 시 주의사항
- C++기본 문법에 의하여 작성
- 외부 라이브러리 사용 금지 (dll, STL등)
- for문 인덱스 변수는 외부에 미리 선언할 것 (아래와 같이 하지 말 것)
for (int i = 0; i < 10...
- 파일 내에 global변수, 함수, class등 선언 가능, 단 다른 팀과 중복되지 않도록 명명
- 제출시 본 파일의 함수 f201701001()를 해당 팀번호로 수정하고 "팀번호.cpp"로 저장하여 제출
*/

// 상수 지정
#define MAXXY 16
#define BLACK 1
#define WHITE 0
#define EMPTY -1
#define OUTBD -2
#define NULL 0
// B함수는 오목판의 해당 위치의 상황을 얻고자 할 때, 사용하는 함수로 리턴은 BLACK, WHITE, EMPTY, OUTBD중 한 값을 리턴함
// B(-3,6)는 오목판 바깥을 지시하므로 OUTBD(-2)를 리턴값으로 얻음
// B(-3,6,BLACK)는 오목판 밖을 지시하나, 리턴값은 def값으로 넘겨준 BLACK를 리턴값으로 얻음
extern int B(int x, int y, int def = OUTBD);

int board[17][17];
int Aicolor = 0;
int bestMove(int(&result)[2], int maximizing, int depth, int alpha, int beta, int color);
int DecideScore(int consecutive, int openEnd, int maximizing); // 휴리스틱을 위한 점수 리턴
int AnalyzeBoard(int maximizing, int color);
int AnalyzeRow(int maximizing, int color);
int AnalyzeCol(int maximizing, int color);
int AnalyzeVerticalOne(int maximizing, int color);
int AnalyzeVerticalTwo(int maximizing, int color);

FILE* f1;
// AI 작성시, B(int x,int y,int)함수를 통해 판세를 분석하고 알을 둘 위치 값을 인자 *NewX, *NewY를 통해 호출 함수로 되돌린다.
// 다음은 적의 알이 가로로 연속 2개 있고 그 오른쪽이 비어있는 곳을 찾아 알을 두는 AI의 예이다.
//
// NewX,NewY : AI에 의해 알을 두고자 하는 값에 대한 리턴 포인터 변수
// mc : AI가 둘 알의 색 (BLACK 또는 WHITE)
// CurTurㅜ : 현재 진행된 수
// 4개 방향으로의 전술 적용을 위해, COmDevDlg::IsGameOver() 함수의 구현 참조

void f201701001(int *NewX, int *NewY, int mc, int CurTurn)
{
	f1 = fopen("log.txt", "w");
	int result[2] = { 0, };
	int a = 0;
	Aicolor = mc;
	int ec = mc == WHITE ? BLACK : WHITE;	//	적의 색 확인
	*NewX = rand() % MAXXY;
	*NewY = rand() % MAXXY;					//	일단 아무데나 둠

	int iy, ix;
	int color = 0;

	//initial board
	for (iy = 0; iy < MAXXY; iy++)
	{
		for (ix = 0; ix < MAXXY; ix++)
		{
			color = B(ix, iy);
			if (color != OUTBD)
				board[iy][ix] = color;
		}
	}

	bestMove(result, 1, 3, INT_MIN, INT_MAX, mc);
	*NewX = result[0];
	*NewY = result[1];
	fclose(f1);
	return;
}

void logMep()
{
	int ix, iy;
	fprintf(f1, "=============\n");
	for (iy = 0; iy < MAXXY; iy++)
	{
		for (ix = 0; ix < MAXXY; ix++)
		{
			if (board[iy][ix] == EMPTY)
				fprintf(f1, "-");
			else if (board[iy][ix] == BLACK)
				fprintf(f1, "*");
			else if (board[iy][ix] == WHITE)
				fprintf(f1, "0");
		}
		fprintf(f1, "\n");
	}
}
//maximizing -> max, min 판별
//result[0] -> bestX, result[1] -> bestY
//color -> 돌을 둘 컬러
int bestMove(int(&result)[2], int maximizing, int depth, int alpha, int beta, int color)
{
	if (depth == 0)
	{
		//logMep();
		int ec = color == WHITE ? BLACK : WHITE;
		int tmp_score = AnalyzeBoard(maximizing, ec); //이전 칼라
		//fprintf(f1, "score: %d\n", tmp_score);
		return tmp_score;
	}

	int ix, iy;
	int temp;
	int op_color = color == WHITE ? BLACK : WHITE;
	int response[2];
	int prone = 0;
	if (maximizing)
	{
		for (iy = 0; iy < MAXXY; iy++)
		{
			for (ix = 0; ix < MAXXY; ix++)
			{
				if (board[iy][ix] == EMPTY)
				{
					board[iy][ix] = color;
					temp = bestMove(response, !maximizing, depth - 1, alpha, beta, op_color);
					if (temp > alpha)
					{
						alpha = temp;
						result[0] = ix;
						result[1] = iy;
					}
					board[iy][ix] = EMPTY;
					if (beta <= alpha)
					{
						prone = 1;
						break;
					}
				}
			}
			if (prone)
				break;
		}
		return alpha;
	}
	else
	{
		for (iy = 0; iy < MAXXY; iy++)
		{
			for (ix = 0; ix < MAXXY; ix++)
			{
				if (board[iy][ix] == EMPTY)
				{
					board[iy][ix] = color;
					temp = bestMove(response, !maximizing, depth - 1, alpha, beta, op_color);
					if (temp < beta)
					{
						beta = temp;
						result[0] = ix;
						result[1] = iy;
					}
					board[iy][ix] = EMPTY;
					if (beta <= alpha)
					{
						prone = 1;
						break;
					}
				}
			}
			if (prone)
				break;
		}
		return beta;
	}
}

//color -> 해당 턴의 컬러
int AnalyzeBoard(int maximizing, int color)
{
	int score = 0;
	score += AnalyzeRow(maximizing, color);
	score += AnalyzeCol(maximizing, color);
	score += AnalyzeVerticalOne(maximizing, color);
	score += AnalyzeVerticalTwo(maximizing, color);
	return score;
}

int AnalyzeRow(int maximizing, int color)
{
	int consecutive = 0, openEnd = 0;
	int dx, dy;
	int score = 0;
	for (dy = 0; dy < MAXXY; dy++)
	{
		for (dx = 0; dx < MAXXY; dx++)
		{
			if (board[dy][dx] == color)
				consecutive += 1;
			else if (board[dy][dx] == EMPTY && consecutive > 0)
			{
				openEnd += 1;
				score += DecideScore(consecutive, openEnd, color == Aicolor);
				consecutive = 0;
				openEnd = 1;
			}
			else if (board[dx][dx] == EMPTY)
				openEnd = 1;
			//상대방 돌을 만날경우
			else if (consecutive > 0)
			{
				score += DecideScore(consecutive, openEnd, color == Aicolor);
				consecutive = 0;
				openEnd = 0;
			}
			else openEnd = 0;
		}
		//돌이 row 끝까지 있었을 경우.
		if (consecutive > 0)
		{
			score += DecideScore(consecutive, openEnd, color == Aicolor);
		}
		consecutive = 0;
		openEnd = 0;
	}
	return score;
}

int AnalyzeCol(int maximizing, int color)
{
	int consecutive = 0, openEnd = 0;
	int dy, dx;
	int score = 0;
	for (dx = 0; dx < MAXXY; dx++)
	{
		for (dy = 0; dy < MAXXY; dy++)
		{
			if (board[dy][dx] == color)
				consecutive += 1;
			else if (board[dy][dx] == EMPTY && consecutive > 0)
			{
				openEnd += 1;
				score += DecideScore(consecutive, openEnd, color == Aicolor);
				consecutive = 0;
				openEnd = 1;
			}
			else if (board[dx][dx] == EMPTY)
				openEnd = 1;
			//상대방 돌을 만날경우
			else if (consecutive > 0)
			{
				score += DecideScore(consecutive, openEnd, color == Aicolor);
				consecutive = 0;
				openEnd = 0;
			}
			else openEnd = 0;
		}
		//돌이 row 끝까지 있었을 경우.
		if (consecutive > 0)
		{
			score += DecideScore(consecutive, openEnd, color == Aicolor);
		}
		consecutive = 0;
		openEnd = 0;
	}
	return score;
}

int AnalyzeVerticalOne(int maximizing, int color)
{
	int consecutive = 0, openEnd = 0;
	int diagonal, dx, dy, score = 0;
	for (diagonal = -MAXXY + 1; diagonal < MAXXY; diagonal++)
	{
		for (dx = 0; dx < MAXXY; dx++)
		{
			dy = -dx + diagonal;
			if (dx >= 0 && dx < MAXXY && dy >= 0 && dy < MAXXY)
			{
				if (board[dy][dx] == color)
					consecutive += 1;
				else if (board[dy][dx] == EMPTY && consecutive > 0)
				{
					openEnd += 1;
					score += DecideScore(consecutive, openEnd, color == Aicolor);
					consecutive = 0;
					openEnd = 1;
				}
				else if (board[dx][dx] == EMPTY)
					openEnd = 1;
				//상대방 돌을 만날경우
				else if (consecutive > 0)
				{
					score += DecideScore(consecutive, openEnd, color == Aicolor);
					consecutive = 0;
					openEnd = 0;
				}
				else openEnd = 0;
			}
			else continue;
		}
		//돌이 row 끝까지 있었을 경우.
		if (consecutive > 0)
		{
			score += DecideScore(consecutive, openEnd, color == Aicolor);
		}
		consecutive = 0;
		openEnd = 0;
	}
	return score;
}

int AnalyzeVerticalTwo(int maximizing, int color)
{
	int consecutive = 0, openEnd = 0;
	int diagonal, dx, dy, score = 0;
	for (diagonal = 0; diagonal > (MAXXY - 1) * -2; diagonal--)
	{
		for (dx = 0; dx < MAXXY; dx++)
		{
			dy = dx + diagonal;
			if (dx >= 0 && dx < MAXXY && dy >= 0 && dy < MAXXY)
			{
				if (board[dy][dx] == color)
					consecutive += 1;
				else if (board[dy][dx] == EMPTY && consecutive > 0)
				{
					openEnd += 1;
					score += DecideScore(consecutive, openEnd, color == Aicolor);
					consecutive = 0;
					openEnd = 1;
				}
				else if (board[dx][dx] == EMPTY)
					openEnd = 1;
				//상대방 돌을 만날경우
				else if (consecutive > 0)
				{
					score += DecideScore(consecutive, openEnd, color == Aicolor);
					consecutive = 0;
					openEnd = 0;
				}
				else openEnd = 0;
			}
			else continue;
		}
		//돌이 row 끝까지 있었을 경우.
		if (consecutive > 0)
		{
			score += DecideScore(consecutive, openEnd, color == Aicolor);
		}
		consecutive = 0;
		openEnd = 0;
	}
	return score;
}

int DecideScore(int consecutive, int openEnd, int current_turn)
{
	if (openEnd == 0 && consecutive < 5)
		return 0;
	switch (consecutive)
	{
	case 4:
		switch (openEnd)
		{
		case 1:
			if (current_turn)
				return 100000000;
			return 1;
		case 2:
			if (current_turn)
				return 100000000;
			return 1;
		}
	case 3:
		switch (openEnd)
		{
		case 1:
			if (current_turn)
				return 7;
			return 1000;
		case 2:
			if (current_turn)
				return 10000;
			return 10;
		}
	case 2:
		switch (openEnd)
		{
		case 1:
			if (current_turn)
				return 20;
			return 1000;
		case 2:
			if (current_turn)
				return 40;
			return 500;
		}
	case 1:
		switch (openEnd)
		{
		case 1:
			if(current_turn)
				return 1;
			return 2000;
		case 2:
			if (current_turn)
				return 2;
			return 1800;
		}
	default:
		return 200000000;
	}
	return 0;
}
