app_name = obs

src_files = src/main.cpp src/OApp.cpp src/OAppSettings.cpp src/OAppLanguage.cpp src/OAppUI.cpp

include_path = include
lib_path = lib

libs = -lraylib -lm

ifeq ($(OS),Windows_NT)
	libs += -lopengl32 -lgdi32 -lwinmm
endif

ifeq ($(OS),Linux)
	libs += -lGL
endif

debug:
	clear
	g++ -O0 -Wall -Wextra -Werror -std=c++11 -D _DEBUG -I$(include_path) -L$(lib_path) $(src_files) -o $(app_name)_d $(libs)
	./$(app_name)_d

product:
	g++ -Ofast -std=c++11 -I$(include_path) -L$(lib_path) $(src_files) -o $(app_name) $(libs)
