# PREFIX is environment variable, but if it is not set, then set default value
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

cmath:	main.cpp	math.cpp
	g++ -o cmath main.cpp math.cpp

install:	cmath
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 cmath $(DESTDIR)$(PREFIX)/bin/
