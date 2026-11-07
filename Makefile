CC = clang
CFLAGS = -Wall -Wextra -O3 -Iinclude
LIBS = -lcurl

SRC = src/main.c src/extractor.c src/ai_client.c
OBJ = $(SRC:.c=.o)
TARGET = cibpai

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f src/*.o $(TARGET)
