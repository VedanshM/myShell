CC := gcc
CFLAGS = -Wall -I$(INC_DIR)
TARGET:= mysh

SRC_DIR := src
OBJ_DIR:= obj
INC_DIR:= include
DEP_DIR:= obj

CFILES:= $(shell ls $(SRC_DIR))
SRCFILES:= $(patsubst %.c,$(SRC_DIR)/%.c,$(CFILES))
OBJECTS:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(CFILES))
DFILES:= $(patsubst %.c,$(DEP_DIR)/%.d,$(CFILES))

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

include $(DFILES)

$(DEP_DIR)/%.d: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MM -MP -MT $(OBJ_DIR)/$(*F).o -MT $@ $< -MF $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ_DIR)/* 
