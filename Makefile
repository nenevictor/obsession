app_name_product = obs
app_name_debug = $(app_name_product)_d
app_name = obs

main_file_path = src/main.cpp

include_path = include
lib_path = lib

libs = -lraylib -lopengl32 -lm

ifeq ($(OS),Windows_NT)
  libs += -lgdi32 -lwinmm
endif

debug:
  clear
  g++ -O0 -Wall -Wextra -Werror -std=c++11 -D _DEBUG $(main_file_path) -I$(include_path) -L$(lib_path) -o $(app_name_debug) $(libs)
  $(app_name_debug)$(app_ext)

product:
  clear
  g++ -Ofast -Wall -Wextra -std=c++11 $(main_file_path) -I$(include_path) -L$(lib_path) -o $(app_name) $(libs) -mwindows
