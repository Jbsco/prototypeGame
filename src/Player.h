#pragma once

double radFact=M_PI/180;
double degFact=180/M_PI;

class Player:public Particle{
	double vx,vy,ax,ay,wgt,pxT,pyT;
	SDL_Point center;
	SDL_RendererFlip flip;
	int minX,maxX,minY,maxY;
	Mix_Chunk *waves;
	Sprite *overlay,*HUDbg,*HUD1,*HUD2,*HUDin;
	bool mode;
	public:
		bool coords,hudMode,playMode;
		double throttle,pr; // throttle, position of rotation
		char outCoords[11];
		char outVelR[11];
		char outVel[3];
		Player(SDL_Renderer *newRenderer,string filename,
		double newPx=0.0,double newPy=0.0):Particle(newRenderer,filename,newPx,newPy){
			overlay=new Sprite(renderer,"img/gridOlay.bmp",-320,-240); // use loopOlay(millis,vx,vy);
			HUD1=new Sprite(renderer,"img/HUD1.bmp",540,380); // use loopHUD(millis,*vel r-angle*)
			HUD2=new Sprite(renderer,"img/HUD2.bmp",540,380); // use loopHUD(millis,*set r-angle*)
			HUDbg=new Sprite(renderer,"img/HUDbg.bmp",540,380); // use loopHUD(millis,*set r-angle*)
			HUDin=new Sprite(renderer,"img/HUDinbg.bmp",0,380); // use loopHUD(millis,*set r-angle*)
			minX=-1;
			maxX=-1;
			minY=-1;
			maxY=-1;
			ay=98.0;
			throttle=0;
			wgt=10;
			pr=0;
			mode=1;
			hudMode=0;
			coords=0;
			playMode=0;
			pxT=0; // total coord positioning
			pyT=0; // total coord positioning
			// vr=0;
			// ar=0;
			center=(SDL_Point){18,10};
			flip=SDL_FLIP_NONE;
			waves=mm.readWAV("img/crash.wav");
		}
		void setBounds(int newMinX,int newMinY,int newMaxX,int newMaxY){
			minX=newMinX;
			maxX=newMaxX;
			minY=newMinY;
			maxY=newMaxY;
		}
		void handleEvent(const SDL_Event &e){
			/*// mouse motion:
			else if(e.type==SDL_MOUSEMOTION){
				ax=e.motion.x/2-px;
				ay=e.motion.y/2-py;
				//px=(maxX+minX)/2+e.motion.xrel;
				//py=(maxY+minY)/2+e.motion.yrel;
			}*/
			if(e.type==SDL_KEYDOWN)if(e.key.keysym.sym==SDLK_c)coords=!coords;
			if(e.type==SDL_KEYDOWN)if(e.key.keysym.sym==SDLK_h)hudMode=!hudMode;
			if(e.type==SDL_KEYDOWN)if(e.key.keysym.sym==SDLK_p/*&&mode*/)playMode=!playMode;
			if(e.type==SDL_KEYDOWN)if(e.key.keysym.sym==SDLK_m){
				mode=!mode;
				//playMode=0;
				vx=0;
				vy=0;
				if(mode){
					px=320;
					pxT=320;
					py=465;
					pyT=465;
				}
			}
			if(e.type==SDL_KEYDOWN)if(e.key.keysym.sym==SDLK_r){
				vx=0;
				vy=0;
				if(mode){
					px=320;
					py=465;
				}
			}
		}
		void loop(int millis){
			double dt=((float)millis)/1000.0;
			// vr+=ar*dt; // degrees per time-constant
			// pr+=vr*dt; // in degrees
			if(pr>180)pr-=359;
			if(pr<-180)pr+=359;
			vx+=(ax)*dt+throttle*cos((270+pr)*radFact)*dt; // x-component of throttle-velocity, uses radians
			vy+=(ay)*dt+throttle*sin((270+pr)*radFact)*dt; // y-component of throttle-velocity, uses radians
			// speed limit: (max magnitude: 1000)
			double mag=sqrt(pow(vx,2)+pow(vy,2));
			if(mag>1000){
				vx=1000*(vx/mag);
				vy=1000*(vy/mag);
			}
			// apply velocity to position per mode:
			pxT-=vx*dt; // total coords
			pyT-=vy*dt; // total coords
			if(mode)px+=vx*dt;
			else px-=vx*dt;
			if(mode)py+=vy*dt;
			else py-=vy*dt;
			// collect telemetry text:
			if(coords){
				snprintf(outCoords,11,"%04dx %04dy",(int)pxT,(int)pyT);
				snprintf(outVelR,11,"%03dv %+04dr",(int)(sqrt(pow(vx,2)+pow(vy,2))),(int)pr);
			}
			if(hudMode)snprintf(outVel,4,"%03d",(int)(sqrt(pow(vx,2)+pow(vy,2))/10));
			if(mode){
				if(px>625){
					px=625;
					vx=(-0.3*vx);
					vy=(0.7*vy);
					if(vx<-35){
						Mix_HaltChannel(2);
						Mix_PlayChannel(2,waves,0);
					}
				}
				if(px<-10){
					px=-10;
					vx=(-0.3*vx);
					vy=(0.7*vy);
					if(vx>35){
						Mix_HaltChannel(1);
						Mix_PlayChannel(1,waves,0);
					}
				}
				if(py>465){
					py=465;
					vx=(0.7*vx);
					vy=(-0.3*vy);
					if(vy<-35){
						Mix_HaltChannel(2);
						Mix_PlayChannel(2,waves,0);
					}
				}
				if(py<0){
					py=0;
					vx=(0.7*vx);
					vy=(-0.3*vy);
					if(vy>35){
						Mix_HaltChannel(1);
						Mix_PlayChannel(1,waves,0);
					}
				}
			}
			else{
				if(pxT>=8000){
					vx=(-0.3*vx);
					//vy=(0.7*vy);
					if(vx<-35){
						Mix_HaltChannel(2);
						Mix_PlayChannel(2,waves,0);
					}
				}
				if(pxT<=-8000){
					vx=(-0.3*vx);
					//vy=(0.7*vy);
					if(vx>35){
						Mix_HaltChannel(1);
						Mix_PlayChannel(1,waves,0);
					}
				}
				if(pyT>=6000){
					//vx=(0.7*vx);
					vy=(-0.3*vy);
					if(vy<-35){
						Mix_HaltChannel(2);
						Mix_PlayChannel(2,waves,0);
					}
				}
				if(pyT<=-6000){
					//vx=(0.7*vx);
					vy=(-0.3*vy);
					if(vy>35){
						Mix_HaltChannel(1);
						Mix_PlayChannel(1,waves,0);
					}
				}
			}
			// DestR must be relative to velocity in 'x' and 'y' to implement receded player position relative to scrolling velocity
			// need proportionalilty relative to screen center, minus velocity direction proportion
			// must be static (non-proportional) if velocities are below threshold (independent in 'x' 'y')
			// if over threshold, must approach center of screen (320,240) from current position (px,py)
				// proportionality must be independent functions of 'vx' and 'vy'
				// need to know maximum velocity achievable (with or without gravity..?)
					// likely just need to define a limit on position recession
				// i.e. once beyond velocity threshold, the (px,py) must be reduced while the (320-vx,240-vy) increases (inverse relation)
			if(mode){ // default mode, no proportional pos
				DestR.x=(int)px;
				DestR.y=(int)py;
			}
			else{ // move grid overlay & proportional pos (player recedes from velocity vector)
				double xFactor=1-fabs(pxT)/7695;
				double yFactor=1-fabs(pyT)/5770;
				if(xFactor<0)xFactor=0;
				if(yFactor<0)yFactor=0;
				if(pxT>-7695&&pxT<7695) DestR.x=(int)(305-xFactor*0.30*vx); // proportional when nearer to the boundary limits
				else{ // unless pT limits exceeded, then position is able to move from proportional location to default mode screen position
					if(pxT<=-7695) DestR.x=(int)-1*(pxT+7375); // right side of bounds
					else if(pxT>=7695) DestR.x=(int)-1*(pxT-7990); // left side of bounds
				}
				if(pyT>-5770&&pyT<5770) DestR.y=(int)(230-yFactor*0.22*vy);
				else{ // unless pT limits exceeded, then position is able to move from proportional location to default mode screen position
					if(pyT<=-5770) DestR.y=(int)-1*(pyT+5535); // lower side of bounds
					else if(pyT>=5770) DestR.y=(int)-1*(pyT-6000); // upper side of bounds
				}
				// check *T coordinates for total positioning
				// unless pxT limits exceeded, then position is halted until pxT back in bounds
				if(pxT>=7695||pxT<=-7695); // null case, do not update positions
				else if(px>-400) px=-1200;
				else if(px<-1200) px=-400;
				// unless pyT limits exceeded, then position is halted until pyT back in bounds
				if(pyT>=5770||pyT<=-5770); // null case, do not update positions
				else if(py>-200) py=-800;
				else if(py<-800) py=-200;
				if(pxT<=-7695) overlay->loopOlay(millis,-880,py-(yFactor*.22*vy)); // right bounds, x position of overlay fixed
				else if(pxT>=7695) overlay->loopOlay(millis,-713,py-(yFactor*.22*vy)); // left bounds, x position of overlay fixed
				else if(pyT<=-5770) overlay->loopOlay(millis,px-(xFactor*.3*vx),-560); // lower bounds, y position fixed
				else if(pyT>=5770) overlay->loopOlay(millis,px-(xFactor*.3*vx),-43); // upper bounds, y position fixed
				else overlay->loopOlay(millis,px-(xFactor*.3*vx),py-(yFactor*.22*vy)); // move overlay by same factor as player proportional
			}
			if(hudMode){
			// rotate AOT and pitch HUD elements
				HUDbg->loop(millis);
				HUDin->loop(millis);
				HUD2->loopHUD(millis,pr);
				if(vx>0&&vy<0)HUD1->loopHUD(millis,(-atan(vx/vy)*degFact)); // upper right quadrant
				if(vx>0&&vy>0)HUD1->loopHUD(millis,(180-atan(vx/vy)*degFact)); // lower right quadrant
				if(vx<0&&vy<0)HUD1->loopHUD(millis,(-atan(vx/vy)*degFact)); // upper left quadrant
				if(vx<0&&vy>0)HUD1->loopHUD(millis,(-180-atan(vx/vy)*degFact)); // lower left qudrant
			}
			SDL_RenderCopyEx(renderer,image,&SrcR,&DestR,pr,&center,flip);
		}
		void getPositioning(SDL_Rect &positioning){
			if(!mode)positioning=(SDL_Rect){(int)(pxT-(.3*vx)),(int)(pyT-(.22*vy)),(int)(305-0.30*vx),(int)(230-0.22*vy)};
			else positioning=(SDL_Rect){(int)px,(int)py,(int)px,(int)py};
		}
		~Player(){
			delete overlay;
			delete HUD1;
			delete HUD2;
		}
};
