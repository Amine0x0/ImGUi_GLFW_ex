CC = c++ -std=c++11
FLAGS = -Wall -Wextra -g3 -fsanitize=leak 
SRC_FOLDER = ./src
INCL_FOLDER = ./incl
IMGUI_FOLDER = ./imgui
BACKEND_FOLDER = ./imgui/backends
IMPLOT_FOLDER = ./implot

SRCS = $(wildcard $(SRC_FOLDER)/*.cpp) \
       $(wildcard $(SRC_FOLDER)/App/**/*.cpp) \
       $(wildcard $(IMGUI_FOLDER)/*.cpp) \
       $(BACKEND_FOLDER)/imgui_impl_glfw.cpp \
       $(BACKEND_FOLDER)/imgui_impl_opengl3.cpp \
       $(IMPLOT_FOLDER)/implot.cpp \
       $(IMPLOT_FOLDER)/implot_items.cpp

OBJS = $(SRCS:.cpp=.o)
EXEC = app

INCLUDES = -I$(INCL_FOLDER) -I$(IMGUI_FOLDER) -I$(BACKEND_FOLDER) -I$(IMPLOT_FOLDER)
LIBS = -lglfw -lGL -ldl -lpthread

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $@ $(LIBS)

%.o: %.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

re: fclean all

clean:
	rm -f $(OBJS) $(EXEC)

fclean: clean
	rm -rf $(EXEC)