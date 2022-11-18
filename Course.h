#pragma once

class Course{
    protected:
		SDL_Renderer *renderer;
		vector<Sprite *> courseObjects;
		vector<bool> clearCheck;
		Sprite *object;
        SDL_Rect timerRect,scoreRect;
		TTF_Font *conTTF;
		SDL_Surface *surfaceMessage;
		SDL_Texture *Message;
		SDL_Color timerTxt,scoreTxt;
		double timer,winTime;
		int score;
		char timeStr[6],scoreStr[6];
	public:
		Course(SDL_Renderer *newRenderer){
			renderer=newRenderer;
			timerTxt=(SDL_Color){255,255,255};
			scoreTxt=(SDL_Color){255,255,255};
			timerRect=(SDL_Rect){540,20,80,24};
			scoreRect=(SDL_Rect){540,40,80,24};
			conTTF=TTF_OpenFont("img/CONSOLA.TTF",24);
			score=0;
        }
		void generate(){ // should pull from known object types and generate randomly located sprites
			srand(time(NULL));
			for(int i=0;i<5+rand()%10;i++){
				pair<Sprite*,bool> pair;
				courseObjects.push_back(new Sprite(renderer,"img/courseObj.bmp",(rand()%591),(rand()%431)));
				clearCheck.push_back(0);
			}
			timer=10; // seconds
			winTime=0;
		}
		void loop(int millis,SDL_Rect &positioning){ // may need input for relative position, and return for checking SDL_Rect of current course object
			timer-=millis/1000.0; // subtract time
			if(score<(int)courseObjects.size())winTime+=millis/1000.0; // keep winning time
			//for(auto o:courseObjects){
			for(int i=0;i<(int)courseObjects.size();i++){ // can't use auto iterator, need to update clearCheck vector
				if(courseObjects[i]->check()){
					courseObjects[i]->loopCourse(millis,positioning); // only render if within/near viewport
					// check if player is within object checkRect, clear object, reset timer to 10sec, increase score
					if(courseObjects[i]->qualify(positioning)&&!clearCheck[i]&&timer>0){
						timer=10.0;
						score++;
						courseObjects[i]->changeImage("img/courseObjClr.bmp");
						clearCheck[i]=1;
					}
				}
			}
			// display timer/score: make c_str, render
			if(score>=(int)courseObjects.size()){ // win condition
				timerTxt=(SDL_Color){0,255,0}; // timer green
				scoreTxt=(SDL_Color){0,255,0}; // score green
				snprintf(timeStr,6,"% 6f",winTime); // set string to winning time
				if(timer>0)timer=0; // always reset after 5sec
			}else{ // ongoing play
				if(timer>0)snprintf(timeStr,6,"% 6f",timer); // countdown timer
				else{ // countdown timer exceeded
					snprintf(timeStr,6," 0.00"); // don't display negative
					timerTxt=(SDL_Color){255,0,0}; // timer red
				}
			}
			snprintf(scoreStr,6,"%d/%d",score,(int)courseObjects.size()); // score text
			renderText();
			if(timer<-5){ // reset
				timerTxt=(SDL_Color){255,255,255};
				scoreTxt=(SDL_Color){255,255,255};
				courseObjects.clear();
				clearCheck.clear();
				generate();
				timer=10;
				winTime=0;
				score=0;
			}
		}
		void renderText(){
			surfaceMessage=TTF_RenderText_Solid(conTTF,timeStr,timerTxt);
			Message=SDL_CreateTextureFromSurface(renderer,surfaceMessage);
			SDL_RenderCopy(renderer,Message,NULL,&timerRect);
			surfaceMessage=TTF_RenderText_Solid(conTTF,scoreStr,scoreTxt);
			Message=SDL_CreateTextureFromSurface(renderer,surfaceMessage);
			SDL_RenderCopy(renderer,Message,NULL,&scoreRect);
		}
		~Course(){
			for(auto o:courseObjects)delete o;
		}
};
