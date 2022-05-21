#include <iostream>
#include <random>
#include "raylib.h" 

using namespace std;

struct AnimationData {
    Rectangle rectangle;
    Vector2 position;
    int animationFrame;
    float updateTime;
    float runningTime;
};

// Function to check if the Sprite is on the ground
bool IsOnGround(AnimationData data, int windowHeight) {
    return data.position.y >= windowHeight - data.rectangle.height;
};

// Function for updating the Animation
AnimationData UpdateAnimationData(AnimationData data, float deltaTime, int maxFrameValue) {
    // Update Running Time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime) {
        data.runningTime = 0.0;

        // Update Animation Frame
        data.rectangle.x = data.animationFrame * data.rectangle.width;
        data.animationFrame++;

        if(data.animationFrame > maxFrameValue) {
            data.animationFrame = 0;
        }
    }

    return data;
};

int main() {
    // Window Dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // Initialize the Window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");
    SetTargetFPS(60);

    // Acceleration due to gravity, (pixels/s)/s
    const int gravity{1000};

    // Nebula Textures
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    int nebulaVelocity{-200}; // Nebula x velocity (pixels/second)

    // Nebula Array
    const int nebulaCount = 3;
    AnimationData nebulae[nebulaCount]{};

    // Random Position
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(300,350);
    
    for(int i = 0; i < nebulaCount; i++) {
        auto randomLength = uni(rng);

        nebulae[i].rectangle.x = 0.0;
        nebulae[i].rectangle.y = 0.0;
        nebulae[i].rectangle.width = static_cast<float>(nebula.width/8.0);
        nebulae[i].rectangle.height = static_cast<float>(nebula.height/8.0);
        nebulae[i].position.y = static_cast<float>(windowDimensions[1]) - static_cast<float>(nebula.height/8.0);
        nebulae[i].animationFrame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].position.x = windowDimensions[0] + (i * randomLength);
    }

    // Finish Line
    float finishLine{nebulae[nebulaCount - 1].position.x + 200};
    bool collision{};

    // Sprite Variable Creations
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimationData scarfyData;
    scarfyData.rectangle.width = scarfy.width / 6;
    scarfyData.rectangle.height = scarfy.height;
    scarfyData.rectangle.x = 0;
    scarfyData.rectangle.y = 0;
    scarfyData.position.x = (windowDimensions[0]/2) - (scarfyData.rectangle.width/2);
    scarfyData.position.y = windowDimensions[1] - scarfyData.rectangle.height;
    scarfyData.animationFrame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    // Jumping Variables, Pixels per secon
    const int jumpVelocity{-600};
    bool isInAir{false};
    int velocity{0};

    // Background Variables
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    // Midground Variables
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    // Foreground Variables
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    // Keeping the Window Open
    while(!WindowShouldClose()) {
        // Delta Time
        const float dt{ GetFrameTime() };

        // Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Update the background
        bgX -= 20 * dt;
        if(bgX <= -background.width * 2) {
            bgX = 0.0;
        }

        // Draw the Background
        Vector2 bg1Position{bgX, 0.0};
        DrawTextureEx(background, bg1Position, 0.0, 2.0, WHITE);
        Vector2 bg2Position{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg2Position, 0.0, 2.0, WHITE);

        // Update the midground
        mgX -= 40 * dt;
        if(mgX <= -midground.width * 2) {
            mgX = 0.0;
        }

        // Draw the Midground
        Vector2 mg1Position{mgX, 0.0};
        DrawTextureEx(midground, mg1Position, 0.0, 2.0, WHITE);
        Vector2 mg2Position{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Position, 0.0, 2.0, WHITE);

        // Update the Foreground
        fgX -= 80 * dt;
        if(fgX <= -foreground.width * 2) {
            fgX = 0.0;
        }

        // Draw the Foreground
        Vector2 fg1Position{fgX, 0.0};
        DrawTextureEx(foreground, fg1Position, 0.0, 2.0, WHITE);
        Vector2 fg2Position{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Position, 0.0, 2.0, WHITE);

        // Ground Check
        if(IsOnGround(scarfyData, windowDimensions[1])) {
            // Rectangle is on ground
            velocity = 0;
            isInAir = false;
        } else {
            // Rectangle is in the air
            velocity += gravity * dt;
            isInAir = true;
        }

        // Jump
        if(IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity += jumpVelocity;
        }

        for(int i = 0; i < nebulaCount; i++) {
            nebulae[i].position.x += nebulaVelocity * dt;
        }

        // Update Finish Line
        finishLine += nebulaVelocity * dt;

        // Update Scarfy Y Position
        scarfyData.position.y += velocity * dt;

        // Sprite Animation Update
        if(!isInAir) {
            scarfyData = UpdateAnimationData(scarfyData, dt, 5);
        }

        // Update Nebula Animation Frame
        for(int i = 0; i < nebulaCount; i++) {
            nebulae[i] = UpdateAnimationData(nebulae[i], dt, 7);
        }

        
        for(AnimationData neb : nebulae) {
            float pad{50};
            Rectangle nebRec {
                neb.position.x + pad,
                neb.position.y + pad,
                neb.rectangle.width - (pad * 2),
                neb.rectangle.height - (pad * 2)
            };
            Rectangle scarfyRec{
                scarfyData.position.x,
                scarfyData.position.y,
                scarfyData.rectangle.width,
                scarfyData.rectangle.height
            };

            if(CheckCollisionRecs(nebRec,scarfyRec)) {
                collision = true;
            }
        }

        if(collision) {
            // Lose the game
            DrawText("Game Over!", windowDimensions[0]/4,windowDimensions[1]/2,40,WHITE);
        } else if(scarfyData.position.x >= finishLine) {
            // Win the game
            DrawText("You Win!!!", windowDimensions[0]/4,windowDimensions[1]/2,40,WHITE);
            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rectangle, scarfyData.position, WHITE);
        } else {
            // Draw Nebula
            for(int i = 0; i < nebulaCount; i++) {
                DrawTextureRec(nebula, nebulae[i].rectangle, nebulae[i].position,WHITE);
            }

            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rectangle, scarfyData.position, WHITE);
        }

        // End Drawing
        EndDrawing();
    }

    // Unload the Textures
    UnloadTexture(nebula);
    UnloadTexture(scarfy);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    // Close the Window
    CloseWindow();
}

