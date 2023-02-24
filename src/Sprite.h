#pragma once

class Sprite{ // Just draw no physics or keyboard
	protected:
		SDL_Renderer *renderer;
		SDL_Texture *image;
		SDL_Rect SrcR,DestR;
		double px,py;
	public:
		Sprite(SDL_Renderer *newRenderer,string filename,double newPx=0.0,double newPy=0.0){
			renderer=newRenderer;
			image=mm.read(renderer,filename,SrcR);
			DestR=SrcR;
			px=newPx;
			py=newPy;
		}
		virtual void handleEvent(SDL_Event &e){} 
		virtual void loop(int millis){
			DestR.x=(int)px;
			DestR.y=(int)py;
			SDL_RenderCopy(renderer,image,&SrcR,&DestR);
		}
		virtual void loopOlay(int millis,double newPx,double newPy){ // loop overlay graphic with position (tied to velocity)
			DestR.x=(int)newPx;
			DestR.y=(int)newPy;
			SDL_RenderCopy(renderer,image,&SrcR,&DestR);
		}
		virtual void loopHUD(int millis,int newR){ // rotation only
			DestR.x=(int)px;
			DestR.y=(int)py;
			SDL_Point center={50,50};
			SDL_RenderCopyEx(renderer,image,&SrcR,&DestR,newR,&center,SDL_FLIP_NONE);
		}
		virtual void loopCourse(int millis,SDL_Rect &positioning){
			//px+=positioning.x;
			//py+=positioning.y;
			DestR.x=px/*+positioning.x+positioning.w*/;
			DestR.y=py/*+positioning.y+positioning.h*/;
			SDL_RenderCopy(renderer,image,&SrcR,&DestR);
		}
		virtual bool check(){
			return (DestR.x<700)&&(DestR.x>-60)&&(DestR.y<560)&&(DestR.y>-60); // check if within bounds of viewport
		}
		virtual bool qualify(SDL_Rect &positioning){
			return (positioning.x>=DestR.x)&&(positioning.x<=DestR.x+50)&&(positioning.y>=DestR.y)&&(positioning.y<=DestR.y+50);
		}
		virtual void changeImage(string newFilename){
			image=mm.read(renderer,newFilename,SrcR);
		}
		virtual ~Sprite(){
			SDL_DestroyTexture(image);
		}
};
