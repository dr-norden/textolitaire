CC = gcc
CFLAGS = -std=c11 -Wall
TARGET=sol
MODULES=getche.o card.o solstack.o controls.o display.o table.o sol.o

all: $(TARGET)

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
