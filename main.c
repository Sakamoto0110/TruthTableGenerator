#include<stdio.h>
#include<stdlib.h>


#define ArrSize(x)  (sizeof(x) / sizeof((x)[0]))
int _exp(int base, int actual, int power)
{
	return power == 1 ? actual : _exp(base, actual * base, power - 1);
}
#define exp(x,y) _exp(x,x,y);

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
			return (char)inrec.Event.KeyEvent.wVirtualKeyCode;
	}
}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
#pragma endregion 


#define CELLWIDTH 5
#define CELLHEIGHT 1
typedef struct CELL_t
{
	int x, y, width, height;
	char label;
	char rparam, rparamW;
	char lparam, lparamW;
	int value;
}Cell;

Cell CreateCellBlank(int x, int y, int w, int h)
{
	Cell c;
	c.value = 0;
	c.x = x;
	c.y = y;
	c.width = w;
	c.height = h;
	c.label = ' ';
	c.lparam  = ' ';
	c.lparamW = ' ';
	c.rparam  = ' ';
	c.rparamW = ' ';
	return c;
}
Cell CreateCellCustom(int x, int y, int w, int h, char label)
{
	Cell c;
	c.value = 0;
	c.x = x;
	c.y = y;
	c.width = w;
	c.height = h;
	c.label = ' ';
	c.lparam = ' ';
	c.lparamW = ' ';
	c.rparam = ' ';
	c.rparamW = ' ';
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
			g.cells[x + (y * g.width)] = CreateCellBlank(x, y, CELLWIDTH,CELLHEIGHT);
			
		}
	}
	return g;
}


Grid CreateGridForTable(int x, int y, int w, int h, int* table)
{
	Grid g = CreateGrid(x,y,w,h);
	
	for(int i = 0; i < g.width*g.height; i++)
	{
		g.cells[i].label = table[i] == 1 ? 'v' : 'f';
	}
	
	return g;
}

Grid CreateGridPreset(int x, int y, int w, int h, char* c)
{
	Grid g = CreateGrid(x, y, w, h);

	for (int i = 0; i < g.width * g.height; i++)
	{
		g.cells[i].label = c[i];
	}

	return g;
}
Grid CreateGridFill(int x, int y, int w, int h, char c)
{
	Grid g = CreateGrid(x, y, w, h);

	for (int i = 0; i < g.width * g.height; i++)
	{
		g.cells[i].label = c;
	}

	return g;
}

void EreaseGrid(Grid g)
{
	for (int y = 0; y < g.height; y++)
	{
		for (int x = 0; x < g.width; x++)
		{
			Cell c = g.cells[x + (y * g.width)];
			//gotoxy(c.x*(c.width) + g.offx+2, c.y*c.height + g.offy+1);
			gotoxy(x * (c.width) + g.offx + (x * 1), y * c.height + g.offy + 1);
			for (int i = 0; i < 10; i++)
			{
				printf("               \n");
			}
			gotoxy(x * (c.width) + g.offx + (x * 1), y * c.height + g.offy + 1);
			switch (CELLWIDTH)
			{
			case 1:
				printf(" ");
				break;
			case 3:
				printf("   ");
				break;
			case 5:
				printf("     ");
				break;
			default:
				printf(" . ");
				break;
			}

		}
	}
	gotoxy(0, 0);
}

void DrawGrid(Grid g)
{

	
	
	for(int y = 0; y < g.height; y++)
	{
		for(int x = 0; x < g.width; x++)
		{
			Cell c = g.cells[x + (y * g.width)];
			//gotoxy(c.x*(c.width) + g.offx+2, c.y*c.height + g.offy+1);
			gotoxy(x * (c.width) + g.offx + (x * 1), y * c.height + g.offy + 1);
			for(int i = 0; i < 10; i++)
			{
				printf("               \n");
			}
			gotoxy(x * (c.width) + g.offx + (x*1), y * c.height + g.offy + 1);
			switch(CELLWIDTH)
			{
			case 1:
				printf("%c", c.label);
				break;
			case 3:
				printf("%c%c%c", c.lparam, c.label, c.rparam);
				break;
			case 5:
				printf("%c%c%c%c%c", c.lparamW, c.lparam, c.label, c.rparam, c.rparamW);
				break;
			default:
				printf(" . ");
				break;
			}
			
		}
	}
	gotoxy(0, 0);
}



void MakeExprCellW(Cell* c, char c1, char c2,char c3, char c4, char c5)
{
	c->lparamW = c1;
	c->lparam = c2;
	c->label = c3;
	c->rparamW = c4;
	c->rparam = c5;
}

void MakeExprCell(Cell* c, char c1, char c2, char c3)
{
	c->lparam = c1;
	c->label = c2;
	c->rparam = c3;
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



typedef struct OPERATIONDATA_t
{
	int* sourceA;
	int* sourceB;
	char opChar;
	int offset;
	int size;
	int(*predicate)(int a, int b);
}OpData;

int* GenerateCustomTable(OpData op)
{
	int* results = calloc(op.size, sizeof(int));
	for (int i = 0; i < op.size; i++)	
		results[i] = op.predicate(op.sourceA[i * op.offset], op.sourceB[i * op.offset]);	
	return results;
}

int OP_AND(int a, int b) { return a == 1 && b == 1; }
int OP_OR(int a, int b) { return a == 1 || b == 1; }

void SetOperatorPredicate(OpData* op, int opCode)
{
	switch(opCode)
	{
	case 0:
		op->predicate = &OP_AND;
		op->opChar = '^';
		break;
	case 1:
		op->predicate = &OP_OR;
		op->opChar = 'v';
		break;
	}
	
}

const char* OpCodeToStr(int i)
{
	switch (i)
	{
	case 0: return "AND";
	case 1: return "OR";
	default: return "INVALID";
	}
}

typedef struct UNBINDEDSOURCE_t
{
	int s1;
	int s2;
	int opCode;
}UnbindedSource;
typedef struct TABLEINFO_t
{
	int x, y,width,height;
	size_t nElements;
	char* hLabels;
	UnbindedSource* sources;
	int nOperations;

}TableInfo;

typedef struct TABLE_t
{
	TableInfo ti;
	struct
	{
		Grid gHeader,gTable,gSeparator;
	}baseTable;
	struct
	{
		Grid gOperatorHeader, *gOperator;
	}operatorTable;
}Table;

void DrawTable(Table t)
{
	DrawGrid(t.baseTable.gHeader);
	DrawGrid(t.baseTable.gTable);
	DrawGrid(t.baseTable.gSeparator);

	DrawGrid(t.operatorTable.gOperatorHeader);
	for (int i = 0; i < t.ti.nOperations; i++)
	{
		DrawGrid(t.operatorTable.gOperator[i]);
	}
	
	
}

Table BuildTable(TableInfo ti)
{
	Table t;
	t.ti = ti;
	int* baseData = GenerateBaseTable(ti.nElements);
	t.baseTable.gHeader = CreateGridPreset(ti.x,
										   ti.y,
										 ti.nElements,
		                                 1,
		                                   ti.hLabels);
	t.baseTable.gTable = CreateGridForTable(ti.x,
										  ti.y + 1,
										  ti.nElements,
										  ti.height,
											baseData);
	t.baseTable.gSeparator = CreateGridFill(ti.x + ti.width + ti.nElements - 1,
											  ti.y,
											1,
											ti.height + 1,
											'|');
	
	t.operatorTable.gOperatorHeader = CreateGridFill(t.baseTable.gSeparator.offx + t.baseTable.gSeparator.width + ti.nElements,
													   ti.y,
													 ti.nOperations+1,
													 1,
													 '*');

	t.operatorTable.gOperator = calloc(8, sizeof(Grid));

	OpData op;
	op.offset = ti.nElements;
	op.size = ti.height;
	for(int i = 0; i < ti.nOperations; i++)
	{
		op.sourceA = &baseData[ti.sources[i].s1];
		op.sourceB = &baseData[ti.sources[i].s2];
		SetOperatorPredicate(&op, ti.sources[i].opCode);
		
		
		MakeExprCell(&t.operatorTable.gOperatorHeader.cells[i],
					t.baseTable.gHeader.cells[ti.sources[i].s1].label,
					op.opChar,
					t.baseTable.gHeader.cells[ti.sources[i].s2].label );
		int* dat = GenerateCustomTable(op);
		
		t.operatorTable.gOperator[i] = CreateGridForTable(t.operatorTable.gOperatorHeader.offx +i*CELLWIDTH+i,
												      ti.y+1,
											 		  1,
												      ti.height,
												     	dat);
		
	}
	return t;
}

void ConstructTable(int _x, int _y, char* elements, size_t nElements)
{
	
	int* table = GenerateBaseTable(nElements);

	int x = _x;
	int y = _y;
	int w = nElements*CELLWIDTH;
	int h = exp(2, nElements);
	
	Grid gHeader = CreateGridPreset(x, y, nElements, 1, elements);
	Grid gTable = CreateGridForTable(x, y+1, nElements, h, table);
	Grid gSeparator = CreateGridFill(x + w + nElements - 1, y, 1, h+1, '|');
	DrawGrid(gHeader);
	DrawGrid(gTable);
	DrawGrid(gSeparator);
	

	Grid gOperatorHeader = CreateGridFill(gSeparator.offx + gSeparator.width + nElements, y, 1, 1, '*');
	MakeExprCell(&gOperatorHeader.cells[0], gHeader.cells[0].label, '^', gHeader.cells[1].label);
	
	OpData op;
	op.sourceA = &table[0];
	op.sourceB = &table[1];
	op.offset = nElements;
	op.size = h;
	op.predicate = &OP_AND;
	int* opTable = GenerateCustomTable(op);
	Grid gOperatorTable = CreateGridForTable(gSeparator.offx + gSeparator.width + nElements, y + 1, 1, h, opTable);
	//DrawGrid(gOperatorHeader);
	//DrawGrid(gOperatorTable);
	
}

void RebuildTable(TableInfo* ti)
{
	ti->width = ti->nElements * CELLWIDTH;
	ti->height = exp(2, ti->nElements==0?1: ti->nElements);
	Table t = BuildTable(*ti);
	DrawTable(t);
}

int SelectOperator()
{
	int isDone = 0;
	int result = 0;
	gotoxy(0, 0);
	printf("Selected operation: ");
	while (!isDone)
	{
		gotoxy(0, 1);
		printf("          ");
		gotoxy(0, 1);
		printf("%s", OpCodeToStr(result));
		const int k = getch();
		switch (k)
		{
		case VK_SPACE:		
			isDone = 1;		
		break;
		case VK_RIGHT:
			result++;
			if (result > 1)			
				result = 0;			
		break;
		default:
			break;
		}
	}
	gotoxy(0, 0);
	printf("                     ");
	gotoxy(0, 1);
	printf("                     ");
	return result;
}

void BuildOperator(TableInfo* ti)
{
	Table t = BuildTable(*ti);
	int selected = 0;	
	int isDone = 0;
	int s1 = -1, s2 = -1;

	while(!isDone)
	{
		const int x = t.baseTable.gHeader.offx + selected * CELLWIDTH + 2+ selected;
		const int y = t.baseTable.gHeader.offy - 1;
		const char carret = 'v';
		gotoxy(x, y);
		printf("%c", carret);
		const int key = getch();
		switch(key)
		{
		case VK_LEFT:
			if (selected > 0)
			{
				gotoxy(x, y);
				printf(" ");
				selected--;
			}
			break;
		case VK_RIGHT:
			if(selected < ti->nElements-1)
			{
				gotoxy(x, y);
				printf(" ");
				selected++;
			}		
			break;
		case VK_SPACE:
			{
			if (s1 == -1)			
				s1 = ti->sources[ti->nOperations].s1 = selected;							
			else if (s1 != -1 && s2 == -1)							
				s2 = ti->sources[ti->nOperations].s2 = selected;				
			
			if (s1 != -1 && s2 != -1)
			{
				gotoxy(x, y);
				printf(" ");
				ti->sources[(ti->nOperations)++].opCode = SelectOperator();
				RebuildTable(ti);
				isDone = 1;
			}
				
				
			}
			break;

		case VK_ESCAPE:
			gotoxy(x, y);
			printf(" ");
			isDone = 1;
			break;
		}
	}
	
}

void ApplyKey(int selected, TableInfo* ti)
{
	switch (selected)
	{
	case 0:
		RebuildTable(ti);
		break;
	case 2:
		BuildOperator(ti);
		break;
	case 3:
		exit(0);
		break;
	default:
		break;
	}
}

void ApplyLateralKey(int selected, int wparam, TableInfo* ti)
{
	switch(selected)
	{
	case 0:

		break;
	case 1:
		if (wparam > 0)
		{
			if (ti->nElements < 7)
			{
				ti->nElements++;
				RebuildTable(ti);
			}
		}
		if (wparam < 0)
		{
			if (ti->nElements > 0)
			{
				system("cls");
				ti->nElements--;
				RebuildTable(ti);
			}

		}
		break;
	}
}

int main()
{
	hidecursor();

	TableInfo ti;
	ti.hLabels = calloc(8, sizeof(char));
	ti.hLabels[0] = 'P';
	ti.hLabels[1] = 'Q';
	ti.hLabels[2] = 'R';
	ti.hLabels[3] = 'S';
	ti.hLabels[4] = 'T';
	ti.hLabels[5] = 'U';
	ti.hLabels[6] = 'V';
	ti.hLabels[7] = 'W';
	ti.x = 30;
	ti.y = 4;
	ti.nElements = 2;
	ti.width = ti.nElements * CELLWIDTH;
	ti.height = exp(2, ti.nElements);
	ti.sources = calloc(8, sizeof(UnbindedSource));
	ti.nOperations = 0;
	RebuildTable(&ti);
	
	
	int selected = 0;
	while (1)
	{
		int x = 0, y = 4;
		const int hspacing = 3;
		const int vspacing = 2;
		const int numberOfOptions = 4;
		const char carret = '>';
		
		gotoxy(0, y+selected*hspacing);
		printf("%c", carret);
		
		gotoxy(x+ vspacing,y);
		printf("Rebuild table");
		gotoxy(x + vspacing, y+1*hspacing);
		printf("Size: %i", ti.nElements);
		gotoxy(x + vspacing, y + 2 * hspacing);
		printf("Add simple operation");
		gotoxy(x + vspacing, y + 3 * hspacing);
		printf("Exit");
		int key = getch();
		switch(key)
		{		
		case VK_UP: // up
			if (selected > 0)
			{
				gotoxy(0, y+selected * hspacing);
				printf(" ");
				selected--;
			}
			break;
		case VK_DOWN:
			if (selected <= numberOfOptions)
			{
				gotoxy(0, y + selected * hspacing);
				printf(" ");
				selected++;
			}
			break;
		case VK_LEFT:
			ApplyLateralKey(selected, -1, &ti);
			break;
		case VK_RIGHT: 
			ApplyLateralKey(selected, 1, &ti);
			break;
		case VK_SPACE: 
			ApplyKey(selected, &ti);
			break;
		default:
			//printf("%i\n", key);
			break;
		}
		
		gotoxy(0, y+selected * hspacing);
		printf("%c", carret);

	}
}

