CC = gcc
CFLAGS = -Wall
INCLUDES = -I/opt/homebrew/include
LFLAGS = -L/opt/homebrew/lib
LIBS = -lgsl -lgslcblas -lm
SOURCES = sim_main.c append_files.c initialize.c interact.c new_generation.c hyp_optima.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = execute_sim
all: $(EXECUTABLE) clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -O3 -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o

