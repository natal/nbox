beat:	main.cc
	g++ -L../../ -lnbox -o beat main.cc lib/libfmodex.so -pthread -g

exec: beat
	LD_LIBRARY_PATH='../../' ./beat drumloop.wav map.nm weights.wt

debug: beat
	LD_LIBRARY_PATH='../../' gdb ./beat

clean:
	rm beat
