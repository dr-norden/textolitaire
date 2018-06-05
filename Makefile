CC = gcc
CFLAGS = -std=c11 -Wall
TARGET=sol
MODULES=getche.o card.o solstack.o controls.o display.o table.o sol.o diskdata.o

all: $(TARGET)

textolitaire2: getche.o card.o solstack.o controls.o display-curses.o table.o sol.o diskdata.o
	@echo "[LD] $@ $^"
	@ $(CC) -lncursesw $^ -o $@

display-curses.o: display-curses.c
	@echo "[CC] $@ $<"
	@ $(CC) $(CFLAGS) -c $< -o $@


%.o: %.c %.h
	@echo "[CC] $@ $<"
	@ $(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(MODULES)
	@echo "[LD] $@ $^"
	@ $(CC) $^ -o $@

run:: $(TARGET)
	./$<

clean::
	@echo "[RM] $(TARGET) $(MODULES)"
	@ rm -f $(TARGET) $(MODULES)

again::
	@ $(MAKE) clean
	@ $(MAKE)
