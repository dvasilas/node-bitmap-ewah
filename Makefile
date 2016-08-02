BITMAP-EWAH		= $(dir $(abspath $(MAKEFILE_LIST)))
TARGET 			= $(BITMAP-EWAH)src/EWAHBoolArray
SRCDIR 			= $(BITMAP-EWAH)src

all: | $(SRCDIR) $(TARGET)

$(SRCDIR):
	mkdir $(SRCDIR)

$(TARGET):
	git clone https://github.com/lemire/EWAHBoolArray.git $@
