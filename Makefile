CC := g++
INC := -I ZAPD -I lib/assimp/include -I lib/elfio -I lib/json/include -I lib/stb -I lib/tinygltf -I lib/libgfxd -I lib/tinyxml2
CFLAGS += -g3 -ggdb -fpic -std=c++17 -Wall -fno-omit-frame-pointer

ifeq ($(OPTIMIZATION_ON),0)
  CFLAGS += -O0
else
  CFLAGS += -O2
endif
ifneq ($(ASAN),0)
  CFLAGS += -fsanitize=address
endif
ifneq ($(DEPRECATION_OFF),0)
  CFLAGS += -DDEPRECATION_OFF
endif
# CFLAGS += -DTEXTURE_DEBUG

LDFLAGS := -lstdc++ -lm -ldl -lpng

UNAME := $(shell uname)
ifneq ($(UNAME), Darwin)
    LDFLAGS += -Wl,-export-dynamic -lstdc++fs
endif

SRC_DIRS := ZAPD ZAPD/ZRoom ZAPD/ZRoom/Commands ZAPD/Overlays ZAPD/HighLevel ZAPD/OpenFBX

CPP_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
CPP_FILES += lib/tinyxml2/tinyxml2.cpp
O_FILES   := $(CPP_FILES:.cpp=.o)

all: ZAPD.out

genbuildinfo:
	python3 ZAPD/genbuildinfo.py

clean:
	rm -f $(O_FILES) ZAPD.out
	$(MAKE) -C lib/libgfxd clean

rebuild: clean all

%.o: %.cpp
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

ZAPD/Main.o: genbuildinfo ZAPD/Main.cpp
	$(CC) $(CFLAGS) $(INC) -c ZAPD/Main.cpp -o $@

lib/libgfxd/libgfxd.a:
	$(MAKE) -C lib/libgfxd -j

ZAPD.out: $(O_FILES) lib/libgfxd/libgfxd.a
	$(CC) $(CFLAGS) $(INC) $(O_FILES) lib/libgfxd/libgfxd.a -o $@ $(FS_INC) $(LDFLAGS)
