#pragma once

const SDL_Rect EmptyRect={0,0,0,0};

class TextureInfo{
	public:
		SDL_Texture* texture;
		SDL_Rect srcRect;
		TextureInfo(SDL_Texture *newTexture=NULL,const SDL_Rect &newSrcRect=EmptyRect){
			texture=newTexture;
			srcRect=newSrcRect;
		}
};

