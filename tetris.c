//terminal font 24,  자간 8로 설정
//글자 두배로 키우는 기능  <—  ESC#3, ESC#4

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termio.h>
#include <unistd.h>
#include <wchar.h>

typedef struct {
    int (*xy_arr)[2];
    int shape[4][4];
    void (*spin)(int **);
} block_struct;

void gotoxy(int x, int y);
int getch();
void print_block(block_struct, int, int);
void print_frame(int (*)[12]);
void f1(int ** shape);
void fill_xy_arr(int (*)[2], int (*)[4]);
int find_xmax(int (*)[2]);
int find_xmin(int (*)[2]);
int find_ymax(int (*)[2]);
int isanyblock(int (*frame)[12], int * ,int *);

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
        f1}
    };

    printf("\e[?25l"); //커서 깜빡임 제거

    int x = 6;
    int y = 1;
    int key = 0;
    int current = (rand() % 6);
    while(1)
    {
        if(current == 5)
        {
            size = 10 * 4;
        }
        else
        {
            size = 8 * 4;
        }

        printf("\033[2J");//tput clear
        gotoxy(1,1);
        print_frame(frame);//게임틀 출력
        gotoxy(x,y);
        print_block(block[current], x, y);
        fill_xy_arr(block[current].xy_arr, block[current].shape);
        int xmax = find_xmax(block[current].xy_arr);
        int xmin = find_xmin(block[current].xy_arr);
        int ymax = find_ymax(block[current].xy_arr);
        
        //printf("□");
        //printf("■■■■■■■■■■■\n");
       
        key = getch();
        if(key == 10)
            key = getch();
        if(key == 27)
        {
            key = getch();
            if(key == 91)
            {
                key = getch();
                if(key == 66)
                {
                    //아래로 떨구기
                    y = 20 - (ymax + 1);
                    //x + xy_arr[0][0], y + xy_arr[0][1] : frame index
                    for(int i = 0; i < size/sizeof(block[current].xy_arr[0]); i++)
                    {
                        int framex[size/sizeof(block[current].xy_arr[0])], framey[size/sizeof(block[current].xy_arr[0])];
                        for(int j = 0; j < size/sizeof(block[current].xy_arr[0]); j++)
                        {
                            framex[j] = x + block[current].xy_arr[j][0] - 1;
                            framey[j] = y + block[current].xy_arr[j][1] - 1;
                        }
                        printf("%ld", sizeof(framex)/sizeof(framex[0]));
                        //if(해당하는 x좌표에 해당하는 y좌표 위로 하나의 행이라도 블럭이 있으면 return 1;)
                        if(isanyblock(frame, framex, framey))
                        {
                            y--;
                            i = -1;
                            continue;
                        }
                        /*
                        if(frame[framey[i]][framex[i]] == 2 || frame[framey[i]][framex[i]] == 1)
                        {
                            
                        }
                        */
                        //frame배열에 겹치는 부분이 없다면 frame에 2값 대입
                        if(i == size/sizeof(block[current].xy_arr[0]) - 1)
                        {
                            for(int j = 0; j < size/sizeof(block[current].xy_arr[0]); j++)
                            {
                                frame[framey[j]][framex[j]] = 2;
                            }
                        }
                    }
                current = rand()%6;
                x = 6;
                y = 1;
                key = 0;
                }
                if(key == 67)
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
                }
                if(key == 68)
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
                }
            }
        }
        
    }
    
}

void f1(int ** shape)
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
    for(int i = 0; i < size/sizeof(xy_arr[0]); )
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
    /*
    for(int j = 0; j <size/sizeof(xy_arr[0]); j++)
    {
        printf("%d %d\n", xy_arr[j][0], xy_arr[j][1]);
    }*/
}

void gotoxy(int x, int y) 
{
     printf("\033[%d;%df",y,x);

     fflush(stdout);
}

int find_xmax(int (*xy_arr)[2])
{
    int xmax = xy_arr[0][0];
    for(int i = 0; i < size/sizeof(xy_arr[0]); i++)
    {
        if(xmax < xy_arr[i][0])
            xmax = xy_arr[i][0];
    }
    return xmax;
}

int find_xmin(int (*xy_arr)[2])
{
    int xmin = xy_arr[0][0];
    for(int i = 0; i < size/sizeof(xy_arr[0]); i++)
    {
        if(xmin > xy_arr[i][0])
            xmin = xy_arr[i][0];
    }
    return xmin;
}

int find_ymax(int (*xy_arr)[2])
{
    int ymax = xy_arr[0][1];
    for(int i = 0; i < size/sizeof(xy_arr[0]); i++)
    {
        if(ymax < xy_arr[i][1])
            ymax = xy_arr[i][1];
    }
    return ymax;
}

int isanyblock(int (*frame)[12], int * framex, int * framey)
{
    for(int i = 0; i < sizeof(framex)/sizeof(framex[0]); i++)
    {
        for(int j = framey[i]; j >= 0; j--)
        {
            if(frame[j][framex[i]] == 2)
            return 1;
        }
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

void print_block(block_struct block, int x, int y)
{
    for(int i = 0; i < 4; i ++)
    {
        int xmax = find_xmax(block.xy_arr);
        int xmin = find_xmin(block.xy_arr);
        int ymax = find_ymax(block.xy_arr);
        //printf("\033[s");//커서 위치 저장
        for(int j = 0; j < 4; j++)
        {
            if(x == 1 && block.shape[i][j] == 0)
                printf("▨");
            else if(x == 12 && block.shape[i][j] == 0)
                printf("▨");
            else if(block.shape[i][j] == 0)
                printf(" ");
            else if(block.shape[i][j] == 1)
                printf("□");
            gotoxy(++x,y);//커서 우측으로 한 칸 이동
        }
        x -= 4;
        y++;
        gotoxy(x,y);
        //좌측으로 네 칸, 아래로 한 칸 커서 이동
        //printf("\033[u");//저장된 커서 위치로 이동
        //printf("\033[%dB", 1);//아래로 한 칸씩 커서 이동
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