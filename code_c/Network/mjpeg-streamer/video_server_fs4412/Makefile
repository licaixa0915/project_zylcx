CC=arm-none-linux-gnueabi-gcc
JPEG_HEAD_PATH=-I ./include
JPEG_LIB_PATH=-L ./lib 

CFLAGS=-c -g $(JPEG_HEAD_PATH)

mjpg_streamer:main.o http.o camera.o compress.o
	$(CC) main.o http.o camera.o compress.o -o mjpg_streamer -lpthread $(JPEG_LIB_PATH) -ljpeg

main.o:main.c
	$(CC) $(CFLAGS) main.c -o main.o 

http.o:http.c
	$(CC) $(CFLAGS) http.c -o http.o 

camera.o:camera.c
	$(CC) $(CFLAGS) camera.c -o camera.o 

compress.o:compress.c
	$(CC) $(CFLAGS) compress.c -o compress.o

clean:
	rm -rf *.o mjpg_streamer
