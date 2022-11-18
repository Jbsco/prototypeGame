#pragma once

class Particle:public Sprite{
	protected:
		double vx,vy,ax,ay,wgt;
	public:
		Particle(SDL_Renderer *newRenderer,string filename,
		double newPx=0.0,double newPy=0.0,
		double newVx=0.0,double newVy=0.0,
		double newAx=0.0,double newAy=0.0):Sprite(newRenderer,filename,newPx,newPy){
			vx=newVx;
			vy=newVy;
			ax=newAx;
			ay=newAy;
			wgt=10;
		}
		void loop(int millis){
			double dt=((float)millis)/1000.0;
			px+=vx*dt;
			py+=vy*dt;
			vx+=ax*dt;
			vy+=wgt*ay*dt;
			if(px>=640||px<0)vx=-(0.9*vx);
			if(py>=480-15||py<0)vy=-(0.9*vy);
			Sprite::loop(millis);
		}
		~Particle(){
		}
};
