#
# Makefile
# alex, 2016-11-28 04:00
#

BASIC=parted
SOURCE=$(BASIC).c
OBJ=$(BASIC).o
TARGET=$(BASIC)
CC=gcc

all:
	$(CC) -o $(TARGET) $(SOURCE)

modify:
	$(CC) -DMODIFY -o $(TARGET) $(SOURCE)

# vim:ft=make
#
