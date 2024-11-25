#include "FEHLCD.h"
#include "FEHImages.h"
#include "FEHRandom.h"
#define JUMPSPEED 0.06

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
        Button(char img[], char img2[], float x, float y, float xS, float yS, int screen){
            image.Open(img);
            clickedImage.Open(img2);
            screenToInvoke = screen;
            xSize = xS;
            ySize = yS;
            xPos = x;
            yPos = y;
        }

        int callScreen(){
            return screenToInvoke;
        } 


        // Overloaded method
        bool draw(){
                if(!clicked){
                    image.Draw(xPos, yPos);
                    return false;
                }else{
                    clicked = false;
                    return true;
                }
                
            
        }

        void draw(float xClicked, float yClicked){
            if(xClicked > xPos && xClicked < (xPos+xSize) && yClicked > yPos && yClicked < (yPos+ySize)){
                clickedImage.Draw(xPos, yPos);
                clicked = true;
            }else{
                image.Draw(xPos, yPos);
            }
            
        }
        

};

class Character{
    public:
        float stressIndex = 0;
        float xPos = 0;
        float yPos = 85;
        int jumpIndex = 0;
        bool jumping = false;
        FEHImage character;

        void changeCostume(char costume[]){
            character.Open(costume);
        }

        void drawChar(){
            character.Draw(30, yPos);
        }

        void transitionJump(int maxIndex){
            if(maxIndex > 75){
                maxIndex = 75;
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

class Ground{
    public:
        float position;
        FEHImage image;
        

        Ground(){
            image.Open("Ground/Ground-1.png");
        }

         void openImage(char imageName []){
            image.Open(imageName);
        }

        void drawGround(){
           image.Draw(position,0);
       }
        

};

class Obstacle{
    public:
        float xPos;
        float yPos;
        bool generated = false;
        char imageName[30] = "";
        FEHImage image;
        
        Obstacle(){
            image.Open("obstacles/AlarmClock.png");
            xPos = 400;
            yPos = 0;
        }

        Obstacle(char img[], float y){
            image.Open(img);
            strcpy(imageName, img);
            xPos = 0;
            yPos = y;
        }

        void draw(){
                image.Draw(xPos, yPos);
            
        }
};

class Object{
    public:
        float xPos;
        float yPos;
        bool generated = false;
        char imageName[30] = "";
        FEHImage image;
        
        Object(){
            image.Open("objects/Heart.png");
            xPos = 400;
            yPos = 0;
        }

        Object(char img[]){
            image.Open(img);
            strcpy(imageName, img);
            xPos = 0;
            yPos = 0;
        }

        void draw(){
                image.Draw(xPos, yPos);
            
        }
};

class JumpBar{
    private:
        float totalX = 75;
        float totalY = 20;
        float innerX = 0;
        float xPos = 15;
        float yPos = 205;

    public:
        void resetBar(){
            innerX = 0;
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(xPos,yPos,totalX,totalY); 
        }

        void increaseBar(int max){
            if(innerX < max){
                innerX++;
            }
            
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(xPos,yPos,totalX,totalY); 

            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(xPos,yPos,innerX,totalY); 
        }
};



int main()
{
    // 1: menu, 2: stats, 3: credits, 4: instructions, 5: game
    int screen = 1;

    // Variables for the menu
    Button startButton("Buttons/sprite_0.png", "Buttons/sprite_1.png", 90, 150, 60, 15, 5);
    Button statsButton("Buttons/sprite_2.png", "Buttons/sprite_3.png", 65, 190, 60, 15, 2);
    Button creditsButton("Buttons/sprite_4.png", "Buttons/sprite_5.png", 160, 190, 60, 15, 3);
    Button infoButton("Buttons/sprite_6.png", "Buttons/sprite_7.png", 160, 150, 60, 15, 4);


    // Variables for the game
    Character player;
    player.changeCostume("character/sprite_00.png");

    Ground currGround [3];

    currGround[0].position = 0;
    currGround[1].position = 300;
    currGround[2].position = 600;


    Obstacle currentObstacles[15];
    Object currentObjects[15];
    
    char objectImages[3][30] = {"objects/Bed.png", "objects/Heart.png","objects/Coffee.png"};
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

    while (1) {

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
            }

            
        // Actual game
        }else if(screen == 5){
            // background
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(0,0,319,239);
            
            // sprites
            currGround[0].drawGround();
            currGround[1].drawGround();
            currGround[2].drawGround();

            player.drawChar();

            // check if touching
            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos,false) && moveSpeed == 0){
                timeHeld++;
                bar.increaseBar(75 - player.stressIndex);
                player.changeCostume("Char_crouch/sprite_0_crouch.png");
            }else{

                // Detecting jump status
                if(timeHeld != 0){
                    // if it was just released, set jump info
                    player.changeCostume("character/sprite_01.png");
                    jumpLevel = timeHeld;
                    jumpLevel -= player.stressIndex;
                    timeHeld = 0;
                    moveSpeed = 1.5;
                    player.jumpIndex = 1;
                }else if(player.jumpIndex != 0){
                    // Mid jump
                    player.transitionJump(jumpLevel);
                    player.xPos += moveSpeed;

                    // Reset xPos so it never gets too large
                    if(player.xPos > 2000){
                        player.xPos = 0;
                        lastGeneratedX = 0;
                        lastObGeneratedX = 0;

                    }
                    
                }else{
                    // Jump is done
                    moveSpeed = 0;
                    player.changeCostume("character/sprite_00.png");
                }

                bar.resetBar();
                
            }


            // Ground adjustment
            currGround[0].position -= moveSpeed;
            currGround[1].position -= moveSpeed;
            currGround[2].position -= moveSpeed;
      

            if(currGround[0].position <= -300){
                currGround[0].position = 600;
               
            }
            if(currGround[1].position <= -300){
                currGround[1].position = 600;
            
            }
            if(currGround[2].position <= -300){
                currGround[2].position = 600;
                
            }


            // Change obstacle generation distance
            if(currObstacleGenMax > 80){
                currObstacleGenMax -= 0.02;
            }

            // Generate obstacles
            
            if(player.xPos - lastGeneratedX > currGenerationDistance){
                currentObstacles[currObstacleGenerated].xPos = 350;
                currentObstacles[currObstacleGenerated].yPos = 155;

                int random = 12 * (Random.RandInt() / 32767.0);
                currentObstacles[currObstacleGenerated].image = obstacleImages[random];
                strcpy(currentObstacles[currObstacleGenerated].imageName, obstacleImages[random]);
                currentObstacles[currObstacleGenerated].generated = true;

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

                if(currObstacleGenerated < 14){
                    currObstacleGenerated++;
                }else{
                    currObstacleGenerated = 0;
                }
                
                lastGeneratedX = player.xPos;

                float randomDistance = currObstacleGenMax * (Random.RandInt() / 32767.0) + 40 + (currObstacleGenMax / 40);
                currGenerationDistance = randomDistance;
            }

            // //Generate objects
            
            if(player.xPos - lastObGeneratedX > currObGenerationDistance){
                currentObjects[currObjectGenerated].xPos = 350;
                currentObjects[currObjectGenerated].yPos = 155;

                int random = 3 * (Random.RandInt() / 32767.0);
                 if(random == 1 || random == 2){
                    currentObjects[currObjectGenerated].yPos = 100 * (Random.RandInt() / 32767.0) + 55;
                 }else if(random == 0){
                    currentObjects[currObjectGenerated].yPos = 120;
                 }
                currentObjects[currObjectGenerated].image = objectImages[random];
                strcpy(currentObjects[currObjectGenerated].imageName, objectImages[random]);
                currentObjects[currObjectGenerated].generated = true;

                if(currObjectGenerated < 14){
                    currObjectGenerated++;
                }else{
                    currObjectGenerated = 0;
                }
                
                lastObGeneratedX = player.xPos;

                float randomDistance = 1250 * (Random.RandInt() / 32767.0) + 20;
                currObGenerationDistance = randomDistance;
            }


            for(int i = 0; i < 15; i++){
                if(currentObstacles[i].generated){
                    currentObstacles[i].xPos -= moveSpeed;
                    currentObstacles[i].draw();

                    if(currentObstacles[i].xPos < -250){
                        currentObstacles[i].generated = false;
                    }
                }
                

            }

            for(int i = 0; i < 15; i++){
                if(currentObjects[i].generated){
                    currentObjects[i].xPos -= moveSpeed;
                    currentObjects[i].draw();

                    if(currentObjects[i].xPos < -250){
                        currentObjects[i].generated = false;
                    }
                }
                
            }

            // Check collisions
            // Check obstacles
            for(int i = 0; i < 15; i++){
                if (currentObstacles[i].generated) {
                    if (strcmp(currentObstacles[i].imageName, obstacleImages[0]) == 0) { // AlarmClock
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[1]) == 0) { // Bill
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[2]) == 0) { // Cell_Phone
                        if (currentObstacles[i].xPos < 90 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[3]) == 0) { // Clock
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[4]) == 0) { // books
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 40) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[5]) == 0) { // Thunder can have y
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[6]) == 0) { // paper1
                        if (currentObstacles[i].xPos < 95 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[7]) == 0) { // Application
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[8]) == 0) { // messages can have y
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[9]) == 0) { // News can have y
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[10]) == 0) { // Email can have y
                        if (currentObstacles[i].xPos < 100 && currentObstacles[i].xPos > 50 && player.yPos > currentObstacles[i].yPos-100 && player.yPos < currentObstacles[i].yPos+0) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }

                    } else if (strcmp(currentObstacles[i].imageName, obstacleImages[11]) == 0) { // Phone2
                        if (currentObstacles[i].xPos < 90 && currentObstacles[i].xPos > 70 && player.yPos > 60) {
                            currentObstacles[i].generated = false;
                            currentObstacles[i].xPos = -251;
                        }
                    }
                }
            }

            // check objects
            for (int i = 0; i < 3; i++) {
                if (currentObjects[i].generated) {
                    if (strcmp(currentObjects[i].imageName, objectImages[0]) == 0) { // Bed
                        if (currentObjects[i].xPos < 130 && currentObjects[i].xPos > 60 && player.yPos > 60) {
                            currentObjects[i].generated = false;
                            currentObjects[i].xPos = -251;
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[1]) == 0) { // Heart can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            currentObjects[i].generated = false;
                            currentObjects[i].xPos = -251;
                        }

                    } else if (strcmp(currentObjects[i].imageName, objectImages[2]) == 0) { // Coffee can have y
                        if (currentObjects[i].xPos < 100 && currentObjects[i].xPos > 50 && player.yPos > currentObjects[i].yPos-100 && player.yPos < currentObjects[i].yPos+0) {
                            currentObjects[i].generated = false;
                            currentObjects[i].xPos = -251;
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
            LCD.WriteAt("High Score:",50,100);
            LCD.WriteAt("9000",190,100);

            LCD.SetFontColor(LIGHTCORAL);
            LCD.WriteAt("Click anywhere to return",10,210);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos, false)){
                screen = 1;
            }
            
        }else if(screen == 3){ // credits
            // background
            LCD.SetFontColor(LIGHTGOLDENRODYELLOW);
            LCD.FillRectangle(0,0,319,239);

            // add text
            LCD.SetFontColor(BROWN);
            LCD.WriteAt("An Escaping",100,20);
            LCD.WriteAt("Meatball Production",50,40);

            LCD.SetFontColor(BURLYWOOD);
            LCD.WriteAt("Click anywhere to return",10,210);

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos, false)){
                screen = 1;
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
            }
            
        }
        

        LCD.Update();
        // Never end
    }
    return 0;
}

