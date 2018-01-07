CC=g++
CFLAGS= -Wall -g
BOOST_LIB=/lib/gcc/x86_64-pc-cygwin/6.4.0/include/c++
TARGET=~/HurricaneLoss
OBJS= CalculateLoss.o gethurricaneloss.o
gethurricaneloss:${OBJS}
	${CC} -I${BOOST_LIB} -L/lib ${CFLAGS} -o $@ ${OBJS}
clean:
	-rm -f *.o core *.core *.exe
install: gethurricaneloss
	test -d ${TARGET} || mkdir ${TARGET}
	install -m 0755 gethurricaneloss ${TARGET}



