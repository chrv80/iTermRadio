CC      := gcc
PKG     := mpv

CFLAGS  := -Wall -Wextra -O2 -Iinclude -MMD -MP \
           $(shell pkg-config --cflags $(PKG))

LDLIBS  := -lncurses -lpanel -lmenu \
           $(shell pkg-config --libs $(PKG))

TARGET  := iTermRadio

SRC_DIR := src
OBJ_DIR := build
BINDIR = /usr/sbin

SRC     := $(wildcard $(SRC_DIR)/*.c)
OBJ     := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

.PHONY: all clean run install uninstall
.SILENT:

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

install:
	@echo "Installing to $(BINDIR)"
	@cp $(TARGET) $(BINDIR)/
	@chmod +x $(BINDIR)/$(TARGET)
	@echo "Done."

uninstall:
	@echo "Removing from $(BINDIR)"
	@rm -f $(BINDIR)/$(TARGET)
	@echo "Done."

clean:
	@rm -rf $(OBJ_DIR)

-include $(OBJ:.o=.d)