CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Iinclude 
LIBS=-lm
ARGET=atc

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
# you may need to put your extra files here
_DEPS = message.h data_to_struct.h utilities.h estimate_position.h number_of_contacts.h separation_issue.h free_list.h
_OBJS = main.o data_to_struct.o utilities.o estimate_position.o number_of_contacts.o separation_issue.o free_list.o

DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)




void func(char** cleaned) {
	cleaned[0][0] = 1
}

char cleaned[255][255]
func(cleaned)





