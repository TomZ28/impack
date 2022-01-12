impack : impack.o impack_utils.o impack_pack.o impack_extract.o
	gcc -std=gnu99 -o impack $^

%.o : %.c impack.h impack_utils.h
	gcc -std=gnu99 -c $<

clear :
	rm -f *.o *~

clean :
	rm -f *.o impack *~