beat:	main.cc
	g++ -L../../ -lnbox -O3 -o beat main.cc lib/libfmodex64.so -pthread

exec: beat
	LD_LIBRARY_PATH='../../' ./beat drumloop.wav map.nm weights.wt

clean:
	rm beat
