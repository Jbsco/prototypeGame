#pragma once

class ProtoGame{
	SDL_Window *win;
	SDL_bool loopShouldStop;
	SDL_Thread *renderLoop;
	protected:
		int w,h;  //  Dimension of screen space
		SDL_Renderer *renderer;
		SDL_Joystick* stickIn; // first joystick available
		double throttleVal,pitchVal; // position values for throttle and pitch axes
		SDL_Surface *surfaceMessage;
		TTF_Font *conTTF;
		SDL_Color White;
		//SDL_Rect Message_rect1,Message_rect2;
		SDL_Texture *Message;
	public:
		ProtoGame(string name,int newWidth=640,int newHeight=480,int seed=0){
			win=NULL;
			renderer=NULL;
			White=(SDL_Color){255,255,255}; // rgb
			stickIn=NULL;
			throttleVal=-32768;
			pitchVal=0;
			w=newWidth;  //  Dimensions are part of the protogame class
			h=newHeight;
			loopShouldStop=SDL_FALSE;
			SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);
			TTF_Init();
			conTTF=TTF_OpenFont("img/CONSOLA.TTF",24);
			if(SDL_NumJoysticks()){
				stickIn=SDL_JoystickOpen(0);
			}
			// Added some parameters to constructor to set and store dimensions
			win=SDL_CreateWindow(name.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,0);
			renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
			if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1)
				throw Exception("SDL2_mixer failure");
			if(seed==0)srand(time(NULL));
			else srand(seed);
			//SDL_ShowCursor(SDL_DISABLE);
			//auto start=chrono::high_resolution_clock::now();
		}
		int getW(){return w;} // Read-Only accessor methods
		int getH(){return h;} // ''
		void doLoop(){
			renderLoop=SDL_CreateThread(ProtoGame::renderThread,"RenderThread",(void*)this);
			// int millis=SDL_GetTicks(); 
			while(!loopShouldStop){
				// unsigned millisDt=SDL_GetTicks()-millis;  // Calculate millis
				SDL_Event event;
				while(SDL_PollEvent(&event)){
					switch(event.type){
						case SDL_QUIT:
							loopShouldStop=SDL_TRUE;
							break;
					}
					doEvent(event);
				}
				/* if(millisDt>1){
					loop(millisDt);
					millis=SDL_GetTicks();
				} */
				// SDL_Delay(10); // reduces CPU usage considerably, some performance impact
			}
		}
		// int renderThread(/*ProtoGame *this, */void *ptr){
		static int renderThread(void *protoPointer){ // will hand over ptr to ProtoGame
			ProtoGame *game=(ProtoGame*)protoPointer; // typecast to ProtoGame*
			int millis=SDL_GetTicks(); // getMicros would be ideal, possibly using chrono class
			while(!game->loopShouldStop){
				unsigned millisDt=SDL_GetTicks()-millis;  // Calculate millis
				if(millisDt>1){
					game->loop(millisDt);
					// SDL_Delay(1); // crushes performance, not recommended as of yet
					millis=SDL_GetTicks();
				}
			}
			return 0;
		}
		virtual void doEvent(const SDL_Event &event)=0;
		virtual void loop(int millis)=0;
		void renderText(char *text,SDL_Rect Message_rect){
			surfaceMessage=TTF_RenderText_Solid(conTTF,text,White);
			Message=SDL_CreateTextureFromSurface(renderer,surfaceMessage);
			SDL_RenderCopy(renderer,Message,NULL,&Message_rect);
		}
		~ProtoGame(){
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(win);
			SDL_JoystickClose(stickIn);
			stickIn=NULL;
			SDL_DestroyTexture(Message);
			TTF_CloseFont(conTTF);
			TTF_Quit();
			SDL_Quit();
		}
};
