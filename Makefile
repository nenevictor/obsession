app_name = obs

src_files = src/main.cpp src/OApp.cpp src/OAppSettings.cpp src/OAppLanguage.cpp src/OAppUI.cpp

include_path = include
lib_path = lib

libs = -lraylib -lopengl32 -lm

ifeq ($(OS),Windows_NT)
	libs += -lgdi32 -lwinmm
endif

debug:
	clear
	g++ -O0 -Wall -Wextra -Werror -std=c++11 -D _DEBUG -I$(include_path) -L$(lib_path) $(src_files) -o $(app_name)_d $(libs)
	$(app_name)_d

product:
	clear
	g++ -Ofast -Wall -Wextra -std=c++11 -I$(include_path) -L$(lib_path) $(src_files) -o $(app_name) $(libs) -mwindows