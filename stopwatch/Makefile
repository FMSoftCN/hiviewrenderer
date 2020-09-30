libstopwatchrender.so : StopwatchRender.o
	gcc -shared -fPIC -o libstopwatchrender.so StopwatchRender.o

StopwatchRender.o : StopwatchRender.c StopwatchRender.h HiViewCanvas.h
	gcc -c -fPIC StopwatchRender.c

install:
	mkdir -p /opt/view-renderers/views.fmsoft.cn/
	ln -sf /opt/view-renderers/views.fmsoft.cn /opt/view-renderers/views.default
	cp -f libstopwatchrender.so /opt/view-renderers/views.fmsoft.cn/libstopwatch-default.so

clean:
	rm  -f *.o *.so 
