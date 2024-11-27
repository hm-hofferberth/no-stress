// Authors: Hannah Hofferberth and Pierre van Zyl

#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "FEHRandom.h"
#define JUMPSPEED 0.06

/* Class for buttons on the menu page

Variables:
- xPos and yPos describe the button's position
- xSize and ySize describe the button's size
- image and clickedImage are the images for the button
- screenToInvoke is the screen to change to when button is clicked
- clicked is whether or not the button has been clicked

Functions:
-  Constructor initializes the button and fills each field with info
-  callScreen returns the screen associated with the button
-  draw() draws the button and returns if it has been clicked
-  draw(float, float) draws the button and checks if it's being clicked

Written by both Hannah and Pierre
*/

class Button{
    private:
        float xPos;
        float yPos;
        float xSize;
        float ySize;
        FEHImage image;
        FEHImage clickedImage;
        int screenToInvoke;
        bool clicked = false;
    public:
        // Constructor written by Hannah
        Button(char img[], char img2[], float x, float y, float xS, float yS, int screen){
            image.Open(img);
            clickedImage.Open(img2);
            screenToInvoke = screen;
            xSize = xS;
            ySize = yS;
            xPos = x;
            yPos = y;
        }

        // Written by Hannah
        // Returns screen associated with the button
        int callScreen(){
            return screenToInvoke;
        } 


        // Overloaded method
        // Written by Hannah
        // Returns true if it has been clicked, false if not. Draws button if not.
        bool draw(){
                if(!clicked){
                    image.Draw(xPos, yPos);
                    return false;
                }else{
                    clicked = false;
                    return true;
                }
                
            
        }

        // Written by Hannah and Pierre
        // Draws button and checks if it's being clicked.
        void draw(float xClicked, float yClicked){
            if(xClicked > xPos && xClicked < (xPos+xSize) && yClicked > yPos && yClicked < (yPos+ySize)){
                clickedImage.Draw(xPos, yPos);
                clicked = true;
            }else{
                image.Draw(xPos, yPos);
            }
            
        }
        

};

/* Class that describes the character

Variables:
- stressIndex is how many times the character has hit an obstacle
- xPos is how far the character has moved
- yPos is the character's y position
- jumpIndex is a number denoting where the character is in its jump
- colliding is an int denoting if a character has just collided and should thus have the red/green flash costume
- image is the image associated with the character

Functions:
- changeCostume sets the image to the parameter
- drawChar draws the character at its current position
- transitionJump changes yPos based on the character's point in the jump

Written by Hannah
*/
class Character{
    public:
        float stressIndex = 0;
        float xPos = 0;
        float yPos = 85;
        int jumpIndex = 0;
        int colliding = 0;
        FEHImage image;

        // Changes image of character to costume[]
        void changeCostume(char costume[]){
            image.Open(costume);
        }

        // Draws the character
        void drawChar(){
            image.Draw(30, yPos);
        }

        // Changes yPos based on the character's point in the jump. Ends jump if reached the ground.
        void transitionJump(int maxIndex){
            // Changes the max height based on the stress level
            if(maxIndex > (75 - (stressIndex * 5))){
                maxIndex = 75 - (stressIndex * 5);
            }

            if(jumpIndex < maxIndex){
                yPos -= (maxIndex - jumpIndex) * (JUMPSPEED);
                jumpIndex++;
            }else{
                if(yPos <= 85){
                    yPos+= (jumpIndex - maxIndex) * (JUMPSPEED);
                    jumpIndex++;
                }else{
                    jumpIndex = 0;
                }
            }
        }

    

};

/* Class that describes the ground

Variables:
- position is the current x position of the Ground
- image is the image associated with the Ground

Functions:
- Constructor initializes the Ground with the correct image
- drawGround draws the Ground at the current position

Written by Hannah
*/
class Ground{
    public:
        float position;
        FEHImage image;
        
        // Initializes all Ground with the same image
        Ground(){
            image.Open("Ground/Ground-1.png");
        }
        
        // Draws ground at current position
        void drawGround(){
           image.Draw(position,0);
       }
        

};

/* Class that describes the Background

Variables:
- position is the current x position of the Background
- image is the image associated with the Background

Functions:
- Constructor initializes the Background with the correct image
- drawGround draws the Background at the current position

Written by Hannah
*/
class Background{
    public:
        float position;
        FEHImage image;
        
        // Initializes all Background with the same image
        Background(){
            image.Open("Backgrounds/BackgroundWClouds.png");
        }

        // Draws background at current position
        void drawGround(){
           image.Draw(position,0);
       }
        

};

/* Class that describes the obstacles (both good and bad)

Variables:
- xPos and yPos are position of obstacle
- generated is a boolean describing whether or not the obstacle is visible and can collide
- imageName is the name of the obstacle's image
- image is the image that draws the obstacle

Functions:
- Constructor initializes the Obstacle with an arbitrary image
- draw draws the obstacle at its current position

Written by Pierre
*/
class Obstacle{
    public:
        float xPos;
        float yPos;
        bool generated = false;
        char imageName[30] = "";
        FEHImage image;
        
        // Initializes obstacle at point off the screen with arbitrary image
        Obstacle(){
            image.Open("obstacles/AlarmClock.png");
            xPos = 400;
            yPos = 0;
        }

        // Draws obstacle at current position
        void draw(){
            image.Draw(xPos, yPos);
        }
};

/* Class that describes the jump bar

Variables:
- totalX is the total width of bar
- totalY is the height of the bar
- innerX is the width of the blue bar
- xPos and yPos are the position of the bar

Functions:
- resetBar draws the bar when the screen is not being clicked
- increaseBar draws the bar when the screen is being clicked

Written by Hannah
*/
class JumpBar{
    private:
        float totalX = 75;
        float totalY = 20;
        float innerX = 0;
        float xPos = 15;
        float yPos = 205;

    public:
        // If screen is not currently being clicked
        void resetBar(int stressIndex){
            innerX = 0;
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(xPos,yPos,totalX,totalY); 

            // Part of bar affected by stress
            LCD.SetFontColor(RED);
            LCD.FillRectangle(totalX - (stressIndex * 5) + 15,yPos,(stressIndex * 5),totalY); 
        }

        void increaseBar(int max, int stressIndex){
            // Increase size of blue part of bar
            if(innerX < max){
                innerX++;
            }
            
            // Draw bar
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(xPos,yPos,totalX,totalY); 
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(xPos,yPos,innerX,totalY); 
            
            // Draw part of bar affected by stress
            LCD.SetFontColor(RED);
            LCD.FillRectangle(totalX - (stressIndex * 5) + 15,yPos,(stressIndex * 5),totalY); 
        }
};


// Written by Pierre
// Function for effects of collision with an obstacle
void collideObstacle(Obstacle *hitObstacle, Character *hitPlayer, int *screen) {
    hitObstacle->generated = false;
    hitObstacle->xPos = -251;
    hitPlayer->stressIndex++;

    (*hitPlayer).changeCostume("Collided.png");
    (*hitPlayer).colliding = 15;

    // End game
    if((*hitPlayer).stressIndex > 5){
        (*screen) = 6;
    }
}

// Written by Pierre
// Function for effects of collision with a good object
void collideObject(Obstacle *hitObject, Character *hitPlayer, int *screen) {
    hitObject->generated = false;
    hitObject->xPos = -251;
    // Give stress back
    if(hitPlayer->stressIndex > 0){
        hitPlayer->stressIndex--;
    }

    (*hitPlayer).changeCostume("Healed.png");
    (*hitPlayer).colliding = 15;
}


// Written by Hannah
// Updates high score
void checkScore(float *score, float *maxScore){
    if(*score > *maxScore){
        *maxScore = *score;
    }        
}

// Main method
// Written by both Hannah and Pierre
int main()
{
    // 1: menu, 2: stats, 3: credits, 4: instructions, 5: game, 6: game over
    int screen = 1;

    // Variables for the menu
    Button startButton("Buttons/sprite_0.png", "Buttons/sprite_1.png", 90, 150, 60, 25, 5);
    Button statsButton("Buttons/sprite_2.png", "Buttons/sprite_3.png", 65, 190, 60, 25, 2);
    Button creditsButton("Buttons/sprite_4.png", "Buttons/sprite_5.png", 160, 190, 60, 25, 3);
    Button infoButton("Buttons/sprite_6.png", "Buttons/sprite_7.png", 160, 150, 60, 25, 4);


    // Variables for the game
    Character player;
    char crouches[6][35] = {"Char_crouch/sprite_0_crouch.png", "Char_crouch/sprite_2_crouch.png", "Char_crouch/sprite_4_crouch.png", "Char_crouch/sprite_6_crouch.png", "Char_crouch/sprite_8_crouch.png", "Char_crouch/sprite_10_crouch.png"};
    char stands[6][35] = {"character/sprite_00.png", "character/sprite_02.png", "character/sprite_04.png", "character/sprite_06.png", "character/sprite_08.png", "character/sprite_10.png"};
    char jumps[6][35] = {"character/sprite_01.png", "character/sprite_03.png", "character/sprite_05.png", "character/sprite_07.png", "character/sprite_09.png", "character/sprite_11.png"};
    player.changeCostume(stands[0]);

    FEHImage background;
    background.Open("Backgrounds/BlueBackground-1.png");

    float score = 0;
    float maxScore = 0;
    int runsPlayed = 0;
 
    Ground currGround [3];
    Background currBackground [3];

    currGround[0].position = 0;
    currGround[1].position = 300;
    currGround[2].position = 600;

    currBackground[0].position = 0;
    currBackground[1].position = 300;
    currBackground[2].position = 600;

    Obstacle currentObstacles[15];
    Obstacle currentObjects[15]; // "Objects" refer to the good obstacles
    
    char objectImages[7][30] = {"objects/Bed.png", "objects/Heart.png","objects/Coffee.png","objects/Outside.png", "objects/Sports.png", "objects/Call.png", "objects/Journal.png"};
    char obstacleImages[12][30] = {"obstacles/AlarmClock.png",
    "obstacles/Bill.png", "obstacles/Cell_Phone.png", "obstacles/Clock.png", 
   "obstacles/books.png", "obstacles/Thunder.png", "obstacles/paper1.png","obstacles/Application.png", 
   "obstacles/messages.png","obstacles/News.png","obstacles/Email.png", "obstacles/Phone2.png"};

    JumpBar bar;

    int currObjectGenerated = 0;
    int currObstacleGenerated = 0;
    float lastGeneratedX = 100;
    float lastObGeneratedX = 200;
    float currGenerationDistance = 50;
    float currObGenerationDistance = 1550;
    float currObstacleGenMax = 390;

    float timeHeld = 0;
    float moveSpeed = 0;
    int jumpLevel = 50;

    bool resetTime = false;

    // Ensures no double click
    bool released = false;

    while (1) {
        // If game needs to be reset, set variables back
        if(resetTime){
            runsPlayed++;
            released = false;
            
            currObjectGenerated = 0;
            currObstacleGenerated = 0;
            lastGeneratedX = 100;
            lastObGeneratedX = 200;
            currGenerationDistance = 50;
            currObGenerationDistance = 1550;
            currObstacleGenMax = 390;

            timeHeld = 0;
            moveSpeed = 0;
            jumpLevel = 50;

            currGround[0].position = 0;
            currGround[1].position = 300;
            currGround[2].position = 600;

            currBackground[0].position = 0;
            currBackground[1].position = 300;
            currBackground[2].position = 600;

            player.colliding = false;
            player.stressIndex = 0;
            player.xPos = 0;
            player.yPos = 85;
            
            score = 0;
            timeHeld = 0;
            player.changeCostume(stands[0]);

            for(int i = 0; i < 15; i++){
                currentObstacles[i].generated = false;
                
            }

            for(int i = 0; i < 15; i++){
                currentObjects[i].generated = false;
                
            }

            resetTime = false;
        }
        
        // Menu
        if(screen == 1){
          // background
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(0,0,319,239); 

            // Declares an image for logo
            FEHImage logo;
            // Open the image
            logo.Open("Logo-2.png.png");
            // Draw the logo
            logo.Draw(45, 10);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos,false) && moveSpeed == 0){
                startButton.draw(x_pos, y_pos);
                infoButton.draw(x_pos, y_pos);
                statsButton.draw(x_pos, y_pos);
                creditsButton.draw(x_pos, y_pos);
                
            }else{
                if(released){
                    if(startButton.draw()){
                    screen = startButton.callScreen();
                    }
                    if(infoButton.draw()){
                        screen = infoButton.callScreen();
                    }
                    if(statsButton.draw()){
                        screen = statsButton.callScreen();
                    }
                    if(creditsButton.draw()){
                        screen = creditsButton.callScreen();
                    }
                }else{
                    startButton.draw();
                    infoButton.draw();
                    statsButton.draw();
                    creditsButton.draw();
                    released = true;
                }
                
            }

            
        // Actual game
        }else if(screen == 5){
            // background
            background.Draw(0,0);

            // score
            LCD.SetFontColor(WHITESMOKE);
            int placesLeft = 0;
            if(((int)score / 100) / 10 != 0){
                placesLeft = 1;
                if(((int)score / 100) / 100 != 0){
                    placesLeft = 2;
                }
                if(((int)score / 100) / 1000 != 0){
                    placesLeft = 3;
                }
            }

            LCD.WriteAt((int)score / 100, 290 - placesLeft * 10, 10);

            
            // sprites

            // Ground
            currGround[0].drawGround();
            currGround[1].drawGround();
            currGround[2].drawGround();
            // Background
            currBackground[0].drawGround();
            currBackground[1].drawGround();
            currBackground[2].drawGround();
            // Player
            player.drawChar();

            // check if screen clicked
            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos,false) && moveSpeed == 0){
                timeHeld++;
                bar.increaseBar(75 - (player.stressIndex * 5), player.stressIndex);
                if(player.colliding == 0){
                    player.changeCostume(crouches[(int)player.stressIndex]);
                }else{
                    player.colliding --;
                }
                
            }else{ // Not currently clicked, or is currently jumping
                
                // Detecting jump status
                if(timeHeld != 0){

                    // Set image to correct jumping sprite
                    player.changeCostume(jumps[(int)player.stressIndex]);
                    // Set jump info
                    jumpLevel = timeHeld;
                    timeHeld = 0;
                    moveSpeed = 1.25;
                    player.jumpIndex = 1;

                }else if(player.jumpIndex != 0){
                    // Mid jump
                    if(player.colliding == 0){ // If hasn't just hit an object/obstacle
                        player.changeCostume(jumps[(int)player.stressIndex]);
                    }else{
                        player.colliding --; // Decrease amount of time left to flash green/red
                    }

                    // Move character and transition where it is in its jump
                    player.transitionJump(jumpLevel); 
                    player.xPos += moveSpeed;
                    score += moveSpeed;

                    // Reset xPos every so often so it never gets too large
                    if(player.xPos > 2000){
                        player.xPos = 0;
                        lastGeneratedX = 0;
                        lastObGeneratedX = 0;
                    }
                    
                }else{ // Jump is done
                    moveSpeed = 0;
                    if(player.colliding == 0){
                        player.changeCostume(stands[(int)player.stressIndex]);
                    }else{
                        player.colliding --;
                    }
                }

                // Set bar back to normal (no inner blue bar)
                bar.resetBar(player.stressIndex);
                
            }


            // Ground and Background adjustment
            currGround[0].position -= moveSpeed;
            currGround[1].position -= moveSpeed;
            currGround[2].position -= moveSpeed;
            currBackground[0].position -= moveSpeed/4;
            currBackground[1].position -= moveSpeed/4;
            currBackground[2].position -= moveSpeed/4;

            // If they've gone off the screen, reset them to the other side
            if(currGround[0].position <= -300){
                currGround[0].position = 600;
            }
            if(currGround[1].position <= -300){
                currGround[1].position = 600;
            }
            if(currGround[2].position <= -300){
                currGround[2].position = 600;
            }

            if(currBackground[0].position <= -300){
                currBackground[0].position = 600;
            }
            if(currBackground[1].position <= -300){
                currBackground[1].position = 600;
            }
            if(currBackground[2].position <= -300){
                currBackground[2].position = 600;
            }

            // Change obstacle generation distance (gradually makes game harder)
            if(currObstacleGenMax > 80){
                currObstacleGenMax -= 0.03;
            }

            // Generate good objects
            if(player.xPos - lastObGeneratedX > currObGenerationDistance){ //If it's time for a new object to be generated

                currentObjects[currObjectGenerated].xPos = 350;
                currentObjects[currObjectGenerated].yPos = 155;

                // Randomize which object will appear
                int random = 7 * (Random.RandInt() / 32767.0);
                currentObjects[currObjectGenerated].image = objectImages[random];
                strcpy(currentObjects[currObjectGenerated].imageName, objectImages[random]);
                // Set its status to "generated" (will appear and collide)
                currentObjects[currObjectGenerated].generated = true;
                
                // Depending on which was generated, sets y-pos
                 if(random == 1 || random == 2 || random == 3){
                    currentObjects[currObjectGenerated].yPos = 100 * (Random.RandInt() / 32767.0) + 55;
                 }else if(random == 0){
                    currentObjects[currObjectGenerated].yPos = 120;
                 }else if(random == 4 || random == 5|| random == 6 || random == 7){
                    currentObjects[currObjectGenerated].yPos = 80 * (Random.RandInt() / 32767.0) + 35;
                 }

                if(currObjectGenerated < 14){ // If the array of current objects isn't full, move to next element for next time
                    currObjectGenerated++;
                }else{ // If the array is full, start back from the beginning (replace objects that have gone off the screen)
                    currObjectGenerated = 0;
                }
                
                lastObGeneratedX = player.xPos; // Note where last object was generated

                // Choose next distance at which to generate object (randomized)
                float randomDistance = 1250 * (Random.RandInt() / 32767.0) + 20;
                currObGenerationDistance = randomDistance;
            }

            // Generate obstacles
            if(player.xPos - lastGeneratedX > currGenerationDistance){ //If it's time for a new obstacle to be generated
               
                currentObstacles[currObstacleGenerated].xPos = 350;
                currentObstacles[currObstacleGenerated].yPos = 155;

                // Randomize which obstacle will appear
                int random = 12 * (Random.RandInt() / 32767.0);
                currentObstacles[currObstacleGenerated].image = obstacleImages[random];
                strcpy(currentObstacles[currObstacleGenerated].imageName, obstacleImages[random]);
                // Set its status to "generated" (will appear and collide)
                currentObstacles[currObstacleGenerated].generated = true;

                // Depending on which was generated, sets y-pos
                if(random == 5){
                    currentObstacles[currObstacleGenerated].yPos = 140 * (Random.RandInt() / 32767.0) - 40;
                }else if(random == 4){
                    currentObstacles[currObstacleGenerated].yPos = 120;
                }else if(random == 8 || random == 9 || random == 10){
                    currentObstacles[currObstacleGenerated].yPos = 100 * (Random.RandInt() / 32767.0);
                }else if(random == 7 || random == 6){
                    currentObstacles[currObstacleGenerated].yPos = 147;
                }else if(random == 11){
                    currentObstacles[currObstacleGenerated].yPos = 147;
                }

                if(currObstacleGenerated < 14){ // If the array of current obstacles isn't full, move to next element for next time
                    currObstacleGenerated++;
                }else{ // If the array is full, start back from the beginning (replace obstacles that have gone off the screen)
                    currObstacleGenerated = 0;
                }
                
                lastGeneratedX = player.xPos; // Note where last obstacle was generated

                // Choose next distance at which to generate obstacle (randomized)
                float randomDistance = currObstacleGenMax * (Random.RandInt() / 32767.0) + 40 + (currObstacleGenMax / 40);
                currGenerationDistance = randomDistance;
            }

            // Draw objects
            for(int i = 0; i < 15; i++){
                if(currentObjects[i].generated){
                    currentObjects[i].xPos -= moveSpeed;
                    currentObjects[i].draw();

                    if(currentObjects[i].xPos < -250){
                        currentObjects[i].generated = false;
                    }
                }
                
            }

            // Draw obstacles
            for(int i = 0; i < 15; i++){
                if(currentObstacles[i].generated){
                    currentObstacles[i].xPos -= moveSpeed;
                    currentObstacles[i].draw();

                    if(currentObstacles[i].xPos < -250){
                        currentObstacles[i].generated = false;
                    }
                }
                

            }

            // Check collisions

            // Check obstacles
            for(int i = 0; i < 15; i++){
                if (currentObstacles[i].generated) {
                    if (strcmp(currentObstacles[i].imageName, obstacleImages[0]) == 0) { // AlarmClock
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[1]) == 0) { // Bill
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[2]) == 0) { // Cell_Phone
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[3]) == 0) { // Clock
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[4]) == 0) { // books
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 40) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[5]) == 0) { // Thunder can have y
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[6]) == 0) { // paper1
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[7]) == 0) { // Application
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[8]) == 0) { // messages can have y
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[9]) == 0) { // News can have y
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[10]) == 0) { // Email can have y
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[11]) == 0) { // Phone2
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            collideObstacle(&currentObstacles[i], &player, &screen);
                            if(player.stressIndex > 5){
                                checkScore(&score, &maxScore);
                            }
                        }
                    }
                }
            }

            // Check objects
            for (int i = 0; i < 15; i++) {
                if (currentObjects[i].generated) {
                    if (strcmp(currentObjects[i].imageName, objectImages[0]) == 0) { // Bed
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 60 && player.yPos > 60) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[1]) == 0) { // Heart can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[2]) == 0) { // Coffee can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[3]) == 0) { // Outside can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[4]) == 0) { // Sports can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[5]) == 0) { // Call can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }
                        
                    } else if (strcmp(currentObjects[i].imageName, objectImages[6]) == 0) { // Journal can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            collideObject(&currentObjects[i], &player, &screen);
                        }
                    }
                }
            }


        }else if(screen == 2){ // stats
            // background
            LCD.SetFontColor(LIGHTPINK);
            LCD.FillRectangle(0,0,319,239);

            // add text
            LCD.SetFontColor(MAROON);
            LCD.WriteAt("High Score:",70,120);
            LCD.WriteAt((int)maxScore/100,210,120);

            LCD.SetFontColor(MAROON);
            LCD.WriteAt("Runs Played:",60,90);
            LCD.WriteAt(runsPlayed,220,90);

            LCD.SetFontColor(LIGHTCORAL);
            LCD.WriteAt("Click anywhere to return",10,210);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos, false)){
                screen = 1;
                released = false;
            }
            
        }else if(screen == 3){ // credits
            // background
            LCD.SetFontColor(LIGHTGOLDENRODYELLOW);
            LCD.FillRectangle(0,0,319,239);

            // add text
            LCD.SetFontColor(BROWN);
            LCD.WriteAt("An Escaping",100,20);
            LCD.WriteAt("Meatball Production",50,35);

            FEHImage brand;
            brand.Open("Logo2x.png");
            brand.Draw(95,40);

            LCD.WriteAt("Hannah Hofferberth and",25,164);    
            LCD.WriteAt("Pierre van Zyl",60,180);    

            LCD.SetFontColor(BURLYWOOD);
            LCD.WriteAt("Click anywhere to return",10,210);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos, false)){
                screen = 1;
                released = false;
            }
            
        }else if(screen == 4){ // info
            // background
            LCD.SetFontColor(LIGHTGREEN);
            LCD.FillRectangle(0,0,319,239);

            // add text
            LCD.SetFontColor(DARKGREEN);
            LCD.WriteAt("How to play:",85,20);

            LCD.SetFontColor(GREEN);
            LCD.WriteAt("Click anywhere to return",10,210);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos, false)){
                screen = 1;
                released = false;
            }
            
        }else if(screen == 6){ // game over
            // background
            LCD.SetFontColor(DARKRED);
            LCD.FillRectangle(0,0,319,239);

            // add text
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Game Over!",100,20);
            LCD.WriteAt("Score: " ,70,50);
            LCD.WriteAt((int)score/100 ,200,50);
            LCD.WriteAt("High Score: " ,55,85);
            LCD.WriteAt((int)maxScore/100 ,205,85);

            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Click to return to menu",15,210);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos, false)){
                screen = 1;
                released = false;
                resetTime = true;
            }
            
        }
        

        LCD.Update();
        // Never end
    }

    return 0;
}

