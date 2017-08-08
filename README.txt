TO COMPLIE
	You must have X11!
	complie code : g++ -o eliminate eliminate.cpp -L/usr/X11R6/lib -lX11 -lstdc++
	run after complie : ./eliminate



NOTE BEFORE RUN:
There are three hardness level 1,2,3 (other value are not accpetable)
The speed can be set by user range: 1 to infinite (be careful, recommand 1 or 2! ehhh 3 maybe?)
The time can be set by user range: 1 to infinite(in second). (play as long as you wish)
The FPS will be constantly 30.


TO RUN:
In the command line argument:
	run with default: time = 15s speed=1 hardness=1
	run with 1 argument: time = argv[1]
	run with 2 argument: time = argv[1] speed = argv[2]
	run with 3 argument: time = argv[1] speed = argv[2] hardness = argv[3]
eg: ./elim 30 1 2 
	will play 30 second with 1 speed level and 2 hardness level.

Note: recommand speed level between 1 or 2.

Enjoy.
