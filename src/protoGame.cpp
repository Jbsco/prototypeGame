#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <time.h>
#include <SDL.h> 	
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Exception.h"
#include "TextureInfo.h"
#include "MediaManager.h"
#include "ProtoGame.h"
#include "Sprite.h"
#include "Particle.h"
#include "Course.h"
#include "Player.h"
#include "Animation.h"

using namespace std;

class Game:public ProtoGame{
	vector<Sprite *>sprites;
	Sprite *background;
	Player *p;
	Course *c;
	const uint8_t *input;
	SDL_Rect positioning; /* using this struct as {overall-pos x,
												   overall-pos y,
												   screen-pos x,
												   screen-pos y}*/
	public:
		Game():ProtoGame("Supercool Game",640,480,10){ // Size,Seed
			background=new Sprite(renderer,"img/background.bmp");
			sprites.push_back(background);
			c=new Course(renderer);
			c->generate();
			//double sx=getW()/2.0;
			//double sy=getH()/2.0;
			input=SDL_GetKeyboardState(NULL);
			sprites.push_back(new Animation(renderer,"img/P15_",5,100,-100,-100));
			p=new Player(renderer,"img/player.bmp",320,465);
			p->setBounds(0,0,w,h);
			sprites.push_back(p);
		}
		void doEvent(const SDL_Event &event){
			p->handleEvent(event);
		}
		void loop(int millis){
			if(stickIn==NULL){
				// check input bits:
				if(input[SDL_SCANCODE_W]){
					p->throttle+=7.5;
					if(p->throttle>500) p->throttle=500;
				}else p->throttle=0;
				if(p->throttle<0)p->throttle=0;
				if(input[SDL_SCANCODE_A])p->pr-=1;
				if(input[SDL_SCANCODE_S])p->throttle=0;
				if(input[SDL_SCANCODE_D])p->pr+=1;
			}
			// check joystick input:
			if(stickIn!=NULL){ // *note* TBS Tango-II axes: 0=LS-Y, 1=RS-X, 2=RS-Y, 3=LS-X
				throttleVal=SDL_JoystickGetAxis(stickIn,0); // y-axis on left stick
				p->throttle=(throttleVal+32768)*0.007629; // scale -32768:32768 to 0:500
				pitchVal=SDL_JoystickGetAxis(stickIn,2); // y-axis on right stick
				p->pr+=pitchVal*0.000063; // scale -32768:32768 to -1.75:1.75
			}
			SDL_RenderClear(renderer);
			p->loop(millis);
			p->getPositioning(positioning); // positioning rect updated here
			for(auto q:sprites)q->loop(millis);
			if(p->playMode)c->loop(millis,positioning); // sending positioning rect to locate course objects
			if(p->coords){
				renderText(p->outCoords,(SDL_Rect){1,1,80,12});
				renderText(p->outVelR,(SDL_Rect){1,16,80,12});
			}
			if(p->hudMode){ // HUD input bars
				int yThr=468-p->throttle*0.148;
				int yPit=431-pitchVal*0.0011291504;
                SDL_SetRenderDrawColor(renderer,255,0,0,SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer,13,yThr,23,yThr); // throttle bar
                SDL_SetRenderDrawColor(renderer,0,255,0,SDL_ALPHA_OPAQUE);
				SDL_RenderDrawLine(renderer,36,yPit,46,yPit); // pitch bar
				renderText(p->outVel,(SDL_Rect){560,420,60,24}); // velocity text overlay
			}
			SDL_RenderPresent(renderer);
		}
		~Game(){
			for(auto p:sprites)delete p;
		}
};

int main(int argc,char *argv[]){
	try{
		Game g;
		g.doLoop();
	}catch(Exception e){
		cerr << e;
	}
    return 0;
}
