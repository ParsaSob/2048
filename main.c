#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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

    printf("\nTop 10 Players:\n");
    for (int i = 0; i < num_players && i < 10; i++) {
        players[i].num = i;
        printf("%d. %s - %d\n", i + 1, players[i].username, players[i].score);
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
    system("CLS");
    for (a = 0; a < RC; ++a) {
        printf("\n");
        for (b = 0; b < RC; ++b) {
            printf("%5i", board[a][b]);
        }
    }
    printf("\n");
}
void reverse_board(char input[], int board[20][20], int RC)
{
    int a, b, c, tmp;

    if (!strcmp(input, "right")) {
        for (a = 0; a < RC; ++a) {
            for (b = 0, c = RC-1; b < RC/2; ++b, --c) {
                tmp = board[a][b];
                board[a][b] = board[a][c];
                board[a][c] = tmp;
            }
        }
    }
    else if  (!strcmp(input, "down")) {
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
    int d = 0;
    char movement[RC*RC*RC*RC];
    int tiles = 0;

    print_board(board, RC);
    while (1) {
        printf("(enter: left,right,up,down,exit)>> ");
        scanf("%s", movement);
        if (!strcmp(movement, "down")) {
            reverse_board(movement,board, RC);
            add_tiles_verticaly(board,RC);
            tiles = check_board(board, RC);
            if (tiles == -1) {
                while(d==0) {
                    char con;
                    printf("\nyou won!!!!!!!\n");
                    d = 1;
                    printf("\nDo you want to continue:\n for YES enter 'y'\n for NO enter 'n'\n");
                    scanf(" %c", &con);
                    if (con == 'N' || con == 'n') {
                        return -1;
                    }
                }
            }
            else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
            reverse_board(movement, board,RC);
        } else if (!strcmp(movement, "up")) {
            add_tiles_verticaly(board,RC);
            tiles = check_board(board,RC);
            if (tiles == -1) {
                while(d==0) {
                    char con;
                    printf("\nyou won!!!!!!!\n");
                    d = 1;
                    printf("\nDo you want to continue:\n for YES enter 'y'\n for NO enter 'n'\n");
                    scanf(" %c", &con);
                    if (con == 'n' || con == 'N') {
                        return -1;
                    }
                }
            }  else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
        } else if (!strcmp(movement, "right")) {
            reverse_board(movement,board,RC);
            add_tiles_horizontaly(board,RC);
            tiles = check_board(board,RC);
            if (tiles == -1) {
                while(d==0) {
                    char con;
                    printf("\nyou won!!!!!!!\n");
                    d = 1;
                    printf("\nDo you want to continue:\n for YES enter 'y'\n for NO enter 'n'\n");
                    scanf(" %c", &con);
                    if (con == 'N' || con == 'n') {
                        return -1;
                    }
                }
            }  else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
            reverse_board(movement, board,RC);
        } else if (!strcmp(movement, "left")) {
            add_tiles_horizontaly(board,RC);
            tiles = check_board(board,RC);
            if (tiles == -1) {
                while(d==0) {
                    char con;
                    printf("\nyou won!!!!!!!\n");
                    d = 1;
                    printf("\nDo you want to continue:\n for YES enter 'y'\n for NO enter 'n'\n");
                    scanf(" %c", &con);
                    if (con == 'n' || con == 'N') {
                        return -1;
                    }
                }
            } else if (tiles == 0)
                return 0;
            generate_random(board,tiles,RC);
        } else if (!strcmp(movement, "exit")) {
            return 1;
        } else if (!strcmp(movement, "put")){
            int i1, j1, value1, valid;
            scanf("%d %d %d", &i1, &j1, &value1);
            valid = isPowerOfTwo(value1);
            if (i1 < 1 || i1 > RC || j1 < 1 || j1 > RC) {
                printf("\ncoordinate out of range!\n");
                continue;
            }
            if(board[i1-1][j1-1] != 0) {
                printf("\ncell is not empty!\n");
                continue;
            }
            if (valid == 0 || value1 == 1) {
                printf("\ninvalid value!\n");
                continue;
            }
            board[i1-1][j1-1] = value1;
        } else {
            printf("Do not recognize this movement please type again\n");
            continue;
        }
        print_board(board,RC);
    }
}



int GameMenu (char username[30]) {
    int p1 = 0;
    char wmenu[20];
    printf("\nWelcome to the Game\n");
    printf("You have several choices\nFor start type 'start'\nfor seeing scoreboard type 'scoreboard'\nfor logout type 'logout'\n");
    scanf("%s",wmenu);
    if (strcmp(wmenu,"start") == 0) {
        int score = 0;
        int rc;
        printf("Enter the size of the game:\n");
        scanf("%d", &rc);
        if (rc < 3) {
            printf("invalid size, size must be bigger than 2\n");
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
                printf("YOU LOSE, NOW CRY\n"	\
	   "Byyyeee\n");
                break;
            case -1:
                printf("You think this is the end, you fool.\nThe real game is starting just right now\n");
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
            while (fscanf(f, "%s %d", us.username, &us.score) != EOF) {
                if (strcmp(us.username, username) == 0) {
                    fseek(f,-sizeof(us),SEEK_CUR);
                    found = 1;
                    break;
                }
            }
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
            fclose(f);

        }
        p1 = GameMenu(username);
        if (p1==1) return 1;
    } else if (strcmp(wmenu,"scoreboard") == 0) {
        int r = 0;
        char rank[20];
        r = scoreboard(username);
            printf("\nIf you want to see your rank before exit enter 'myrank' and for just exit enter back:\n");
            scanf("%s", rank);
            if (strcmp(rank, "myrank") == 0) {
                if (strcmp(username,"guest")!=0) {
                    printf("\nCongratulations, Your rank is '%d'\n", r + 1);
                    p1 = GameMenu(username);
                    if (p1==1) return 1;
                } else {
                    printf("\nyou are guest!\n");
                    p1 = GameMenu(username);
                    if (p1==1) return 1;
                }
            } else if (strcmp(rank, "back") == 0){
                p1 = GameMenu(username);
                if (p1==1) return 1;
            }

    } else if (strcmp(wmenu,"logout") == 0) {
        return 1;
    }else if (strcmp(wmenu,"exit") == 0) {
        return 0;
    } else {
        printf("invalid command");
        p1 = GameMenu(username);
        if (p1==1) return 1;
    }
    return 0;
}

int main() {
    int p = 1;
    while (p == 1) {
        p = 0;
        int c = 1;
        char enter[20];
        printf("How do you want to enter:\n");
        printf("\nIf you want to signup enter your username and password like this:\nsignup <username> <password>\nfor login, like this:\nlogin <username> <password>\nand for continue as a guest, just type guest and then press enter:\n");
        scanf("%s", enter);
        if (strcmp(enter, "signup") == 0) {
            struct log w;
            while (c == 1) {
                w = signup();
                c = w.what;
                if (c == 2) break;
            }
            p = GameMenu(w.username);

        } else if (strcmp(enter, "login") == 0) {
            struct log w;
            while (c == 1) {
                w = login();
                c = w.what;
                if (c == 2) break;
            }
            p = GameMenu(w.username);
        } else if (strcmp(enter, "guest") == 0) {
            p = GameMenu("guest");
        } else if (strcmp(enter, "exit") == 0) {
            return 0;
        } else {
            printf("invalid command");
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

    scanf("%s", username);
    if (strcmp(username,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    //username[strcspn(username, "\n")] = 0;
    scanf("%s", password);
    //password[strcspn(password, "\n")] = 0;
    if (strcmp(password,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    while(fread(&l,sizeof(l),1,log)) {
        if (strcmp(username, l.username) == 0 && strcmp(password, l.password) == 0) {
            printf("\nSuccessful Login\n");
            fclose(log);
            ll.what = 0;
            for (int i = 0; i < 30; ++i) {
                ll.username[i]=username[i];
            }
            return ll;
        }
    }
    fseek(log, 0, SEEK_SET);
    while(fread(&l,sizeof(l),1,log)) {
        if (strcmp(username, l.username) != 0) {
            printf("\nusername not found!\nfor login just enter your correct username and password like this\n<username> <password>\n");
            fseek(log, 0, SEEK_SET);
            ll.what = 1;
            return ll;
        } else if (strcmp(username, l.username) == 0 && strcmp(password, l.password) != 0) {
            printf("\npassword incorrect!\nfor login just enter your correct username and password like this\n<username> <password>\n");
            fseek(log, 0, SEEK_SET);
            ll.what = 1;
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

    scanf("%s",username);
    if (strcmp(username,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    scanf("%s",l.password);
    if (strcmp(l.password,"exit") == 0) {
        ll.what = 2;
        return ll;
    }
    while(fread(&l,sizeof(l),1,log)) {
        if(strcmp(username,l.username)==0) {
            printf("\nThe username has already taken\nfor signup just enter your username and password like this\n<username> <password>\n");
            fclose(log);
            fseek(log, 0, SEEK_SET);
            ll.what = 1;
            return ll;
        }
    }
    for (int i = 0; i < 30; ++i) {
        l.username[i] = username[i];
    }
    for (int i = 0; i < 30; ++i) {
        ll.username[i] = username[i];
    }
    fwrite(&l,sizeof(l),1,log);
    fclose(log);

    printf("\nRegistration Successful!\n");
    getchar();
    system("CLS");
    ll.what = 0;
    return ll;
}
