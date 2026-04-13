CC = gcc
TARGET = pwgen
SRCDIR = source
SRCS = $(SRCDIR)/main.c $(SRCDIR)/parser.c $(SRCDIR)/generator.c $(SRCDIR)/platform.c $(SRCDIR)/metadata.c
OBJS = $(patsubst $(SRCDIR)/%.c,%.o,$(SRCS))
HEADERS = $(SRCDIR)/constant.h $(SRCDIR)/parser.h $(SRCDIR)/generator.h $(SRCDIR)/platform.h $(SRCDIR)/metadata.h

CFLAGS = -Os -DNDEBUG -w -fno-asynchronous-unwind-tables -fno-unwind-tables \
         -ffunction-sections -fdata-sections -I$(SRCDIR)
LDFLAGS = -Wl,--gc-sections -Wl,--strip-all -lsodium

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)