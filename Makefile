CXX = gcc
CXXFLAGS = -g -Wextra # Mettre -O1 ou -O2 à la place de -g pour la version prod
HEADERS_LOCALISATION = include

LDFLAGS = -lmingw32 -lSDL2main -lSDL2 # -mwindows pour supprimer l'affichage de la console
LIB_LOCALISATION = lib

EXEC = Chip8-Emulator

SRC = inout.c chip8.c fileSize.c main.c
OBJ = $(SRC:.c=.o)

all : program

program : $(OBJ)
	$(CXX) -L $(LIB_LOCALISATION) $(addprefix obj\, $(OBJ)) -o bin\$(EXEC) $(LDFLAGS)

%.o: src\%.c
	$(CXX) $(CXXFLAGS) -I $(HEADERS_LOCALISATION) -c $< -o obj\$@

clean:
	rm obj/*.o bin/*.exe

.PHONY: all program clean