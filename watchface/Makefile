libwatchfacerender.so : WatchfaceRender.o
	gcc -shared -fPIC -o libwatchfacerender.so WatchfaceRender.o

WatchfaceRender.o : WatchfaceRender.c WatchfaceRender.h HiViewCanvas.h
	gcc -c -fPIC WatchfaceRender.c

install:
	mkdir -p /opt/view-renderers/views.fmsoft.cn/
	ln -sf /opt/view-renderers/views.fmsoft.cn /opt/view-renderers/views.default
	cp -f libwatchfacerender.so /opt/view-renderers/views.fmsoft.cn/libwatchface-default.so

clean:
	rm  -f *.o *.so 
