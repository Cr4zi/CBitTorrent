##
# CBitTorrent
#
# @file
# @version 0.1
CC			= g++
CFLAGS		= -Wall -Wextra -Iinclude
LDFLAGS		= -lssl -lcrypto

SRCDIR 		= src
OBJDIR		= obj
TARGET		= cbittorrent

SRC_FILES 	= $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/net/*.cpp)

OBJ_FILES	= $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/net
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

.PHONY: all clean
# end
