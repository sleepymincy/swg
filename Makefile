#  mmm  mmm     mm     mm   mmm  mmmmmmmm  mmmmmmmm   mmmmmm   mm        mmmmmmmm 
#  ###  ###    ####    ##  ##"   ##""""""  ##""""""   ""##""   ##        ##"""""" 
#  ########    ####    ##m##     ##        ##           ##     ##        ##       
#  ## ## ##   ##  ##   #####     #######   #######      ##     ##        #######  
#  ## "" ##   ######   ##  ##m   ##        ##           ##     ##        ##       
#  ##    ##  m##  ##m  ##   ##m  ##mmmmmm  ##         mm##mm   ##mmmmmm  ##mmmmmm 
#  ""    ""  ""    ""  ""    ""  """"""""  ""         """"""   """"""""  """""""" 
#
# Wooooo cool ASCII art :3 ~

# Compiler settings:
CC				:= clang
CFLAGS			:= -std=gnu11 -Wall -Wextra -Iinclude -fsanitize=memory
LDFLAGS			:= -lm
RELEASE_FLAGS	:= -O2

# Paths and files:
SRC				:= src/main.c src/args.c src/io.c src/generator.c
OBJ				:= $(patsubst src/%.c,build/%.o,$(SRC))
TARGET			:= build/swg

# Installation paths:
prefix			?= /usr/local
exec_prefix		?= $(prefix)
bindir			?= $(exec_prefix)/bin

.PHONY: all release clean install uninstall

# Default target:
all: $(TARGET)

# Link: 
$(TARGET): $(OBJ) | build
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
	@echo "=> linked $@"

# Compile:
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "=> compiled $<"

# Clean up:
.INTERMEDIATE: $(OBJ)
clean:
	rm -rf build

# Make sure ./build exists:
build:
	mkdir -p build

# Build release:
release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

# Install:
install: release
	@echo "=< installing to $(DESTDIR)$(bindir)/swg"
	install -d $(DESTDIR)$(bindir)
	install -m 755 build/swg $(DESTDIR)$(bindir)/swg

# Unsintall:
uninstall:
	@echo "=< removing $(DESTDIR)$(bindir)/swg"
	-rm -f $(DESTDIR)$(bindir)/swg