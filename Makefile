obj=main

build: main.cpp io_base.h page_cache.h mmap.h direct.h
	g++ -g -o main main.cpp

page_cache: io_base.h page_cache.h

clean:
	rm main tmp/*