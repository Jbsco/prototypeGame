#pragma once

class AnimationFrame {
	public:
	SDL_Texture *texture;
	int time;
	AnimationFrame(SDL_Texture *newTexture=NULL,int newTime=100){
		texture=newTexture;
		time=newTime;
	}
};
class Animation:public Particle {
	vector<AnimationFrame> images;
	int totalTime;
	long currentTime;
	public:
		Animation(SDL_Renderer *newRenderer,string filename,int frames=1,
				int millisPerFrame=100,
				double newPx=0.0,double newPy=0.0,
				double newVx=0.0,double newVy=0.0,
				double newAx=0.0,double newAy=0.0) 
				:Particle(newRenderer,filename+"0.bmp",newPx,newPy,newVx,newVy,newAx,newAy){
			images.push_back(AnimationFrame(image,millisPerFrame));
			totalTime=millisPerFrame;
			for (int i=1;i<frames;i++){
				SDL_Texture *t=mm.read(renderer,filename+to_string(i)+".bmp",SrcR);
				images.push_back(AnimationFrame(t,millisPerFrame));
				totalTime+=millisPerFrame;
			}
			currentTime=rand()%600;
		}
		void loop(int millis){
			Particle::loop(millis);
			DestR.x=(int)px; 
			DestR.y=(int)py;        
			// convert current time to the frame of animation we need
			unsigned current=currentTime/100;
			if(current>=images.size())current=0;
			#ifdef DEBUG  
			cout << currentTime <<  ' ' << current << endl;
			#endif  
			SDL_RenderCopy(renderer,images[current].texture,&SrcR,&DestR);
			currentTime=(currentTime+millis)%totalTime;
		}
};
