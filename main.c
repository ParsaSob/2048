#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>


void print_borders(void){
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    refresh();
    for(int i=0;i<70;i++){
        move(0,i);
        printw("-");
        refresh();
        move(22,i);
        printw("-");
        refresh();
    }
    refresh();
    for(int i=0;i<23;i++){
        move(i,0);
        printw("|");
        refresh();
        move(i, 69);
        printw("|");
        refresh();
    }
    attroff(COLOR_PAIR(1));
    refresh();
}


int isPowerOfTwo(int n) {
    int count = 0;
    while (n > 0) {
        count += n & 1;
        n >>= 1;
    }
    return count == 1;
}

struct log {
    char username[30];
    int what;
};
struct usepass {
    char username[30];
    char password[20];
};
struct usescore {
    char username[30];
    int score;
    int num;
};

struct log login (void);
struct log signup(void);

int scoreboard (char username[30]) {
    clear();
    struct usescore players[50];
    int num_players = 0;

    FILE *fp = fopen("score.txt", "r");

    while (fscanf(fp, "%s %d", players[num_players].username, &players[num_players].score) == 2) {
        num_players++;
    }


    fclose(fp);

    for (int i = 0; i < num_players - 1; i++) {
        for (int j = i + 1; j < num_players; j++) {
            if (players[i].score < players[j].score) {
                struct usescore temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
    refresh();
    print_borders();
    refresh();
    move(3,29);
    attron(A_BLINK);
    printw("Top 10 Players:");
    attroff(A_BLINK);
    refresh();
    for (int i = 0; i < num_players && i < 10; i++) {
        players[i].num = i;
        refresh();
        move(4+i,2);
        printw("%d. %s - %d", i + 1, players[i].username, players[i].score);
        refresh();
    }
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < num_players; ++i) {
        if(strcmp(username,players[i].username)==0) {
            return players[i].num;

        }
    }


}


void remove_whitespace_horizontaly(int board[20][20], int rows, int columns, int r, int c)
{
    int a;
    int tmp;


    for (a = columns; a < c - 1; ++a) {
        tmp = board[rows][a];
        board[rows][a] = board[rows][a+1];
        board[rows][a+1] = tmp;
    }
}

void remove_whitespace_verticaly(int board[20][20], int columns, int rows, int r, int c)
{
    int a;
    int tmp;

    for (a = rows; a < r - 1; ++a) {
        tmp = board[a][columns];
        board[a][columns] = board[a+1][columns];
        board[a+1][columns] = tmp;
    }
}

void add_tiles_horizontaly(int board[20][20], int RC)
{
    int a, b, flag;

    for (a = 0; a < RC; ++a) {
        for (b = 0, flag = 0; b < RC - 1 && flag != RC; ++b) {
            if (board[a][b] == 0) {
                remove_whitespace_horizontaly(board, a, b, RC, RC);
                --b;
                ++flag;
            }
            else {
                if (board[a][b+1] == 0) {
                    board[a][b+1] = board[a][b];
                    board[a][b] = 0;
                    --b;
                } else if (board[a][b] == board[a][b+1]) {
                    board[a][b] = board[a][b] + board[a][b+1];
                    board[a][b+1] = 0;
                }
            }
        }
    }
}

void add_tiles_verticaly(int board[20][20],int RC)
{
    int a, b, flag;

    for (a = 0; a < RC; ++a) {
        for (b = 0, flag = 0; b < RC-1 && flag != RC; ++b) {
            if (board[b][a] == 0) {
                remove_whitespace_verticaly(board, a, b, RC, RC);
                --b;
                ++flag;
            }
            else {
                if (board[b+1][a] == 0) {
                    board[b+1][a] = board[b][a];
                    board[b][a] = 0;
                    --b;
                } else if (board[b][a] == board[b+1][a]) {
                    board[b][a] = board[b][a] + board[b+1][a];
                    board[b+1][a] = 0;
                }
            }
        }
    }
}
void print_board(int board[20][20], int RC)
{
    int a, b;
    init_pair(4, COLOR_GREEN,COLOR_BLACK);
    init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
    clear();
    refresh();
    print_borders();
    for (a = 0; a < RC; ++a) {
        refresh();
        move(11-(RC/2)+a,33-((RC+(RC-1)*2)/2));
        for (b = 0; b < RC; ++b) {
            if(board[a][b] == 0) {
                refresh();
                attron(COLOR_PAIR(4));
                printw("[%d]", board[a][b]);
                attroff(COLOR_PAIR(4));
                refresh();
            } else {
                refresh();
                attron(COLOR_PAIR(5));
                printw("[%d]", board[a][b]);
                attroff(COLOR_PAIR(5));
                refresh();
            }
        }
    }
}
void reverse_board(char input, int board[20][20], int RC)
{
    int a, b, c, tmp;

    if (input == 'd' || input == 'D') {
        for (a = 0; a < RC; ++a) {
            for (b = 0, c = RC-1; b < RC/2; ++b, --c) {
                tmp = board[a][b];
                board[a][b] = board[a][c];
                board[a][c] = tmp;
            }
        }
    }
    else if  (input == 's' || input == 'S') {
        for (a = 0; a < RC; ++a) {
            for (b = 0, c = RC-1; b < RC/2; ++b, --c) {
                tmp = board[b][a];
                board[b][a] = board[c][a];
                board[c][a] = tmp;
            }
        }
    }
}
int check_board (int board[20][20], int RC) {
    int a, b;

    int result = 0;
    int empty_tiles = 0;


    for (a = 0; a < RC; ++a)
        for (b = 0; b < RC; ++b)
            if (board[a][b] == 2048)
                result = -1;
            else if (board[a][b] == 0)
                ++empty_tiles;

    result = result == -1 ? result : empty_tiles;

    return result;
}

void generate_random(int board[20][20], int empty_tiles, int RC )
{

    srand(time(NULL));

    int a, b;
    int random = 0;
    int tile = 0;

    random = rand() % empty_tiles;
    tile = (rand() % 9 == 4) ? 4 : 2;

    for (a = 0; a < RC; ++a)
        for (b = 0; b < RC; ++b)
            if (board[a][b] == 0 && random != 0)
                --random;
            else if (board[a][b] == 0 && random == 0) {
                board[a][b] = tile;
                return;
            }
}

int play_game(int board[20][20], int RC)
{
    init_pair(6,COLOR_BLACK,COLOR_MAGENTA);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    int score = 0;
    int d = 0;
    char movement;
    int tiles = 0;

    print_board(board, RC);
    refresh();
    move(1,62);
    attron(COLOR_PAIR(6));
    printw("(%d)", score);
    attroff(COLOR_PAIR(6));
    refresh();
    move(23,1);
    printw("FOR UP PRESS 'W' || FOR DOWN PRESS 'S'");
    refresh();
    move(24,0);
    printw(" FOR RIGHT PRESS 'D' || FOR LEFT PRESS 'A' || FOR EXIT JUST PRESS 'E'");
    refresh();
    while (1) {
        refresh();
        movement = getch();

        if (movement == 's' || movement == 'S') {
            reverse_board(movement,board, RC);
            add_tiles_verticaly(board,RC);
            tiles = check_board(board, RC);
            if (tiles == -1) {
                while(d==0) {
                    clear();
                    refresh();
                    print_borders();
                    char con;
                    refresh();
                    move(5,30);
                    attron(COLOR_PAIR(4));
                    printw("YOU WON!!!");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    d = 1;
                    move(7,23);
                    printw("Do you want to continue:");
                    refresh();
                    move(8,17);
                    printw("for YES enter 'Y' || for NO enter 'N'");
                    refresh();
                    con = getch();
                    if (con == 'N' || con == 'n') {
                        return -1;
                    }
                }
            }
            else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
            reverse_board(movement, board,RC);
        } else if (movement == 'w' || movement == 'W') {
            add_tiles_verticaly(board,RC);
            tiles = check_board(board,RC);
            if (tiles == -1) {
                while(d==0) {
                    clear();
                    refresh();
                    print_borders();
                    char con;
                    refresh();
                    move(5,30);
                    attron(COLOR_PAIR(4));
                    printw("YOU WON!!!");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    d = 1;
                    move(7,23);
                    printw("Do you want to continue:");
                    refresh();
                    move(8,17);
                    printw("for YES enter 'Y' || for NO enter 'N'");
                    refresh();
                    con = getch();
                    if (con == 'N' || con == 'n') {
                        return -1;
                    }
                }
            } else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
        } else if (movement == 'D' || movement == 'd') {
            reverse_board(movement,board,RC);
            add_tiles_horizontaly(board,RC);
            tiles = check_board(board,RC);
            if (tiles == -1) {
                while(d==0) {
                    clear();
                    refresh();
                    print_borders();
                    char con;
                    refresh();
                    move(5,30);
                    attron(COLOR_PAIR(4));
                    printw("YOU WON!!!");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    d = 1;
                    move(7,23);
                    printw("Do you want to continue:");
                    refresh();
                    move(8,17);
                    printw("for YES enter 'Y' || for NO enter 'N'");
                    refresh();
                    con = getch();
                    if (con == 'N' || con == 'n') {
                        return -1;
                    }
                }
            }  else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
            reverse_board(movement, board,RC);
        } else if (movement == 'A'|| movement == 'a') {
            add_tiles_horizontaly(board,RC);
            tiles = check_board(board,RC);
            if (tiles == -1) {
                while(d==0) {
                    clear();
                    refresh();
                    print_borders();
                    char con;
                    refresh();
                    move(5,30);
                    attron(COLOR_PAIR(4));
                    printw("YOU WON!!!");
                    attroff(COLOR_PAIR(4));
                    refresh();
                    d = 1;
                    move(7,23);
                    printw("Do you want to continue:");
                    refresh();
                    move(8,17);
                    printw("for YES enter 'Y' || for NO enter 'N'");
                    refresh();
                    con = getch();
                    if (con == 'N' || con == 'n') {
                        return -1;
                    }
                }
            } else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
        } else if (movement == 'e' || movement == 'E') {
            return 1;
        } else if (movement == 'p'){
            clear();
            refresh();
            move(2,0);
            attron(COLOR_PAIR(3));
            printw("WELCOME TO THE CHAMBER OF SECRET");
            attroff(COLOR_PAIR(3));
            refresh();
            int i1, j1, value1, valid;
            refresh();
            move(4,0);
            scanw("%d %d %d", &i1, &j1, &value1);
            refresh();
            valid = isPowerOfTwo(value1);
            if (i1 < 1 || i1 > RC || j1 < 1 || j1 > RC) {
                refresh();
                move(5,0);
                printw("coordinate out of range! ");
                refresh();
                move(6,0);
                printw("PRESS 'p' AGAIN");
                refresh();
                continue;
            }
            if(board[i1-1][j1-1] != 0) {
                refresh();
                move(5,0);
                printf("cell is not empty! ");
                refresh();
                move(6,0);
                printw("PRESS 'p' AGAIN");
                refresh();
                continue;
            }
            if (valid == 0 || value1 == 1) {
                refresh();
                move(5,0);
                printf("invalid value!");
                refresh();
                move(6,0);
                printw("PRESS 'p' AGAIN");
                refresh();
                continue;
            }
            board[i1-1][j1-1] = value1;
            refresh();
        } else {
            refresh();
            move(23,1);
            attron(COLOR_PAIR(2));
            printw("Do not recognize this movement please try again");
            attroff(COLOR_PAIR(2));
            refresh();
            continue;
        }
        print_board(board,RC);
        for (int i = 0; i < RC; ++i) {
            for (int j = 0; j < RC; ++j) {
                score = board[i][j] + score;
            }
        }
        refresh();
        move(1,62);
        attron(COLOR_PAIR(6));
        printw("(%d)", score);
        attroff(COLOR_PAIR(6));
        refresh();
    }
}


int GameMenu (char username[30]) {
    clear();
    refresh();
    print_borders();
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(1,COLOR_RED,COLOR_BLACK);
    int p1 = 0;
    char wmenu;
    refresh();
    move(2,25);
    attron(COLOR_PAIR(2));
    printw("Welcome to the Game");
    attroff(COLOR_PAIR(2));
    refresh();
    move(4,32);
    attron(A_BLINK);
    printw("START");
    attroff(A_BLINK);
    refresh();
    move(5, 30);
    printw("PRESS 's'");
    move(9,29);
    attron(COLOR_PAIR(3));
    printw("SCOREBOARD");
    attroff(COLOR_PAIR(3));
    refresh();
    move(10, 30);
    printw("PRESS 'c'");
    move(14,31);
    attron(COLOR_PAIR(5));
    printw("LOGOUT");
    attroff(COLOR_PAIR(5));
    refresh();
    move(15, 30);
    printw("PRESS 'l'");
    move(19,32);
    printw("EXIT");
    move(20, 30);
    printw("PRESS 'e'");
    refresh();
    move(24,0);
    wmenu = getch();
    if (wmenu == 's' || wmenu == 'S') {
        int score = 0;
        int rc;
        clear();
        refresh();
        print_borders();
        refresh();
        move(1,2);
        printw("Please enter dimension!");
        refresh();
        move(1,26);
        attron(COLOR_PAIR(3));
        printw("[  ]");
        attroff(COLOR_PAIR(3));
        refresh();
        move(1,27);
        scanw("%d", &rc);
        refresh();
        if (rc < 3) {
            refresh();
            move(3,2);
            printw("Dimension is not valid!");
            refresh();
            move(5,2);
            attron(A_BLINK);
            printw("To continiue press any key: ");
            attroff(A_BLINK);
            refresh();
            getch();
            p1 = GameMenu(username);
            if (p1==1) return 1;
        }
        int board[20][20];
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                board[i][j] = 0;
            }
        }
        int win_condition;
        generate_random(board, rc*rc,rc);
        generate_random(board, rc*rc-1,rc);

        win_condition = play_game(board,rc);
        for (int i = 0; i < rc; ++i) {
            for (int j = 0; j < rc; ++j) {
                score = board[i][j] + score;
            }
        }
        switch (win_condition) {
            case 1:
                break;
            case 0:
                clear();
                refresh();
                print_borders();
                refresh();
                move(4,26);
                attron(COLOR_PAIR(5));
                printw("|-----------------|");
                attroff(COLOR_PAIR(5));
                refresh();
                move(5, 26);
                attron(COLOR_PAIR(5));
                printw("|");
                attroff(COLOR_PAIR(5));
                refresh();
                printw("YOU LOSE, NOW CRY");
                refresh();
                attron(COLOR_PAIR(5));
                printw("|");
                attroff(COLOR_PAIR(5));
                refresh();
                move(6,26);
                attron(COLOR_PAIR(5));
                printw("|-----------------|");
                attroff(COLOR_PAIR(5));
                refresh();
                move(10,31);
                attron(COLOR_PAIR(5));
                printw("|-------|");
                attroff(COLOR_PAIR(5));
                refresh();
                move(11, 31);
                attron(COLOR_PAIR(5));
                printw("|");
                attroff(COLOR_PAIR(5));
                refresh();
                printw("Byyyeee");
                refresh();
                attron(COLOR_PAIR(5));
                printw("|");
                attroff(COLOR_PAIR(5));
                refresh();
                move(12,31);
                attron(COLOR_PAIR(5));
                printw("|-------|");
                attroff(COLOR_PAIR(5));
                refresh();
                move(21, 23);
                attron(A_BLINK);
                printw("PRESS ANY KEY TO CONTINUE");
                attroff(A_BLINK);
                refresh();
                getch();
                break;
            case -1:
                clear();
                refresh();
                print_borders();
                refresh();
                move(4, 23);
                printw("You think this is the end");
                refresh();
                move(10,32);
                printw("YOU FOOL");
                move(15, 24);
                attron(COLOR_PAIR(1));
                printw("Now the real game starts");
                attroff(COLOR_PAIR(1));
                refresh();
                move(21, 23);
                attron(A_BLINK);
                printw("PRESS ANY KEY TO CONTINUE");
                attroff(A_BLINK);
                refresh();
                getch();
                break;
        }
        struct usescore us;
        us.score = score;
        int new_score = score;
        for (int i = 0; i < 30; ++i) {
            us.username[i] = username[i];
        }
        char new_username[30];
        strcpy(new_username,us.username);
        if(strcmp(username,"guest")!= 0) {
            FILE *f = fopen("score.txt", "r+");
            int found = 0;
            int d = 0;
            while (fscanf(f, "%s %d", us.username, &us.score) != EOF) {
                if (strcmp(us.username, username) == 0) {
                    fseek(f,-sizeof(us),SEEK_CUR);
                    if (us.score >= score) {
                        d = 1;
                    }
                    found = 1;
                    break;
                }
            }
            if (d == 0) {
                if (!found) {
                    fprintf(f, "%s %d", new_username, new_score);
                } else {
                    long int size = ftell(f);
                    FILE *temp = fopen("temp.txt", "w");
                    rewind(f);
                    char line[100];
                    while (fgets(line, 100, f)) {
                        if (strncmp(line, username, strlen(username)) != 0) {
                            fputs(line, temp);
                        } else {
                            fprintf(temp, "%s %d\n", us.username, new_score);
                        }
                    }
                    fclose(f);
                    fclose(temp);
                    remove("score.txt");
                    rename("temp.txt", "score.txt");
                }
            }
            fclose(f);

        }
        p1 = GameMenu(username);
        if (p1==1) return 1;
    } else if (wmenu == 'c' || wmenu == 'C') {
        int r = 0;
        char rank;
        r = scoreboard(username);

        if (strcmp(username,"guest")!=0) {
            refresh();
            move(20,2);
            printw("Congratulations, Your rank is '%d'", r + 1);
            refresh();
        } else {
            refresh();
            move(20,2);
            printw("you are guest!");
            refresh();
        }
        refresh();
        move(21,2);
        attron(A_BLINK);
        printw("FOR GETTING BACK JUST PRESS 'B'");
        attroff(A_BLINK);
        refresh();
        rank = getch();
        if (rank == 'b' || rank == 'B') {
            p1 = GameMenu(username);
            if (p1==1) return 1;
        }

    } else if (wmenu == 'l' || wmenu == 'L') {
        return 1;
    }else if (wmenu == 'e' || wmenu == 'E' ) {
        return 0;
    } else {
        refresh();
        move(24,0);
        printw("invalid command");
        refresh();
        p1 = GameMenu(username);
        if (p1==1) return 1;
    }
    return 0;
}



int main(void) {
    int r = 0;
    int p = 1;
    initscr();
    refresh();
    start_color();
    while (p == 1) {
        clear();
        refresh();
        print_borders();
        refresh();
        p = 0;
        int c = 1;
        char enter;
        refresh();
        move(3, 32);
        printw("SIGNUP");
        refresh();
        move(4, 31);
        printw("PRESS 's'");
        move(8, 32);
        printw("LOGIN");
        refresh();
        move(9, 31);
        printw("PRESS 'l'");
        refresh();
        move(13, 32);
        printw("GUEST");
        refresh();
        move(14, 31);
        printw("PRESS 'g'");
        refresh();
        move(18, 32);
        printw("EXIT");
        refresh();
        move(19,31);
        printw("PRESS 'e'");
        refresh();
        enter = getch();
        if (enter == 's' || enter == 'S') {
            system("CLS");;
            struct log w;
            while (c == 1) {
                clear();
                if (r==1) {
                    refresh();
                    move(4, 19);
                    printw("The username has already taken");
                    r = r-1;
                    refresh();
                }
                r++;
                refresh();
                w = signup();
                c = w.what;
                if (c == 2) return 0;
            }
            p = GameMenu(w.username);

        } else if (enter == 'l' || enter == 'L') {
            system("CLS");
            struct log w;
            while (c == 1 || c==3 || c==4) {
                clear();
                if(c==3) {
                    refresh();
                    move(10, 25);
                    printw("USERNAME NOT FOUND!");
                    refresh();
                }else if (c == 4) {
                    refresh();
                    move(10, 25);
                    printw("PASSWORD INCORRECT!");
                    refresh();
                }
                w = login();
                c = w.what;
                if (c == 2) return 0;
            }
            p = GameMenu(w.username);
        } else if (enter == 'g' || enter == 'G') {
            p = GameMenu("guest");
        } else if (enter == 'e' || enter == 'E') {
            return 0;
        } else {
            move(24,0);
            printw("invalid command");
            refresh();
            p = 1;
        }
    }
    return 0;
}
struct log login (void) {
    struct log ll;
    char username[30],password[20];
    FILE *log;

    log = fopen("login.txt","r");

    struct usepass l;
    refresh();
    print_borders();
    refresh();
    move(17, 19);
    printw("IF YOU WANT TO EXIT ENTER 'exit'");
    refresh();
    move(3, 25);
    printw("Enter Your USERNAME:");
    refresh();
    scanw("%s", username);
    if (strcmp(username,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    move(15, 25);
    printw("Enter Your PASSWORD:");
    refresh();
    scanw("%s", password);

    if (strcmp(password,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    fseek(log, 0, SEEK_SET);
    while(fread(&l,sizeof(l),1,log)) {
        if (strcmp(username, l.username) == 0) {
            if (strcmp(password, l.password) == 0) {
                refresh();
                move(24,0);
                printw("Successful Login");
                refresh();
                fclose(log);
                ll.what = 0;
                for (int i = 0; i < 30; ++i) {
                    ll.username[i]=username[i];
                }
                getch();
                return ll;
            } else {
                fseek(log, 0, SEEK_SET);
                ll.what = 4;
                return ll;
            }
        }
    }
    fseek(log, 0, SEEK_SET);
    while(fread(&l,sizeof(l),1,log)) {
        if (strcmp(username, l.username) != 0) {
            fseek(log, 0, SEEK_SET);
            ll.what = 3;
            return ll;
        }
    }



}

struct log signup(void) {
    struct log ll;
    char username[30];
    FILE *log;
    log=fopen("login.txt","a+");
    struct usepass l;
    refresh();
    print_borders();
    refresh();
    move(17, 19);
    printw("IF YOU WANT TO EXIT ENTER 'exit'");
    refresh();
    move(3, 25);
    printw("Enter Your USERNAME:");
    refresh();
    scanw("%s",username);
    if (strcmp(username,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    while(fread(&l,sizeof(l),1,log)) {
        if(strcmp(username,l.username)==0) {
            fclose(log);
            fseek(log, 0, SEEK_SET);
            ll.what = 1;
            return ll;
        }
    }
    refresh();
    clear();
    refresh();
    print_borders();
    refresh();
    move(3, 25);
    printw("Enter Your USERNAME:");
    refresh();
    move(17, 19);
    printw("IF YOU WANT TO EXIT ENTER 'exit'");
    refresh();
    move(15, 25);
    printw("Enter Your PASSWORD:");
    refresh();
    scanw("%s",l.password);
    if (strcmp(l.password,"exit") == 0) {
        ll.what = 2;
        return ll;
    }

    for (int i = 0; i < 30; ++i) {
        l.username[i] = username[i];
    }
    for (int i = 0; i < 30; ++i) {
        ll.username[i] = username[i];
    }
    fwrite(&l,sizeof(l),1,log);
    fclose(log);
    refresh();
    move(24,0);
    printw("Registration Successful!");
    refresh();
    getch();
    system("CLS");
    ll.what = 0;
    return ll;
}
