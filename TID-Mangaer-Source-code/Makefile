CC = g++
CFLAGS = -Wall -Wextra -pthread
SRC_DIR = .

# Executable targets for each task
# Note that a different executable file was made for each task since
# each of them has a different main class and could no be compiled into
# one executable.

TARGET1 = Task1
TARGET2 = Task2
TARGET3 = Task3a
TARGET4 = Task3b
TARGET5 = Task1_Test

# Source and header files for each target
SRC_FILES1 = $(SRC_DIR)/Task1.c
SRC_FILES2 = $(SRC_DIR)/Task2.c
SRC_FILES3 = $(SRC_DIR)/Task3a.c
SRC_FILES4 = $(SRC_DIR)/Task3b.c
SRC_FILES5 = $(SRC_DIR)/Task1_Test.c
INC_FILES = $(SRC_DIR)/tid_map.h

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

$(TARGET1): $(SRC_FILES1) $(INC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES1) -o $(TARGET1)

$(TARGET2): $(SRC_FILES2) $(INC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES2) -o $(TARGET2)

$(TARGET3): $(SRC_FILES3) $(INC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES3) -o $(TARGET3)

$(TARGET4): $(SRC_FILES4) $(INC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES4) -o $(TARGET4)

$(TARGET5): $(SRC_FILES5) $(INC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES5) -o $(TARGET5)

clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

