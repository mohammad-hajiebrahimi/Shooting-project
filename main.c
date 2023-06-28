#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <simple2d.h>

// Game Variables
char player1[20];
char player2[20];
int player_score[2]={0,0};
int Final_Score;
int Game_Speed;
int start_move=0;
int start_angle;
int Angle;
int BallX;
int BallY;
int Ball_Speed;
int playing;

//Drawing function that displays the entire game screen in the state we are in
void Draw() {
    //Draw the bow of the ball
    S2D_DrawQuad(
        750, 0, 0.2, 0.2, 0.2, 1,
        800, 0, 0.2, 0.2, 0.2, 1,
        800, 600, 0.2, 0.2, 0.2, 1,
        750, 600, 0.2, 0.2, 0.2, 1
    );
    //Draw the launch angle
    S2D_DrawQuad(
      730, 590, 1, 0, 0, 1,
      750, 590, 1, 0, 0, 1,
      740, 580, 1, 0, 0, 1,
      730, 590, 1, 0, 0, 1
    );
    //Draw the ball
    S2D_DrawCircle(BallX, BallY, 10, 0, 0, 0,0,0);
    // Draw players name
    S2D_Text *txt = S2D_CreateText("t.ttf", player1, 20);
    txt->x = 100;
    txt->y = 50;
    S2D_DrawText(txt);
    S2D_FreeText(txt);

    S2D_Text *txt = S2D_CreateText("t.ttf", player2, 20);
    txt->x = 150;
    txt->y = 50;
    S2D_DrawText(txt);
    S2D_FreeText(txt);

    //draw players score
    char stext[20];
    sprintf(stext, player_score[0]);
    S2D_Text *txt = S2D_CreateText("t.ttf", stext, 20);
    txt->x = 200;
    txt->y = 50;
    S2D_DrawText(txt);
    S2D_FreeText(txt);

    sprintf(stext, player_score[1]);
    S2D_Text *txt = S2D_CreateText("t.ttf", stext, 20);
    txt->x = 250;
    txt->y = 50;
    S2D_DrawText(txt);
    S2D_FreeText(txt);
    //draw gameSpeed
    sprintf(stext, Game_Speed);
    S2D_Text *txt = S2D_CreateText("t.ttf", stext, 20);
    txt->x = 300;
    txt->y = 50;
    S2D_DrawText(txt);
    S2D_FreeText(txt);
}
void on_key(S2D_Event e){
    //Checking keyboard inputs
    switch (e.type) {
        case S2D_KEY_DOWN:
            //if up key pressed
            if (e.key == S2D_KEY_UP && Angle < 90) Angle++;
            //if down key pressed
            if (e.key == S2D_KEY_DOWN && Angle >0) Angle--;
            //if space key pressed shoot the ball
            if (e.key == 32){
                start_move = 1;
                start_angle = Angle;
            }
            //if S key pressed save data to file
            if (e.key == 83){
                FILE *file = fopen("data.txt", "w");
                fprintf(file, "%s\n", player1);
                fprintf(file, "%s\n", player2);
                fprintf(file, "%d\n", player_score[0]);
                fprintf(file, "%d\n", player_score[1]);
                fprintf(file, "%d\n", Final_Score);
                fprintf(file, "%d\n", Game_Speed);
                fclose(file);
            }
            //if L key pressed load data from file
            if (e.key == 76){
                FILE *file = fopen("data.txt", "r");
                fscanf(file, "%s\n", player1);
                fscanf(file, "%s\n", player2);
                fscanf(file, "%d\n", &player_score[0]);
                fscanf(file, "%d\n", &player_score[1]);
                fscanf(file, "%d\n", &Final_Score);
                fscanf(file, "%d\n", &Game_Speed);
                fclose(file);
            }
        }
}
// AI of game
void Update(){
    int flag = 0;
    // Change the location of the players by ramdom
    int player1Y = rand() % 750;
    int player2Y = rand() % 750;
    player1Y += rand() % 10;
    player2Y += rand() % 10;
    //check the players is not out of screen
    if (player1Y < 0) player1Y = 0;
    else if (player1Y > 750) player1Y = 750;
    if (player2Y < 0) player2Y = 0;
    else if (player2Y > 750) player2Y = 750;
    // if the ball conflict to player 1
    if (BallX <= 50 && BallY >= player1Y && BallY <= player1Y + 50) {
        // change the score
        if (playing == 1) {
            player_score[0] -= 3;
        }
        else {
            player_score[0] += 5;
        }
        flag = 1;
    }
    // if the ball conflict to player 2
    if (BallX <= 50 && BallY >= player2Y && BallY <= player2Y + 50) {
        // change the score
        if (playing == 2) {
            player_score[1] -= 3;
        }
        else {
            player_score[1] += 5;
        }
        flag = 1;
    }
    if (BallX <= 10 || ballX >= 790){
        flag = 1;
    }
    // check the ball is moving or not
    if (start_move){
        //change the position of ball
        BallX += Ball_Speed*cos(start_angle*3.14/180)
        BallY += Ball_Speed*sin(start_angle*3.14/180)
        Ball_Speed -= 0.1;
        // check the ball is out or not
        if (BallY > 600){
            flag = 1;
        }
    }
    //if ball is out or ball conflict to player reset data
    if (flag){
        BallX = 10;
        ballY = 590;
        Ball_Speed = 5 / Game_Speed;
        start_angle = Angle;
        start_move =0;
        if (playing==1)playing=2;
        else playing =1;
    }
    //check the winner
    if (player_score[0]>=Final_Score){
        printf("player1 wins\n");
        S2D_Close(window);
    }
    else if (player_score[1]>=Final_Score){
        printf("player2 wins\n");
        S2D_Close(window);
    }

}
void render() {
    S2D_Clear(1, 1, 1, 1);
    Draw();
}

int main(){
    scanf("%s %s %d %d",player1,player2,&Final_Score,&Game_Speed);
    srand(time(NULL));
    S2D_Window *window = S2D_CreateWindow("Game", 800, 600, Update, render, S2D_RESIZABLE);
    window->on_key = on_key;
    Angle = 0;
    BallX = 10;
    BallY = 790;
    Ball_Speed = 5 / Game_Speed;
    start_angle = Angle;
    start_move = 0;
    playing = 1;
    S2D_Show(window);
    return 0;
}
