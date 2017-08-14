#ifdef DEBUG 
TARGET := ../../Lib/Bit32/Linux/libAntThread-32D.a
#else
TARGET := ../../Lib/Bit32/Linux/libAntThread-32.a
#endif

SRCS = $(wildcard ../../Source/*/*.cpp)
OBJS = $(SRCS:.cpp = .o)
GCC = gcc
GXX = g++
LIBS := -lpthread
INCLUDE := -I../../Include -I../../Include/Public -I../../Include/Thread

#ifdef DEBUG 
GXXFLAGS = -g -Wall -O0
#else
GXXFLAGS = -Wall -O0
#endif




all:
	ar r $(TARGET) $(OBJS)

%.o : %.c
	$(GXX) -c $< $(GXXFLAGS) $(INCLUDE) $(LIBS)



.PHONY : clean
	rm *.o $(TARGET)
	.PHONY: clean


