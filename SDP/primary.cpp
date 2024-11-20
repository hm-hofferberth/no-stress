#include "FEHLCD.h"
#include "FEHImages.h"
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
        void openImage(char imageName []){
            image.Open(imageName);
        }
        void drawGround(){
            image.Draw(position,0);
        }
        

};

class Obstacle{

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

        void increaseBar(){
            if(innerX < 75){
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

    Ground ground1;
    Ground ground2;
    ground1.openImage("Ground-2.png.png");
    ground2.openImage("Ground-2.png.png");
    ground1.position = 0;
    ground2.position = 320;

    JumpBar bar;

    float timeHeld = 0;
    float moveSpeed = 0;
    int jumpLevel = 50;

    float last_generated_x = 15;
    
    while (1) {

        if(screen == 1){
          // background
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(0,0,319,239); 

            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos) && moveSpeed == 0){
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

            

        }else if(screen == 5){
            // background
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(0,0,319,239);
            
            // sprites
            ground1.drawGround();
            ground2.drawGround();
            player.drawChar();

            // check if touching
            float x_pos;
            float y_pos;

            if(LCD.Touch(&x_pos, &y_pos) && moveSpeed == 0){
                timeHeld++;
                bar.increaseBar();
            }else{

                // Detecting jump status
                if(timeHeld != 0){
                    // if it was just released, set jump info
                    player.changeCostume("character/sprite_01.png");
                    jumpLevel = timeHeld;
                    jumpLevel -= player.stressIndex;
                    timeHeld = 0;
                    moveSpeed = 2;
                    player.jumpIndex = 1;
                }else if(player.jumpIndex != 0){
                    // Mid jump
                    player.transitionJump(jumpLevel);
                    x_pos += moveSpeed;
                    
                }else{
                    // Jump is done
                    moveSpeed = 0;
                    player.changeCostume("character/sprite_00.png");
                }

                bar.resetBar();
                
            }


            // Ground adjustments
            ground1.position -= moveSpeed;
            ground2.position -= moveSpeed;

            if(ground1.position <= -321){
                ground1.position = ground2.position+320;
                
            }
            if(ground2.position <= -321){
                ground2.position = ground1.position+320;
                
            }


        }else if(screen == 2){
            // background
            LCD.SetFontColor(LIGHTPINK);
            LCD.FillRectangle(0,0,319,239);
            
        }else if(screen == 3){
            // background
            LCD.SetFontColor(LIGHTGOLDENRODYELLOW);
            LCD.FillRectangle(0,0,319,239);
            
        }else if(screen == 4){
            // background
            LCD.SetFontColor(LIGHTGREEN);
            LCD.FillRectangle(0,0,319,239);
            
        }
        

        LCD.Update();
        // Never end
    }
    return 0;
}

