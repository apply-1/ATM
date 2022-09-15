CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
BIN=open destory login unlock query save take repass transfer
TARGE=sever client
RELY=bank.h msgq.c msgq.h tools.c tools.h

all:$(BIN) $(TARGE)
	mv $(BIN) ./bin
	./sever 

%:%.c $(RELY)
	$(CC) $(STD) $(FLAG) tools.c msgq.c $< -o $@

clean:
	cd ./bin && rm -rf $(BIN)
	rm -rf $(TARGE)
	rm -rf *.o
	rm -rf *~ *.h.gch












