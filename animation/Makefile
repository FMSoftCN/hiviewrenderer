libanimationrender.so : AnimationRender.o
	gcc -shared -fPIC -o libanimationrender.so AnimationRender.o

AnimationRender.o : AnimationRender.c AnimationRender.h HiViewCanvas.h
	gcc -c -fPIC AnimationRender.c

install:
	cp -f libanimationrender.so /opt/view-renderers/views.fmsoft.cn/libanimation-default.so

clean:
	rm  -f *.o *.so 
