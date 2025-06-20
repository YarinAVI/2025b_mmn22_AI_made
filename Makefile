CC = gcc
CFLAGS = -Wall -ansi -pedantic
OBJS = mainmat.o mymat.o
TARGET = mainmat

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

mainmat.o: mainmat.c mymat.h
	$(CC) $(CFLAGS) -c mainmat.c

mymat.o: mymat.c mymat.h
	$(CC) $(CFLAGS) -c mymat.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./$(TARGET) < test_valid.txt > output_valid.txt
	./$(TARGET) < test_errors.txt > output_errors.txt
	./$(TARGET) < test_edge.txt > output_edge.txt