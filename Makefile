##
## Makefile for  in /home/king_j/Git/cpp_indie_studio/bomberman
##
## Made by Jimmy King
## Login   <king_j@epitech.net>
##
## Started on  Tue Apr 26 17:47:52 2016 Jimmy King
## Last update Tue Jun  7 10:05:48 2016 Nicolas Gascon
##

export PATH	:=	$(PWD):$(PATH)

NAME		=		bomberman

SRC		=		main.cpp \
				src/Application/Application.cpp \
				src/GameObject/Components/Mesh/MeshComponent.cpp \
				src/GameObject/GameObject.cpp \
				src/Scene/Scenes/Menu.cpp \
				src/Scene/Camera.cpp \
				src/Scene/Scene.cpp \
				src/Scene/SceneManager.cpp \
				src/Utils/Time/Time.cpp \
				include/commons/TinyXML/tinystr.cpp \
				include/commons/TinyXML/tinyxmlparser.cpp \
				include/commons/TinyXML/tinyxmlerror.cpp \
				include/commons/TinyXML/tinyxml.cpp \
				src/GameObject/Components/Player/Player.cpp \
				src/GameObject/Components/Action/AAction.cpp \
				src/Scene/DotSceneLoader.cpp \
				src/Physics/Core/PhysicObject.cpp \
				src/Physics/Core/Physics.cpp \
				src/Physics/MOC/NewMOC.cpp \
				src/Physics/Instances/StaticInstances.cpp \
				src/Input/Input.cpp \
				src/Audio/Audio.cpp \
				src/Audio/Playlist.cpp \
				src/Audio/Manager.cpp \
				src/GameObject/Components/Bomb/Bomb.cpp \
				src/GameObject/Components/Bomb/BombManager.cpp \
				src/GameObject/Components/DestroyComponent/DestroyComponent.cpp \
				src/GameObject/Components/DestroyPhysic/DestroyPhysic.cpp \
				src/GameObject/Components/DestroyPhysic/DestroyPhysicDynamic.cpp \
				src/GameObject/Components/Lua/LuaComponent.cpp \
				src/GameObject/Components/Player/APlayer.cpp \
				src/GameObject/Components/PowerUp/PowerUp.cpp \
				src/GameObject/Components/PowerUp/PowerUpOnCrate.cpp \
				src/GameObject/Components/PowerUp/PowerUpOnPlayer.cpp \
				src/GameObject/Components/Rule/Rule.cpp \
				src/GUI/GUI.cpp \
				src/Map/Map.cpp

SRCC=				include/commons/lua/lapi.c \
				include/commons/lua/lauxlib.c \
				include/commons/lua/lbaselib.c \
				include/commons/lua/lbitlib.c \
				include/commons/lua/lcode.c \
				include/commons/lua/lcorolib.c \
				include/commons/lua/lctype.c \
				include/commons/lua/ldblib.c \
				include/commons/lua/ldebug.c \
				include/commons/lua/ldo.c \
				include/commons/lua/ldump.c \
				include/commons/lua/lfunc.c \
				include/commons/lua/lgc.c \
				include/commons/lua/linit.c \
				include/commons/lua/liolib.c \
				include/commons/lua/llex.c \
				include/commons/lua/lmathlib.c \
				include/commons/lua/lmem.c \
				include/commons/lua/loadlib.c \
				include/commons/lua/lobject.c \
				include/commons/lua/lopcodes.c \
				include/commons/lua/loslib.c \
				include/commons/lua/lparser.c \
				include/commons/lua/lstate.c \
				include/commons/lua/lstring.c \
				include/commons/lua/lstrlib.c \
				include/commons/lua/ltable.c \
				include/commons/lua/ltablib.c \
				include/commons/lua/ltm.c \
				include/commons/lua/lua.c \
				include/commons/lua/luac.c \
				include/commons/lua/lundump.c \
				include/commons/lua/lutf8lib.c \
				include/commons/lua/lvm.c \
				include/commons/lua/lzio.c

OBJ		=		$(SRC:.cpp=.o)

OBJC		=		$(SRCC:.c=.o)

CXXFLAGS		=		-std=c++11 -I./include/linux -I./include/commons -I./include/linux/OGRE

LDFLAGS		= 		-L./libs/linux -lOgreMain -lCEGUIBase-0 -lpthread -lfreeimage-3.16.0 -lOIS-1.4.0 -lsfml-audio -lsfml-system -lCEGUIOgreRenderer-0 -Wl,-rpath,${PWD}/libs/linux

%.o : %.c
				gcc -c $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.cpp
				g++ -c $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

all:				$(NAME)

$(NAME):			$(OBJ) $(OBJC)
				g++ -o $(NAME) $(OBJ) $(OBJC) $(LDFLAGS)

install:
	./install.sh

clean:
				rm -f $(OBJ) $(OBJC)

fclean: clean
				rm -f $(NAME)

re:				fclean all
