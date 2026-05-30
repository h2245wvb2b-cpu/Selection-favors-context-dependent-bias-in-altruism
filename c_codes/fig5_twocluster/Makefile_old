CC = gcc
CFLAGS = -Wall
INCLUDES = -I/opt/local/include
LFLAGS = -L/opt/local/lib
LIBS = -lgsl -lgslcblas -lm
SOURCES = sim_main.c append_files.c initialize.c interact.c new_generation.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = execute_sim

all: $(EXECUTABLE) clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -O3 -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o
