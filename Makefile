app_name_product = obs
app_name_debug = $(app_name_product)_d
app_name = obs

app_ext = .elf

main_file_path = src/main.cpp

include_path = include
lib_path = lib

libs = -lraylib -lopengl32 -lm

ifeq ($(OS),Windows_NT)
  libs += -lgdi32 -lwinmm
  app_ext = .exe
endif

debug:
  clear
  g++ -O0 -Wall -Wextra -Werror -std=c++11 -D _DEBUG $(main_file_path) -I$(include_path) -L$(lib_path) -o $(app_name_debug)$(app_ext) $(libs)
  $(app_name_debug)$(app_ext)

product:
  clear
  g++ -Ofast -Wall -Wextra -std=c++11 $(main_file_path) -I$(include_path) -L$(lib_path) -o $(app_name)$(app_ext) $(libs) -mwindows
