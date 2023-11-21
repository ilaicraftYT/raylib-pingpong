#include "raylib.h"

struct Ball
{
  Vector2 position;
  Vector2 speed = {6.0f, 5.0f};
  int ballRadius = 15;
};

struct Paddle
{
  Vector2 position;
  float width = 15.0f;
  float height;
};

void ProcessPhysics(float deltaTime, Ball &ball, Paddle left_paddle, Paddle right_paddle, bool &gameOver)
{
  ball.position.x += ball.speed.x;
  ball.position.y += ball.speed.y;

  // Check paddles collision for bouncing
  if (CheckCollisionCircleRec(ball.position, ball.ballRadius, {left_paddle.position.x, left_paddle.position.y, left_paddle.width, left_paddle.height}))
  {
    ball.speed.x *= -1.0f;
  }
  if (CheckCollisionCircleRec(ball.position, ball.ballRadius, {right_paddle.position.x, right_paddle.position.y, right_paddle.width, right_paddle.height}))
  {
    ball.speed.x *= -1.0;
  }

  // Check walls collision for bouncing
  if ((ball.position.x >= (GetScreenWidth() - ball.ballRadius)) || (ball.position.x <= ball.ballRadius))
  {
    gameOver = true;
    ball.speed.x *= -1.0f;
  }
  if ((ball.position.y >= (GetScreenHeight() - ball.ballRadius)) || (ball.position.y <= ball.ballRadius))
    ball.speed.y *= -1.0f;
}

void Draw(Ball ball, Paddle left_paddle, Paddle right_paddle)
{
  // Split screen in two
  DrawRectangle(GetScreenWidth() / 2 - 2, 0, 2, GetScreenHeight(), LIGHTGRAY);

  // Draw ball
  DrawCircleV(ball.position, (float)ball.ballRadius, MAROON);

  // Draw paddles
  DrawRectangleV(left_paddle.position, {left_paddle.width, left_paddle.height}, DARKGRAY);
  DrawRectangleV(right_paddle.position, {right_paddle.width, right_paddle.height}, DARKGRAY);
}

void HandleKeyboard(Paddle &left_paddle, Paddle &right_paddle)
{
  if (IsKeyDown(KEY_W))
    left_paddle.position.y -= 5.0f;
  if (IsKeyDown(KEY_S))
    left_paddle.position.y += 5.0f;
  if (IsKeyDown(KEY_UP))
    right_paddle.position.y -= 5.0f;
  if (IsKeyDown(KEY_DOWN))
    right_paddle.position.y += 5.0f;
}

int main()
{
  float deltaTime = 0.0f;
  int screenWidth = 640;
  int screenHeight = 480;
  bool gameOver = false;
  Ball game_ball;
  Paddle left_paddle;
  Paddle right_paddle;

  InitWindow(screenWidth, screenHeight, "Pong");
  SetTargetFPS(60);

  game_ball.position = {(float)GetScreenWidth() / 2, (float)game_ball.ballRadius}; // first ball position
  // sets paddle heights
  left_paddle.height = (float)GetScreenHeight() / 4;
  right_paddle.height = (float)GetScreenHeight() / 4;

  // first paddle position
  left_paddle.position = {10, (float)GetScreenHeight() / 2 - left_paddle.height / 2};
  right_paddle.position = {(float)GetScreenWidth() - 20.0f, (float)GetScreenHeight() / 2 - right_paddle.height / 2};

  while (WindowShouldClose() != true)
  {
    deltaTime = GetFrameTime();
    if (gameOver != false)
    {
      BeginDrawing();
      if (game_ball.position.x >= GetScreenWidth() - game_ball.ballRadius)
      {
        int textWidth = MeasureText("Left Won", 50);
        DrawText("Left Won", GetScreenWidth() / 2.0f - textWidth, GetScreenHeight() / 2.0f - textWidth, 50, RED);
      }
      else
      {
        int textWidth = MeasureText("Right Won", 50);
        DrawText("Right Won", GetScreenWidth() / 2.0f - textWidth, GetScreenHeight() / 2.0f - textWidth, 50, BLUE);
      }
      EndDrawing();
      continue;
    }
    ProcessPhysics(deltaTime, game_ball, left_paddle, right_paddle, gameOver);
    HandleKeyboard(left_paddle, right_paddle);
    BeginDrawing();
    ClearBackground(WHITE);
    Draw(game_ball, left_paddle, right_paddle);
    EndDrawing();
    DrawFPS(20, 20);
  };

  return 0;
};
