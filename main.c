#include<stdio.h>
#include<stdlib.h>


#define ArrSize(x)  (sizeof(x) / sizeof((x)[0]))


#pragma region WindowsOnlyFunctions
#include<windows.h>
void gotoxy(int x, int y)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD) { x, y });
}

char getch()
{
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	if (handle == NULL) return 0;

	DWORD param;
	INPUT_RECORD inrec;
	while (1)
	{
		ReadConsoleInputA(handle, &inrec, 1, &param);
		if (inrec.EventType == KEY_EVENT && inrec.Event.KeyEvent.bKeyDown)
			return (char)inrec.Event.KeyEvent.uChar.UnicodeChar;
	}
}
#pragma endregion 

int _exp(int base, int actual, int power)
{
	return power == 1 ? actual : _exp(base, actual * base, power - 1);
}
#define exp(x,y) _exp(x,x,y);

typedef struct CELL_t
{
	int x, y, width, height;
	char label, wparam;
}Cell;

Cell CreateCellBlank(int x, int y, int w, int h)
{
	Cell c;
	c.x = x;
	c.y = y;
	c.width = w;
	c.height = h;
	c.label = ' ';
	c.wparam = ' ';
	return c;
}
Cell CreateCellCustom(int x, int y, int w, int h, char label)
{
	Cell c;
	c.x = x;
	c.y = y;
	c.width = w;
	c.height = h;
	c.label = label;
	c.wparam = ' ';
	return c;
}

typedef struct GRID_t
{
	int offx, offy,width,height;
	Cell* cells;
}Grid;

Grid CreateGrid(int x, int y, int w, int h)
{
	Grid g;
	g.offx = x;
	g.offy = y;
	g.width = w;
	g.height = h;
	g.cells = calloc(w * h, sizeof(Cell));
	for (int y = 0; y < g.height; y++)
	{
		for (int x = 0; x < g.width; x++)
		{
			g.cells[x + (y * g.width)] = CreateCellBlank(x, y,4,1);
			
		}
	}
	return g;
}

Grid CreateGridForTable(int x, int y, int w, int h, int* table)
{
	Grid g;
	g.offx = x;
	g.offy = y;
	g.width = w;
	g.height = h ;
	g.cells = calloc(g.width*g.height , sizeof(Cell));
	for (int y = 0; y < g.height; y++)
	{
		for (int x = 0; x < g.width; x++)
		{
			int index = x + (y * g.width);
			char c = table[index] == 1 ? 'v' : 'f';
			g.cells[index] = CreateCellCustom(x, y, 4, 1, c);

		}
	}
	return g;
}

Grid CreateGridPreset(int x, int y, int w, int h, char* c)
{
	Grid g;
	g.offx = x;
	g.offy = y;
	g.width = w;
	g.height = h;
	g.cells = calloc(g.width * g.height, sizeof(Cell));
	for (int y = 0; y < g.height; y++)
	{
		for (int x = 0; x < g.width; x++)
		{
			int index = x + (y * g.width);

			g.cells[index] = CreateCellCustom(x, y, 4, 1, c[index]);

		}
	}
	return g;
}
Grid CreateGridFill(int x, int y, int w, int h, char c)
{
	Grid g;
	g.offx = x;
	g.offy = y;
	g.width = w;
	g.height = h;
	g.cells = calloc(g.width * g.height, sizeof(Cell));
	for (int y = 0; y < g.height; y++)
	{
		for (int x = 0; x < g.width; x++)
		{
			int index = x + (y * g.width);

			g.cells[index] = CreateCellCustom(x, y, 4, 1, c);

		}
	}
	return g;
}


void DrawGrid(Grid g)
{
	for(int y = 0; y < g.height; y++)
	{
		for(int x = 0; x < g.width; x++)
		{
			Cell c = g.cells[x + (y * g.width)];
			gotoxy(c.x*c.width + g.offx+1, c.y*c.height + g.offy+1);
			printf(" %c%c ", c.wparam, c.label);
		}
	}
	gotoxy(0, 0);
}








int* GenerateBaseTable(int arr_size)
{
	const int totalLines = 2 << arr_size - 1;
	int* results = calloc(arr_size * totalLines, sizeof(int));
	for (int x = 0; x < arr_size; x++)
	{		
		int interval = totalLines >> x + 1;
		int b = 1;
		for (int y = 0; y < totalLines; y++)
		{
			if(interval <= 0)
			{
				b = !b;
				interval = totalLines >> x + 1;
			}
			results[x + y * arr_size] = b;
			interval--;
			
		}
	
	}
	return results;
}

void ConstructTable(int _x, int _y, char* elements, size_t nElements)
{
	int* table = GenerateBaseTable(nElements);

	int x = _x;
	int y = _y;
	int w = nElements*4;
	int h = exp(2, nElements);
	
	Grid gHeader = CreateGridPreset(x, y, nElements, 1, elements);
	Grid gTable = CreateGridForTable(x, y+1, nElements, h, table);
	Grid gSeparator = CreateGridFill(x + w, y, 1, h+1, '|');
	DrawGrid(gHeader);
	DrawGrid(gTable);
	DrawGrid(gSeparator);
}

int main()
{
	

	char vars[5];
	vars[0] = 'P';
	vars[1] = 'Q';
	vars[2] = 'R';
	vars[3] = 'S';
	vars[4] = 'T';

	ConstructTable(2,2,vars,ArrSize(vars));

	gotoxy(0, 0);

	while (1) getch();
}

