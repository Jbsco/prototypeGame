# UNAME := $(shell uname)

ifeq ($(OS),Windows_NT) 
#	SDLCFLAGS=<your cflags Windows flags go here>
#	SDLLIBS=<your libs Windows flags go here>
#	Your flags should look something like below
	WINLOC=$(GAMELOC)
	SDLCFLAGS=`sdl2-config --cflags`
	SDLLIBS=`sdl2-config --libs` -lSDL2_mixer -lSDL2_ttf
#	SDLCFLAGS=-I$(WINLOC)/msys64/mingw64/include/SDL2/ -Dmain=SDL_main
#	SDLLIBS=-L$(WINLOC)/msys64/mingw64/lib/ -lmingw32 -mwindows -lSDL2main -lSDL2
endif

ifeq ($(OS),MacOS)
#		SDLCFLAGS=-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
#		SDLLIBS=-L/opt/homebrew/lib -lSDL2 -lSDL2_mixer -lSDL2_ttf
		SDLCFLAGS=`sdl2-config --cflags`
		SDLLIBS=`sdl2-config --libs` -lSDL2_mixer -lSDL2_ttf -lSDL2_image
endif

ifeq ($(OS),Linux)
	SDLCFLAGS=-I/usr/include/SDL2 -D_REENTRANT
	SDLLIBS=-lSDL2
endif

SRCDIR=src
SRCS=$(SRCDIR)/protoGame.cpp $(SRCDIR)/Exception.cpp
HEADERS=$(SRCDIR)/Animation.h $(SRCDIR)/MediaManager.h $(SRCDIR)/Player.h \
$(SRCDIR)/Sprite.h $(SRCDIR)/Exception.h  $(SRCDIR)/Particle.h \
$(SRCDIR)/ProtoGame.h  $(SRCDIR)/TextureInfo.h $(SRCDIR)/Course.h
EXE=$(BUILDDIR)/protoGame.exe
SCRIPTS=protoGame.sh protoGame.bat

MEDIADIR=img
MEDIA=$(MEDIADIR)/background.bmp  $(MEDIADIR)/P15_0.bmp  $(MEDIADIR)/P15_1.bmp \
$(MEDIADIR)/P15_2.bmp  $(MEDIADIR)/P15_3.bmp  $(MEDIADIR)/P15_4.bmp \
$(MEDIADIR)/P15_5.bmp  $(MEDIADIR)/player.bmp

BUILDDIR=build
BUILDFILES=Makefile

$(EXE): $(SRCS) $(HEADERS)
	g++ -g -Wall $(SDLCFLAGS) -o $@ $(SRCS) $(SDLLIBS)

protoGame.zip: $(SRCS) $(HEADERS) $(MEDIA) $(BUILDFILES) $(SCRIPTS)
	zip protoGame.zip $+

Deploy.zip: $(MEDIA) $(EXE) $(SCRIPTS)
	zip Deploy.zip $+

run: ${EXE}
	./${EXE}

clean: 
	rm $(EXE)

cleanAll: 
	rm $(BUILDDIR)/*

all: $(EXE) protoGame.zip Deploy.zip 
