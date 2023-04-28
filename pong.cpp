#include <stdio.h>
#include "raylib.h"

struct Ball //define as propriedades da bola
{
    float x, y;
    float speedX, speedY;
    float radius;
    
    void Draw()
    {
        DrawCircle((int)x, (int)y, radius, WHITE);
    }
};

struct Paddle //Define as propriedades do retângulo
{
  float x,y;
  float speed;
  float width, height;
  
  Rectangle GetRect()
  {
     return Rectangle {x - width / 2, y - height / 2, 10, 100}; 
  }
  
  void Draw()
  {
      DrawRectangleRec(GetRect(), WHITE);
      
  }
};

void checkBallAndRightRectangleColision(Ball *ball, Paddle *rightPaddle, Sound sound) //checa a colisão entre a bola e a raquete direita
{
    if(CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius, rightPaddle->GetRect()))
            {
                PlaySound(sound);
                if(ball->speedX > 0)
                {
                   ball->speedX *= -1.1f;
                    
                }
                
            }
}

void checkBallAndLeftRectangleColision(Ball *ball, Paddle *leftPaddle, Sound sound) //checa a colisão entre a bola e a raquete esquerda
{
    if(CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius, leftPaddle->GetRect()))
            {
                PlaySound(sound); //som toca a cada batida na raquete
                
              if(ball->speedX < 0)
                {
                   ball->speedX *= -1.1f;
                    
                }  
            }
            
}

void leftPlayerScore(Ball *ball, int *jogador2) //marca a pontuação do jogador esquerdo quando a bola atinge uma posição maior do que o eixo x
{
    if(ball->x >= GetRenderWidth()){
            *jogador2 += 1;
            ball->x = GetScreenWidth()/2;
            ball->y = GetScreenHeight() / 2;
            ball->speedX = 300;
            ball->speedY = 300;
            
            ball->speedX *= -1;
            
        }
}


void rightPlayerScore(Ball *ball, int *jogador1)
{
    if(ball->x <= 0){
            *jogador1 += 1;
            ball->x = GetScreenWidth()/2;
            ball->y = GetScreenHeight() / 2;
            ball->speedX = 300;
            ball->speedY = 300;
            
            ball->speedX *= 1;   
        }
}

void ballTopWallColision(Ball *ball) //checa a colisão da bola com a "parede de cima"
{
    if(ball->y < 0){
            ball->y = 0;
            ball->speedY *=-1;
        }
}

void ballBottomWallColision(Ball *ball)
{
   if(ball->y > GetScreenHeight()){
            ball->y = GetScreenHeight();
            ball->speedY *= -1;
        } 
}


int main() 
{
	InitWindow(800, 600, "pong"); //daqui pra baixa são definições de variáveis e propriedades dos objetos, como posição nos eixos e velocidade
	SetWindowState(FLAG_VSYNC_HINT);
    
    bool Paused = false;
    bool Win = false;

    Ball ball;
    
    int jogador1 = 0, jogador2 = 0;
    
    ball.x = GetScreenWidth() / 2.0f;
    ball.y = GetScreenHeight() / 2.0f;
    ball.radius = 5;
    ball.speedX = 300;
    ball.speedY = 300;
    
    Paddle leftPaddle;
    leftPaddle.x = 50;
    leftPaddle.y = GetScreenHeight() / 2;
    leftPaddle.width = 10;
    leftPaddle.height = 100;
    leftPaddle.speed = 500;
    
    Paddle rightPaddle;
    rightPaddle.x = GetScreenWidth()  - 50;
    rightPaddle.y = GetScreenHeight() / 2;
    rightPaddle.width = 10;
    rightPaddle.height = 300;
    rightPaddle.speed = 500;
    
    InitAudioDevice();
    
    Sound sound = LoadSound("assets/raquete.wav");
    

	while (!WindowShouldClose())
	{
        if(!Paused and !Win) //a lógica de pausa do jogo, enquanto a variável booleana "Paused" for false, 
                             //tudo que tá dentro desse if, é o que faz a movimentação do jogo, por isso que quando ela fica verdadeira, o jogo para
        {
        ball.x += ball.speedX * GetFrameTime(); //velocidade da bola no eixo x multiplicada pelo frame da tela "cada tela tem um fps diferente, e isso adapta cada tela"        ball.y += ball.speedY * GetFrameTime();
        
        ballBottomWallColision(&ball); //chama a função que chega a colisão com as paredes de cima e de baixo
        ballTopWallColision(&ball);
 
        
        if(IsKeyDown(KEY_W)){ //checa os eventos de pressionamento de teclas para movimentar as raquetes
            leftPaddle.y -= leftPaddle.speed * GetFrameTime();
        }
        
        if(IsKeyDown(KEY_S)){
            leftPaddle.y += leftPaddle.speed * GetFrameTime();
        }
        
        if(IsKeyDown(KEY_UP)){
            rightPaddle.y -= rightPaddle.speed * GetFrameTime();
        }
        
        if(IsKeyDown(KEY_DOWN)){
            rightPaddle.y += rightPaddle.speed * GetFrameTime();
        }
        
        leftPlayerScore(&ball, &jogador2); //chama a função de pontuação do jogador esquerdo
        rightPlayerScore(&ball, &jogador1); //chama a função de pontuação do jogador direito
        
        checkBallAndRightRectangleColision(&ball, &rightPaddle, sound); //checa colisão da bola com a raquete, e faz ela voltar 
        
        checkBallAndLeftRectangleColision(&ball, &leftPaddle, sound); 
     
            
        }
        
        if(jogador1 == 5) //pontuação de vitória para o jogador
        {
            Win = true;
            DrawText("Jogador 1 venceu!", GetScreenWidth() / 2 - 200,GetScreenHeight() / 2 - 50, 30, GREEN );
            DrawText("Aperte SPACE para jogar novamente",GetScreenWidth() / 2 - 200,GetScreenHeight() / 2 +50, 30, GREEN );      
            
        }
        
        if(jogador2 == 5)
        {
            Win = true; 
            DrawText("Jogador 2 venceu!",GetScreenWidth() / 2 - 200,GetScreenHeight() / 2 - 50, 30, GREEN );
            DrawText("Aperte SPACE para jogar novamente",GetScreenWidth() / 2 - 200,GetScreenHeight() / 2 +50, 30, GREEN );            
            
        }
        
        if(IsKeyPressed(KEY_Q))
            {
                Paused = !Paused;
            }
            
         
            
        if(IsKeyPressed(KEY_SPACE))
        {
        
            ball.x = GetScreenWidth() / 2.0f;
            ball.y = GetScreenHeight() / 2.0f;
            
            leftPaddle.x = 50;
            leftPaddle.y = GetScreenHeight() / 2;
            
            rightPaddle.x = GetScreenWidth()  - 50;
            rightPaddle.y = GetScreenHeight() / 2;
            
            Win = false;
            
            jogador1 = 0;
            jogador2 = 0;
            
        }
      
            
        if(Paused)
        {
            DrawText("Paused",GetScreenWidth() / 2 - 50,GetScreenHeight() / 2, 40, GREEN);
        }
        
        
        
		BeginDrawing();
		ClearBackground(BLACK);
        
		ball.Draw();
        leftPaddle.Draw();
        rightPaddle.Draw();
        
        DrawText(TextFormat("%i", jogador1), 800/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", jogador2), 2400/4 - 20, 20, 80, WHITE);

		DrawFPS(10, 10);
		EndDrawing();


	}
    
   
    UnloadSound(sound);     // fecha a função do som

    CloseAudioDevice();     // fecha o audio do dispositivo


	CloseWindow();
	return 0;
}