//terminal font 24,  자간 8로 설정
//글자 두배로 키우는 기능  <—  ESC#3, ESC#4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termio.h>
#include <unistd.h>
#include <wchar.h>
#include <fcntl.h>

typedef struct {
    int (*xy_arr)[2];
    int shape[4][4];
    void (*spin)(int (*)[4]);
} block_struct;

void gotoxy(int x, int y);
int getch();
int print_block(block_struct, int, int, int (*)[12]);
void print_frame(int (*)[12]);
void f1(int (*shape)[4]);
void fill_xy_arr(int (*)[2], int (*)[4]);
int find_xmax(int (*)[2]);
int find_xmin(int (*)[2]);
int find_ymax(int (*)[2]);
int isanyblock(int (*frame)[12], int * ,int *, int);
void oneline_complete(int (*frame)[12]);
void mv_line(int (*frame)[12], int i);
int kbhit(void);
int isgameover(int (*frame)[12], block_struct block, int, int);

int size = 0;

int main()
{
    srand(time(NULL));

    int frame[20][12] = 
    {
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1}
    };
    block_struct block[6] ={
        {(int[4][2]){0}, 
        {
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0}
        },
        f1},
        {(int[4][2]){0}, 
        {
         {0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}
        },
        f1},
        {(int[4][2]){0},
        {
         {0, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 1, 0}
        },
        f1},
        {(int[4][2]){0}, 
        {
         {0, 0, 0, 0},
         {0, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 1, 0}
        },
        f1},
        {(int[4][2]){0},
        {
         {0, 0, 0, 0},
         {0, 0, 1, 0},
         {0, 1, 1, 1},
         {0, 0, 0, 0}
        },
        f1},
        {(int[5][2]){0}, 
        {
         {0, 0, 0, 0},
         {0, 0, 1, 0},
         {0, 1, 1, 1},
         {0, 0, 1, 0}
        },
        NULL}
    };

    printf("\e[?25l"); //커서 깜빡임 제거

    int x = 6;
    int y = 1;
    int key = 0;
    int current = rand() % 6;
    int next = rand() % 6;
    clock_t start, end;
    start = clock();

    while(1)
    {
        if(current == 5)
        {
            size = (10 * 4)/sizeof(block[current].xy_arr[0]);
        }
        else
        {
            size = (8 * 4)/sizeof(block[current].xy_arr[0]);
        }

        fill_xy_arr(block[current].xy_arr, block[current].shape);
        int xmax = find_xmax(block[current].xy_arr);
        int xmin = find_xmin(block[current].xy_arr);
        int ymax = find_ymax(block[current].xy_arr);
        int framex[size], framey[size];
        printf("\033[2J");//tput clear
        oneline_complete(frame);
        gotoxy(1,1);
        print_frame(frame);//게임틀 출력
        
        gotoxy(17, 1);//다음 블럭 출력
        printf("▨▨▨▨▨▨");
        gotoxy(17, 2);
        printf("▨    ▨");
        gotoxy(17, 3);
        printf("▨    ▨");
        gotoxy(17, 4);
        printf("▨    ▨");
        gotoxy(17, 5);
        printf("▨    ▨");
        gotoxy(17, 6);
        printf("▨▨▨▨▨▨");
        gotoxy(18, 2);
        print_block(block[next], 18, 2, frame);
/*
        if(isgameover(frame, block[current], x, y))
            break;
*/
        gotoxy(x,y);
        if(print_block(block[current], x, y, frame))
        {
            current = next;
            next = rand()%6;
            x = 6;
            y = 1;
            continue;
        }

       if(kbhit() == 0)
       {
           int out = 0;
           while(1)
           {
               if(kbhit())
                {
                    out = 1;
                    break;
                }
            
                end = clock();
                if((end - start)/1000 >= 1000)//1초 경과
                {
                    start = end;
                    y++;
                    //블럭 한 칸 아래로
                    break;
                }
           }
           if(out != 1)
            continue;
       }
        key = getch();
        if(key == 10)       //LF 방지
            key = getch();
        if(key == 116 && current < 5)
        {
            block[current].spin(block[current].shape);
            fill_xy_arr(block[current].xy_arr, block[current].shape);
            for(int i = 0; i < size; i++)
            {
                if(x + block[current].xy_arr[i][0] == 1)
                {
                    x++;
                    i = -1;
                }
                if(x + block[current].xy_arr[i][0] == 12)
                {
                    x--;
                    i = -1;
                }


                //y가 기존에 frame과 겹치는 경우도 계산
            }
            continue;
        }
        if(key == 27)
        {
            key = getch();
            if(key == 91)
            {
                key = getch();
                if(key == 66)//방향키 아래
                {
                    //아래로 떨구기
                    y = 20 - (ymax + 1);
                    //x + xy_arr[0][0], y + xy_arr[0][1] : frame index
                    for(int i = 0; i < size; i++)
                    {
                        for(int j = 0; j < size; j++)
                        {
                            framex[j] = x + block[current].xy_arr[j][0] - 1;
                            framey[j] = y + block[current].xy_arr[j][1] - 1;
                        }
                        //if(해당하는 x좌표에 해당하는 y좌표 위로 하나의 행이라도 블럭이 있으면 return 1;)
                        if(isanyblock(frame, framex, framey, size))
                        {
                            y--;
                            i = -1;
                            continue;
                        }
                        //frame배열에 겹치는 부분이 없다면 frame에 2값 대입
                        if(i == size - 1)
                        {
                            for(int j = 0; j < size; j++)
                            {
                                frame[framey[j]][framex[j]] = 2;
                            }
                        }
                    }
                current = next;
                next = rand()%6;
                x = 6;
                y = 1;
                key = 0;
                }
                if(key == 67)//방향키 오른쪽
                {
                    x++;
                    switch(xmax)
                    {
                        case 0:
                            if(x > 11)
                                x--;
                            break;
                        case 1:
                            if(x > 10)
                                x--;
                            break;
                        case 2:
                            if(x > 9)
                                x--;
                            break;
                        case 3:
                            if(x > 8)
                                x--;
                            break;
                    }
                    for(int j = 0; j < size; j++)
                    {
                        framex[j] = x + block[current].xy_arr[j][0] - 1;
                        framey[j] = y + block[current].xy_arr[j][1] - 1;
                    }
                    for(int i = 0; i < size && 1 <= x && x <= 12; i++)
                    {
                        if(frame[framey[i]][framex[i] + 1] == 2)
                        {
                            x--;
                            break;
                        }
                    }
                }
                if(key == 68)//방향키 왼쪽
                {
                    x--;
                    switch(xmin)
                    {
                        case 0:
                            if(x < 2)
                                x++;
                            break;
                        case 1:
                            if(x < 1)
                                x++;
                            break;
                        case 2:
                            if(x < 0)
                                x++;
                            break;
                        case 3:
                            if(x < -1)
                                x++;
                            break;
                    }
                    for(int j = 0; j < size; j++)
                    {
                        framex[j] = x + block[current].xy_arr[j][0] - 1;
                        framey[j] = y + block[current].xy_arr[j][1] - 1;
                    }
                    for(int i = 0; i < size && 1 <= x && x <= 12; i++)
                    {
                        if(frame[framey[i]][framex[i] - 1] == 2)
                        {
                            x++;
                            break;
                        }
                    }
                }
            }
        }
        
    }
    gotoxy(17, 10);
    printf("GAME OVER");    
}

void f1(int (*shape)[4])
{
    int cp_shape[4][4];
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cp_shape[i][j] = shape[i][j];
        }
    }
   for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = cp_shape[j][3-i];
        }
    }
}

void fill_xy_arr(int (*xy_arr)[2], int (*shape)[4])
{
    for(int i = 0; i < size; )
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                if(shape[k][j] == 1)
                {
                    xy_arr[i][0] = j;
                    xy_arr[i++][1] = k;
                }
            }
        }
    }
}

void gotoxy(int x, int y) 
{
     printf("\033[%d;%df",y,x);

     fflush(stdout);
}

int find_xmax(int (*xy_arr)[2])
{
    int xmax = xy_arr[0][0];
    for(int i = 0; i < size; i++)
    {
        if(xmax < xy_arr[i][0])
            xmax = xy_arr[i][0];
    }
    return xmax;
}

int find_xmin(int (*xy_arr)[2])
{
    int xmin = xy_arr[0][0];
    for(int i = 0; i < size; i++)
    {
        if(xmin > xy_arr[i][0])
            xmin = xy_arr[i][0];
    }
    return xmin;
}

int find_ymax(int (*xy_arr)[2])
{
    int ymax = xy_arr[0][1];
    for(int i = 0; i < size; i++)
    {
        if(ymax < xy_arr[i][1])
            ymax = xy_arr[i][1];
    }
    return ymax;
}

int isanyblock(int (*frame)[12], int * framex, int * framey, int size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = framey[i]; j >= 0; j--)
        {
            if(frame[j][framex[i]] == 2)
            return 1;
        }
    }
    return 0;
}

int isgameover(int (*frame)[12], block_struct block, int x, int y)
{
    int framex[size], framey[size];
    for(int i = 0; i < size; i++)
    {
        framex[i] = x + block.xy_arr[i][0] - 1;
        framey[i] = y + block.xy_arr[i][1] - 1;
    }

    for(int i = 0; i < size; i++)
    {
        if(frame[framey[i]][framex[i]] == 2)
            return 1;
    }

    return 0;
}

int getch()
{
    int c;
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr);           // 현재 터미널 설정 읽음
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);         // CANONICAL과 ECHO 끔
    newattr.c_cc[VMIN] = 1;                      // 최소 입력 문자 수를 1로 설정
    newattr.c_cc[VTIME] = 0;                     // 최소 읽기 대기 시간을 0으로 설정
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);  // 터미널에 설정 입력
    c = getchar();                               // 키보드 입력 읽음
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);  // 원래의 설정으로 복구
    return c;
}

int print_block(block_struct block, int x, int y, int (*frame)[12])
{
    int framex[size], framey[size];
    for(int i = 0; i < size; i++)
    {
        framex[i] = x + block.xy_arr[i][0] - 1;
        framey[i] = y + block.xy_arr[i][1] - 1;
    }

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(frame[framey[j]][framex[j]] == 2 || frame[framey[j]][framex[j]] == 1)
            {
                for(int k = 0; k < size; k++)
                {
                    frame[framey[k] - 1][framex[k]] = 2;
                }
                return 1;
            }
        }
        for(int j = 0; j < 4; j++)
        {
            if(x == 1 && block.shape[i][j] == 0)
                printf("▨");
            else if(x == 12 && block.shape[i][j] == 0)
                printf("▨");
            else if(block.shape[i][j] == 0 && frame[y - 1][x - 1] == 0)
                printf(" ");
            else if(block.shape[i][j] == 1 && frame[y - 1][x - 1] == 0)
                printf("□");
            else if(block.shape[i][j] == 0 && frame[y - 1][x - 1] == 2)
                printf("■");
            else if(1 <= x && x <= 12 && block.shape[i][j] == 0 && frame[y - 1][x - 1] == 1)
                printf("▨");
            gotoxy(++x,y);//커서 우측으로 한 칸 이동
        }
        x -= 4;
        y++;
        gotoxy(x,y);
        //좌측으로 네 칸, 아래로 한 칸 커서 이동
    }
    return 0;
}

void oneline_complete(int (*frame)[12])
{
    for(int i = 19; i >= 0; i--)
    {
        int cnt = 0;
        for(int j = 1; j < 11; j++)
        {
            if(frame[i][j] == 2)
                cnt++;
        }
        if(cnt == 10)
        {
            mv_line(frame, i);
            i = 20;
            continue;
        }
    }
}

void mv_line(int (*frame)[12], int i)
{
    for( ; i > 0; i--)
    {
        for(int j = 1; j < 11; j++)
        {
            frame[i][j] = frame[i - 1][j];
        }
    }
}

void print_frame(int (*frame)[12])
{
    
    
    for(int i = 0; i < 20; i++)
    {
        
        for(int j = 0; j < 12; j++)
        {
            if(frame[i][j] == 0)
            {
                printf(" ");
            }
            else if (frame[i][j] == 1)
            {
                printf("▨");
            }
            else if (frame[i][j] == 2)
            {
                printf("■");
            }
        }
        printf("\n");
    }
    
    /*
    for(int i = 0; i < 20; i++)
    {
        printf("%s\n", frame[i]);
    }
    */
    /*
    wchar_t frame[20][10] = 
    {
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' ',L'▨'},
        {L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨',L'▨'}
    };
    wprintf(L"%ls\n", frame[1][1]);
    //wchar_t (*p)[10] = frame;
    for(int i = 0; i < 20; i++)
    {
        wprintf(L"%ls\n", frame[i]);
    }
    */
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}