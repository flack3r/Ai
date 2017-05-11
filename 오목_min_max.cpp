#include "stdafx.h"
/*
��Ʒ��� �������� �����ϼ���.
���и�: ��ϴ��б�
��  ��: �����ǽ�
��  ��: �迵��(IT���� ��ǻ���к� 4�г�)
������: 2017.5.10

���ۼ� �� ���ǻ���
- C++�⺻ ������ ���Ͽ� �ۼ�
- �ܺ� ���̺귯�� ��� ���� (dll, STL��)
- for�� �ε��� ������ �ܺο� �̸� ������ �� (�Ʒ��� ���� ���� �� ��)
for (int i = 0; i < 10...
- ���� ���� global����, �Լ�, class�� ���� ����, �� �ٸ� ���� �ߺ����� �ʵ��� ���
- ����� �� ������ �Լ� f201701001()�� �ش� ����ȣ�� �����ϰ� "����ȣ.cpp"�� �����Ͽ� ����
*/

// ��� ����
#define MAXXY 16
#define BLACK 1
#define WHITE 0
#define EMPTY -1
#define OUTBD -2
#define NULL 0
// B�Լ��� �������� �ش� ��ġ�� ��Ȳ�� ����� �� ��, ����ϴ� �Լ��� ������ BLACK, WHITE, EMPTY, OUTBD�� �� ���� ������
// B(-3,6)�� ������ �ٱ��� �����ϹǷ� OUTBD(-2)�� ���ϰ����� ����
// B(-3,6,BLACK)�� ������ ���� �����ϳ�, ���ϰ��� def������ �Ѱ��� BLACK�� ���ϰ����� ����
extern int B(int x, int y, int def = OUTBD);

int board[17][17];
int Aicolor = 0;
int bestMove(int(&result)[2], int maximizing, int depth, int alpha, int beta, int color);
int DecideScore(int consecutive, int openEnd, int maximizing); // �޸���ƽ�� ���� ���� ����
int AnalyzeBoard(int maximizing, int color);
int AnalyzeRow(int maximizing, int color);
int AnalyzeCol(int maximizing, int color);
int AnalyzeVerticalOne(int maximizing, int color);
int AnalyzeVerticalTwo(int maximizing, int color);

FILE* f1;
// AI �ۼ���, B(int x,int y,int)�Լ��� ���� �Ǽ��� �м��ϰ� ���� �� ��ġ ���� ���� *NewX, *NewY�� ���� ȣ�� �Լ��� �ǵ�����.
// ������ ���� ���� ���η� ���� 2�� �ְ� �� �������� ����ִ� ���� ã�� ���� �δ� AI�� ���̴�.
//
// NewX,NewY : AI�� ���� ���� �ΰ��� �ϴ� ���� ���� ���� ������ ����
// mc : AI�� �� ���� �� (BLACK �Ǵ� WHITE)
// CurTur�� : ���� ����� ��
// 4�� ���������� ���� ������ ����, COmDevDlg::IsGameOver() �Լ��� ���� ����

void f201701001(int *NewX, int *NewY, int mc, int CurTurn)
{
	f1 = fopen("log.txt", "w");
	int result[2] = { 0, };
	int a = 0;
	Aicolor = mc;
	int ec = mc == WHITE ? BLACK : WHITE;	//	���� �� Ȯ��
	*NewX = rand() % MAXXY;
	*NewY = rand() % MAXXY;					//	�ϴ� �ƹ����� ��

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
//maximizing -> max, min �Ǻ�
//result[0] -> bestX, result[1] -> bestY
//color -> ���� �� �÷�
int bestMove(int(&result)[2], int maximizing, int depth, int alpha, int beta, int color)
{
	if (depth == 0)
	{
		//logMep();
		int ec = color == WHITE ? BLACK : WHITE;
		int tmp_score = AnalyzeBoard(maximizing, ec); //���� Į��
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

//color -> �ش� ���� �÷�
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
			//���� ���� �������
			else if (consecutive > 0)
			{
				score += DecideScore(consecutive, openEnd, color == Aicolor);
				consecutive = 0;
				openEnd = 0;
			}
			else openEnd = 0;
		}
		//���� row ������ �־��� ���.
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
			//���� ���� �������
			else if (consecutive > 0)
			{
				score += DecideScore(consecutive, openEnd, color == Aicolor);
				consecutive = 0;
				openEnd = 0;
			}
			else openEnd = 0;
		}
		//���� row ������ �־��� ���.
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
				//���� ���� �������
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
		//���� row ������ �־��� ���.
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
				//���� ���� �������
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
		//���� row ������ �־��� ���.
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
