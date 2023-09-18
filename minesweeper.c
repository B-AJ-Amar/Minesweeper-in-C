// Librarys========================================================
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>

// TODO: toolbar , time, leaderboard ,bombs \/ , options
// exit 

// variables ======================================================
// Colors :
#define FG_RED "\x1b[31m"
#define FG_GREEN "\x1b[32m"
#define FG_YELLOW "\x1b[96;33m"
#define FG_BLUE "\x1b[34m"
#define FG_MAGENTA "\x1b[35m"
#define FG_CYAN "\x1b[36m"

#define FG_NORMAL "\x1b[m"
#define BG_RED "\x1b[41m"

int size = 8,bombs = 10;

typedef struct 
{
    int score;
    char name[30];
}winner;

winner *top10;
int LeaderBoardLen;

int rest, score ;
char colors[8][10]= {FG_CYAN,FG_GREEN,FG_RED,FG_YELLOW,FG_MAGENTA ,FG_NORMAL,BG_RED,FG_BLUE };
// display arr
int* arr ;
// game arr 
char* arr2;



// functions======================================================

void menu();

void leaderboard();
void GetLeaderBoard();
void save();

void help();
// start :
void start_first_move();
void start_display();
void start_fill(int sx,int sy);
void start_toolbar();
void start_play();
int start_fill2(int x,int y);
void win();
void lose();
void banner(int space);

// MAIN ==============================================================
int main(){
    menu();
    return 0;

}
// ===================================================================
// menu
void save(){
    int i;
    FILE *fl;
    fl = fopen("LeaderBoard.csv","w+"); 
    
    for (i = 0; i < LeaderBoardLen; i++)
    {
        fprintf(fl,"%d,%s",top10[i].score,top10[i].name);
        if (i<LeaderBoardLen-1) fprintf(fl,"\n");
        
    }
    fclose(fl);
}

void GetLeaderBoard(){
    int c=0,sorted=0;
    top10 = calloc(1,sizeof(winner));

    FILE *fl;
    fl = fopen("LeaderBoard.csv","r+"); 
    
    do
    {
        c++;
        top10 = realloc(top10,c*sizeof(winner));
        fscanf(fl,"%d,%32[^\n]",&top10[c-1].score,top10[c-1].name);

        
    }while (!feof(fl));

    winner tmp;
    
    while (!sorted)
    {
        sorted = 1;
        for (int i = 0; i < c-1; i++)
        {
            if (top10[i].score>top10[i+1].score)
            {
                sorted = 0;
                tmp= top10[i];
                top10[i]= top10[i+1];
                top10[i+1]= tmp;
            }     
        }      
    }
    fclose(fl);
    LeaderBoardLen =c;   
}

void leaderboard(){
    int i ;
    GetLeaderBoard();
    system("cls");
    banner(0);
    printf("%s      ---------------------------------------------------",FG_CYAN);
    printf("\n     |   N |                          Name |       Score |\n");
      printf("      ---------------------------------------------------\n");
    for (int i = 0; i < LeaderBoardLen; i++)
    {
      printf("     |%4d |%30s |%12d |\n",i+1,top10[i].name,top10[i].score);
    }
      printf("      ---------------------------------------------------\n");

    
    
    printf("\n\n\n\n            [press any key to GoBack ...] \n");
    getch();
    menu();

}
void help(){
    system("cls");
    banner(0);
    printf("\n%s>>> (x > 0 andf  y > 0)  : reveal an empty square.\n",FG_CYAN);
    printf(">>> (x < 0 and y < 0)  : flag an empty square.\n");
    printf(">>> (x = 0 and y = 0)  : go to menu.\n");
    printf(">>> Minesweeper rules : https://cutt.us/Minesweeper \n\n");
    printf(">>> Created By        : B A.J Amar \n\n\n\n");
    printf("            [press any key to GoBack ...] \n");
    getch();
    menu();
}

void menu(){
    char menu[5][30]={"   Start"," (options)","LeaderBoard","   Help", "   Exit"};
    int choice = 0,clr=0;
    char get;

    while (1)
    {
        
        system("cls");
        banner(0); 
        for (int i = 0; i < 5; i++)
        {
            clr =  (i==choice)? 5 : 0 ; 
            
            printf("\n                        %s%s\x1b[m\n",colors[clr],menu[i]);
            
        }
        get = getch();
        if (get==13){
            if (choice==4) exit(1);
            else if (choice==3) help();
            else if (choice==2) leaderboard();
            // else if (choice==1) leaderboard();
            else if (choice==0) {
                // start game
                start_reset();
                start_first_move();
                start_play();
            }
        }
        if (get==0 || get == 0xE0) 
        {
            get = getch();
        }	
		if (get==72) choice -= (choice>0)? 1:0 ;
			// printf("UP");
		else if (get==80) choice +=  (choice<4)? 1:0 ;
			// printf("DOWN");
     
    }
}

// start =====================================================

void banner(int space){
    for (int i = 0; i < space; i++) printf(" ");
   printf("%s __  __ _                                                           \n",FG_YELLOW);
    for (int i = 0; i < space; i++) printf(" ");
    printf("|  \\/  (_)_ __   ___  _____      _____  ___ _ __   ___ _ __          \n");
    for (int i = 0; i < space; i++) printf(" ");
    printf("| |\\/| | | '_ \\ / _ \\/ __\\ \\ /\\ / / _ \\/ _ \\ '_ \\ / _ \\ '__|\n");
    for (int i = 0; i < space; i++) printf(" ");
    printf("| |  | | | | | |  __/\\__ \\\\ V  V /  __/  __/ |_) |  __/ |          \n");
    for (int i = 0; i < space; i++) printf(" ");
    printf("|_|  |_|_|_| |_|\\___||___/ \\_/\\_/ \\___|\\___| .__/ \\___|_|       \n");
    printf("                                           |_|                      %s\n",FG_NORMAL);

}
void start_toolbar(){
  printf("%s ============================================================\n",FG_YELLOW);
    printf("|                                                            |\n");
    printf(" ============================================================%s\n"FG_NORMAL);


}

void start_reset(){
    int i,j;
    arr = calloc(size*size,sizeof(int));
    arr2 = malloc(size*size*sizeof(int));
    rest= size*size-bombs ;
     for ( i = 0 ; i < size; i++)
        {
            for ( j = 0 ; j < size; j++)
            {
                *(arr2+size*i+j) = ' ';
                *(arr+size*i+j) = 0;
            }
        }
}
// to make sure that the first move isn't bomb
void start_first_move(){
    int x,y;    
    do
        {
            system("cls");
            start_display();
            printf(">fm>> ");
            scanf("%d %d", &x, &y);
        } while (x<0 || y<0 || x>8 || y>8);
        if (x==0 || y == 0) menu();
 
        start_fill(x-1,y-1);
        start_fill2(x-1,y-1);
        start_display();

}

void start_display(){
    int i,j,clr;
    system("cls");
    banner(0);
    
    printf("             ");
    for (j= 0; j < size; j++) printf("   %d",j+1);
    printf("\n              ");
    for (j= 0; j < size; j++) printf(" ---");
    
    printf("\n");
    for (i = 0; i < size; i++)
    {
        printf("            %d ",i+1);
        for (j = 0; j < size; j++)
        {
            if(*(arr2+size*i+j) == ' ' ||  *(arr2+size*i+j) == '0')clr=5;
            else if (*(arr2+size*i+j) == '*') clr=6;
            else if (*(arr2+size*i+j) == 'X') clr=2;
            else clr = *(arr+size*i+j)%5-1 ;
            printf("| %s%c%s ",colors[clr],*(arr2+size*i+j),colors[5]);
        }
        printf("|\n              ");
        for (j = 0; j < size; j++)
        {
            printf(" ---");
        }
        printf("\n");  
    }  
}

void start_fill(int sx,int sy){
    // make bombs
    int x,y,i,j,count;
    srand(time(NULL));
    for (i = 0; i < bombs; i++)
    {
        // retry :
        
        x = rand() % 8;
        y = rand() % 8;
        if ( (abs(sx-x)>1 || abs(sy-y)>1 ) && *(arr+size*y+x) != -1) *(arr+size*y+x) = -1;
        
        else i--;
        // else goto retry ;
    }
    // nums
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (*(arr+size*i+j) == -1)continue;
            count =0;
            if (*(arr+size*(i-1)+j) == -1 && i-1 >=0 ) count++;
            if (*(arr+size*i+j-1) == -1 && j-1 >=0 ) count++;
            if (*(arr+size*(i+1)+j) == -1 && i+1 <=7 ) count++;
            if (*(arr+size*i+j+1) == -1 && j+1 <=7 ) count++;

            if (*(arr+size*(i-1)+j-1) == -1 && i-1 >=0 && j-1 >=0 ) count++;
            if (*(arr+size*(i-1)+j+1) == -1 && i-1 >=0 && j+1 <=7 ) count++;
            if (*(arr+size*(i+1)+j-1) == -1 && i+1 <=7 && j-1 >=0 ) count++;
            if (*(arr+size*(i+1)+j+1) == -1 && i+1 <=7 && j+1 <=7 ) count++;

            *(arr+size*i+j) = count;           
        }
       
    }
    score = time(NULL);  
}

void start_play(){
    int x,y;
    if (rest ==0 )win();
    while (1)
    {
        do
        {
            start_display();
            printf(">>>%d >  ",time(NULL)-score);
            scanf("%d %d", &x, &y);
        } while (x<-8 && y<-8 && x>8 && y>8);
        if (x==0 && y == 0) {
            menu();
            break;
        }
        if (x<0 || y<0)
        {
            x= abs(x)-1;
            y= abs(y)-1;
            if (*(arr2+size*y+x)==' ') *(arr2+size*y+x)='X';
            continue;      
        }   
        x--;
        y--;
        if (*(arr+size*y+x)==-1){
            *(arr2+size*y+x) = '*';
            lose();
            break;
        }
        if (*(arr+size*y+x)!=-1 && *(arr+size*y+x)!=0)
        {
            *(arr2+size*y+x) = *(arr+size*y+x)-208 ;
            rest--;
        }

        if (*(arr+size*y+x)==0)
        {
            start_fill2(x,y);            
        }
        if (rest==0){
            win();
            break;
        }  
    }    
}


int start_fill2(int x,int y){
    if (*(arr2+size*y+x)!=' ') return 0;
    if (*(arr+size*y+x)==-1) return 0;
    else if (*(arr+size*y+x)!=0 && *(arr+size*y+x)!=-1 ) *(arr2+size*y+x) = *(arr+size*y+x)-208;
    else if (*(arr+size*y+x)==0)
    {
        *(arr2+size*y+x) = '0';
        if (x+1<8) start_fill2(x+1,y);
        if (x-1>=0) start_fill2(x-1,y);
        if (y+1<8) start_fill2(x,y+1);
        if (y-1>=0) start_fill2(x,y-1);

        if (x+1<8 && y+1<8) start_fill2(x+1,y+1);
        if (x-1>=0 && y-1>=0) start_fill2(x-1,y-1);
        if (x-1>=0 && y+1<8) start_fill2(x-1,y+1);
        if (x+1<8 && y-1>=0) start_fill2(x+1,y-1);

    }
    rest--;
    return 1;   
}

void win(){
    int i,j,top=0;
    score = time(NULL)-score;
    for (i = 0; i < size; i++)
        for ( j = 0; j < size; j++) if (*(arr+size*i+j)==-1)*(arr2+size*i+j)='X';
    
    // check if top 10
    GetLeaderBoard();
    for (i = 0; i <= LeaderBoardLen; i++)
    {
        if (score < top10[i].score) {
            top=1;
            break;
        }  
    }
    

    
    // winn banner
    start_display();
    if (top)
    {
        for ( j = LeaderBoardLen; j > i; j--) top10[j].score=top10[j-1].score;
        printf(">>>Write your name : ");
        scanf("%s",top10[i].name);
        top10[i].score = score;
        save();
        printf("\nYOU WIN , in %d seconds\nyour name added to the LeaderBoard !!!\npress eny button to continue ...  ",score );

    }
    else
    {
        printf("\nYOU WIN , in %d seconds\npress eny button to continue ...  ",score );

    }
    

    getch();
    menu(); 
}
void lose(){
    int i,j;
    for (i = 0; i < size; i++)
        for ( j = 0; j < size; j++) if (*(arr+size*i+j)==-1)*(arr2+size*i+j)='*';
    // winn banner
    start_display();
    printf("\nYOU LOSE !!! Hard luck in the next time \npress eny button to continue ...  ");
    getch();
    menu();  
}

