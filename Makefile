CC := gcc

AR := ar

CFLAGS := -Wall -Wextra -Iinclude -I/usr/include/freetype2
LDFLAGS := -static
LDLIBS := $(shell pkg-config --static --libs freetype2)

PRETTYNAME := Akita Neru
MM_VER     := 10
DD_VER     := 03
YY_VER     := 26
PATCH_VER  := 00

GITHUB_PAGE := https://github.com/GDDASH45/tetoTerritory

VERSTRING  := $(MM_VER).$(DD_VER).$(YY_VER).$(PATCH_VER)

# This file will be generated so the DE can use it, and also different parts of the DE
GENERATED_VERSION_NAME_HEADER := include/generated/DEver.h

SOURCES := $(shell find src -type f -name '*.c')

OBJECTS := $(SOURCES:.c=.o)

LIB_DIRS := $(sort $(dir $(OBJECTS)))

LIBRARIES := $(addsuffix built-in.a,$(LIB_DIRS))

TARGET := bin/tetoTerritory

# Applications
APP_SOURCES := $(shell find apps -mindepth 2 -type f -name 'main.c')
APP_NAMES := $(notdir $(patsubst %/main.c,%,$(APP_SOURCES)))
APP_TARGETS := $(addprefix bin/apps/,$(APP_NAMES))

DEB_DIR := deb
DEB_PACKAGE := $(DEB_DIR)/tetoTerritory.deb

all: $(TARGET) apps

$(GENERATED_VERSION_NAME_HEADER):
	@mkdir -p $(dir $@)
	@printf '#ifndef INCLUDE_GEN_DEVER_H\n' > $@
	@printf '#define INCLUDE_GEN_DEVER_H\n' >> $@
	@printf '\n' >> $@
	@printf '#define DE_PRETTYNAME "$(PRETTYNAME)"\n' >> $@
	@printf '\n' >> $@
	@printf '#define DE_VERSION "$(VERSTRING)"\n' >> $@
	@printf '\n' >> $@
	@printf '#define DE_HOME_PAGE "$(GITHUB_PAGE)"\n' >> $@
	@printf '\n' >> $@
	@printf '#endif\n' >> $@

$(TARGET): $(OBJECTS) $(LIBRARIES) $(GENERATED_VERSION_NAME_HEADER)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBRARIES) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

src/built-in.a: $(filter src/%.o,$(OBJECTS))
	$(AR) rcs $@ $^

src/%/built-in.a:
	$(AR) rcs $@ $(filter src/$*%.o,$(OBJECTS))

# Build all applications
apps: $(APP_TARGETS)

# Build an individual application
bin/apps/%: apps/%/main.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< src/framebuffer.c src/wm/main.c $(LDFLAGS) -o $@ $(LDLIBS)

debpkg: $(TARGET)
	@rm -rf $(DEB_DIR)
	@mkdir -p $(DEB_DIR)/DEBIAN
	@mkdir -p $(DEB_DIR)/usr/bin
	@mkdir -p $(DEB_DIR)/usr/share/tetoDE
	@cp $(TARGET) $(DEB_DIR)/usr/bin/tetoTerritory
	@cp share/teto.ttf $(DEB_DIR)/usr/share/tetoDE/teto.ttf
	@printf '%s\n' \
		'Package: tetode' \
		'Version: $(VERSTRING)' \
		'Section: x11' \
		'Priority: optional' \
		'Architecture: amd64' \
		'Maintainer: tetoDE developers' \
		'Description: tetoDE desktop environment' \
		> $(DEB_DIR)/DEBIAN/control
	@dpkg-deb --build $(DEB_DIR) $(DEB_PACKAGE)
	@echo "DEB package created: $(DEB_PACKAGE)"

clean:
	find src -type f -name '*.o' -delete
	find src -type f -name 'built-in.a' -delete
	rm -f include/generated/DEver.h
	rm -f built-in.a
	rm -rf bin/apps
	rm -rf deb
	rm -f $(TARGET)

.PHONY: all apps debpkg clean