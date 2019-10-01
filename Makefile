DATADIR?=./data/

CC=g++ -g -DDATADIR="\"$(DATADIR)\""

CFLAGS+=-Wall `sdl-config --cflags`

LIBS+=-lGL -lGLEW `sdl-config --libs` -lSDL_image -lSDL_ttf
#SOURCES=main.cpp menu.cpp button.cpp control.cpp text.cpp buttonset.cpp background.cpp sprite.cpp label.cpp checkbox.cpp screenshot.cpp settings.cpp dirs.cpp playerprofile.cpp isfile.cpp ents.cpp editor.cpp nag.cpp console.cpp glfx.cpp general.cpp entspawner.cpp game.cpp level.cpp missioninfo.cpp

SOURCES=background.cpp button.cpp buttonset.cpp camcontrol.cpp checkbox.cpp collisiontest.cpp console.cpp control.cpp dirs.cpp editor.cpp effectqueue.cpp ents.cpp entspawner.cpp game.cpp general.cpp glfx.cpp hud.cpp isfile.cpp label.cpp level.cpp main.cpp menu.cpp missioninfo.cpp nag.cpp playerprofile.cpp screenshot.cpp settings.cpp sprite.cpp terrain.cpp text.cpp ents/ammo.cpp ents/astroid.cpp ents/background.cpp ents/bullets.cpp ents/camlimit.cpp ents/cargobox.cpp ents/effector.cpp ents/playership.cpp ents/weapon.cpp effects/fxspeedup.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=un

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

dirs.o:
	$(CC) -c -o dirs.o dirs.cpp

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE) ents/*.o effects/*.o
