
IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LIBS=

_DEPS = editor.h printf_extentions.h rows.h utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = editor.o rows.o printf_extentions.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUT_DIR=out

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ste: $(OBJ)
	$(CC) -o $(OUT_DIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: ste

test_file: utils/create_test_file.py
	python $< $(OUT_DIR)/test

clean:
	rm -f $(ODIR)/* $(OUT_DIR)/* *~ core $(INCDIR)/*~
