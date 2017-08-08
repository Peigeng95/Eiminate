/*

eliminate app, player need to try to eliminate all things before they drop.
success of elimination gain 5 score.
failure will reduce 5 score

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o eliminate eliminate.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./eliminate

*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
const int Border = 5;
const int BufferSize = 10;
int FPS = 30;
int score = 0;
int lives = 3;
int speed = 2;
int playtime = 3999;
int hardness = 100;
/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	
	Pixmap	pixmap;		// double buffer
	int		width;		// size of pixmap
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		int y;
		int x;
		virtual void paint(XInfo &xinfo) = 0;
		virtual bool isV()=0;
		virtual void move(XInfo &xinfo) = 0;
		virtual void setV() = 0;
};       

class Ball : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, diameter, diameter, 0, 360*64);
		}
		
		void move(XInfo &xinfo) {
			playtime --;
			if (playtime == 0) throw(3);
			y = y - directionY;
			
			if (y < 0){
				directionY = -directionY;
			}
			if ((y > xinfo.height - diameter) && visiable){
				visiable = false;
				throw (1);
			}
		}
		void reverse() {
			directionY = -directionY;
		}
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}
		
		Ball(int x, int y, int diameter): diameter(diameter) {
			this->x = x;			
			directionX = 1 * speed;
			directionY = 5 * speed;
			visiable = true;
		}
		bool isV() {
			return visiable;
		}
		void setV() {
			visiable = false;
		}
	private:
		int diameter;
		int directionX;
		int directionY;
		bool visiable;
};


list<Displayable *> dList;           // list of Displayables
list<Displayable *> leftList;
list<Displayable *> rightList;
list<Displayable *> upList;
list<Displayable *> downList;
Ball balll1(75, 0, 20);
Ball balll2(75, 0, 20);
Ball balll3(75, 0, 20);
Ball balll4(75, 0, 20);
Ball balll5(75, 0, 20);
Ball balll6(75, 0, 20);
Ball balll7(75, 0, 20);
Ball balll8(75, 0, 20);
Ball balll9(75, 0, 20);
Ball balll10(75, 0, 20);
Ball ballu1(232, 0, 20);
Ball ballu2(232, 0, 20);
Ball ballu3(232, 0, 20);
Ball ballu4(232, 0, 20);
Ball ballu5(232, 0, 20);
Ball ballu6(232, 0, 20);
Ball ballu7(232, 0, 20);
Ball ballu8(232, 0, 20);
Ball ballu9(232, 0, 20);
Ball ballu10(232, 0, 20);
Ball balld1(389, 0, 20);
Ball balld2(389, 0, 20);
Ball balld3(389, 0, 20);
Ball balld4(389, 0, 20);
Ball balld5(389, 0, 20);
Ball balld6(389, 0, 20);
Ball balld7(389, 0, 20);
Ball balld8(389, 0, 20);
Ball balld9(389, 0, 20);
Ball balld10(389, 0, 20);
Ball ballr1(546, 0, 20);
Ball ballr2(546, 0, 20);
Ball ballr3(546, 0, 20);
Ball ballr4(546, 0, 20);
Ball ballr5(546, 0, 20);
Ball ballr6(546, 0, 20);
Ball ballr7(546, 0, 20);
Ball ballr8(546, 0, 20);
Ball ballr9(546, 0, 20);
Ball ballr10(546, 0, 20);


/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xinfo) {
	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xinfo.display = XOpenDisplay( "" );
	if ( !xinfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xinfo.screen = DefaultScreen( xinfo.display );

	white = XWhitePixel( xinfo.display, xinfo.screen );
	black = XBlackPixel( xinfo.display, xinfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 630;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xinfo.window = XCreateSimpleWindow( 
		xinfo.display,				// display where window appears
		DefaultRootWindow( xinfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xinfo.display,		// display containing the window
		xinfo.window,		// window whose properties are set
		"x3_doublebuffer",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	// Reverse Video
	i = 1;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
	XSetFillStyle(xinfo.display, xinfo.gc[i], FillSolid);
	XSetLineAttributes(xinfo.display, xinfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 2;
	xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XSetForeground(xinfo.display, xinfo.gc[i], WhitePixel(xinfo.display, xinfo.screen));
	XSetBackground(xinfo.display, xinfo.gc[i], BlackPixel(xinfo.display, xinfo.screen));
        XSetLineAttributes(xinfo.display, xinfo.gc[i],
                             1, LineSolid, CapButt, JoinRound);

	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, hints.width, hints.height, depth);
	xinfo.width = hints.width;
	xinfo.height = hints.height;


	XSelectInput(xinfo.display, xinfo.window, 
		ButtonPressMask | KeyPressMask | PointerMotionMask 
		| EnterWindowMask | LeaveWindowMask
		| StructureNotifyMask   // for resize events
		);

	/*
	 * Don't paint the background -- reduce flickering
	 */
	XSetWindowBackgroundPixmap(xinfo.display, xinfo.window, None);

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xinfo.display, xinfo.window );
	
	XFlush(xinfo.display);
}

/*
 * Function to repaint a display list
 */

//repaintzhe
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();
	// draw into the buffer
	// note that a window and a pixmap are “drawables”
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height);
	//xian
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 
		0, 500, 630, 1);
	while( begin != end ) {
		Displayable *d = *begin;
		if(d->isV()){
			d->paint(xinfo); // the displayables know about the pixmap
		}
		begin++;
	}
	string text = "your Score:";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 5, 20, text.c_str(), text.length());
	stringstream ss;
        ss << score;
        string t_score;
        ss >> t_score;
        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 100, 20, t_score.c_str(), t_score.length());
	string ptime = "time Left:";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 150, 20, ptime.c_str(), ptime.length());
	stringstream sst;
			sst << playtime;
			string t_ptime;
			sst >> t_ptime;
			XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 250, 20, t_ptime.c_str(), t_ptime.length());
	string left = "left";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 75, 40, left.c_str(), left.length());
	string up = "up";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 232, 40, up.c_str(), up.length());
	string down = "down";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 389, 40, down.c_str(), down.length());
	string right = "right";
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 546, 40, right.c_str(), right.length());
	// copy buffer to window
	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
		0, 0); // position to put top left corner of pixmap in window

	XFlush( xinfo.display );

}


void handleButtonPress(XInfo &xinfo, XEvent &event) {
	printf("Got button press!\n");
	// dList.push_front(new Text(event.xbutton.x, event.xbutton.y, "Urrp!"));
	// repaint( dList, xinfo );
	
}

//eliminatezhe
void eliminateup(){
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();
	while( begin != end ) {
		Displayable *d = *begin;
		if((d->x == 232)&&(d-> y < 550) && (d->y > 450)){
			d->setV();
			score = score + 5;
		}
		begin++;
	}
}

void eliminatedown(){
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();
	while( begin != end ) {
		Displayable *d = *begin;
		if((d->x == 389)&&(d-> y < 550) && (d->y > 450)){
			d->setV();
			score = score + 5;
		}
		begin++;
	}
}
void eliminateleft(){
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();
	while( begin != end ) {
		Displayable *d = *begin;
		if((d->x == 75)&&(d-> y < 550) && (d->y > 450)){
			d->setV();
			score = score + 5;
		}
		begin++;
	}
}
void eliminateright(){
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();
	while( begin != end ) {
		Displayable *d = *begin;
		if((d->x == 546)&&(d-> y < 550) && (d->y > 450)){
			d->setV();
			score = score + 5;
		}
		begin++;
	}
}
void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
//anjian
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		}
		if (text[0] == 'w'){
			eliminateup();
		}
		if (text[0] == 's'){
			eliminatedown();
		}
		if (text[0] == 'd'){
			eliminateright();
		}
		if (text[0] == 'a'){
			eliminateleft();
		}
	}
}

void handleMotion(XInfo &xinfo, XEvent &event, int inside) {
}

void handleResize(XInfo &xinfo, XEvent &event) {
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);
	if (xce.width > xinfo.width || xce.height > xinfo.height) {
		XFreePixmap(xinfo.display, xinfo.pixmap);
		int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
		xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xce.width, xce.height, depth);
		xinfo.width = xce.width;
		xinfo.height = xce.height;
	}
}

void creatleft(){
	list<Displayable *>::const_iterator begin = leftList.begin();
	Displayable *left = *begin;
	dList.push_front(left);
	leftList.pop_front();
}
void creatup(){
	list<Displayable *>::const_iterator begin = upList.begin();
	Displayable *up = *begin;
	dList.push_front(up);
	upList.pop_front();
}

void creatdown(){
	list<Displayable *>::const_iterator begin = downList.begin();
	Displayable *down = *begin;
	dList.push_front(down);
	downList.pop_front();
}
	
void creatright(){
	list<Displayable *>::const_iterator begin = rightList.begin();
	Displayable *right = *begin;
	dList.push_front(right);
	rightList.pop_front();
}

void creatakey(){
	if((leftList.size() == 0) && (rightList.size() == 0) && (upList.size() == 0) && (downList.size() == 0)){
		return;
	}
	int go = rand() % 4;
	cout << go << endl;
	if(go == 0){
		if(leftList.size() > 0) creatleft();
		else creatakey();
	}
	if(go == 1){
		if(rightList.size() > 0) creatright();
		else creatakey();
	}
	if(go == 2){
		if(upList.size() > 0) creatup();
		else creatakey();
	}
	if(go == 3){
		if(downList.size() > 0) creatdown();
		else creatakey();
	}
}

//anzhe
void handleAnimation(XInfo &xinfo, int inside) {
	try{	
		if((playtime % hardness == 0) && (playtime >= 1000)){
			creatakey();
		}
		list<Displayable *>::const_iterator begin = dList.begin();
		list<Displayable *>::const_iterator end = dList.end();
		while( begin != end ) {
			Displayable *d = *begin;
			d->move(xinfo);
			begin++;
		}
	}
	catch(int drop){
		if(drop == 3) throw (3);
		if(drop == 1){//shibai
			score = score - 5;
		}
		else throw (0);
			
	}
}

//call


void GameOver(XInfo &xinfo, int gameover) {
   XClearWindow(xinfo.display, xinfo.window);

   string overtype;
   if(gameover == 1) {
      overtype = "GAME OVER";
   }
   else {
      overtype = " YOU WIN";
   }
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 250, 400, overtype.c_str(), overtype.length());


   string t = "Score : ";
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 250, 450, t.c_str(), t.length());

   stringstream ss;
   ss << score;
   string t_score;
   ss >> t_score;
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 300, 450, t_score.c_str(), t_score.length());


   string gg = "Press 'Q' to Quit";
   XDrawString(xinfo.display, xinfo.window, xinfo.gc[1], 250, 500, gg.c_str(), gg.length());

   XFlush(xinfo.display);
   
   XEvent event;
   while(true) {
      XNextEvent(xinfo.display, &event);
      if(event.type == KeyPress) {
			KeySym key;
			char text[BufferSize];
			int i = XLookupString( 
			(XKeyEvent *)&event, 	// the keyboard event
			text, 					// buffer when text will be written
			BufferSize, 			// size of the text buffer
			&key, 					// workstation-independent key symbol
			NULL );					// pointer to a composeStatus structure (unused)
			if ( i == 1) {
				if (text[0] == 'q') {
					exit(0);
				}
			}
   	}
	}
}


unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	leftList.push_front(&balll1);
	leftList.push_front(&balll2);
	leftList.push_front(&balll3);
	leftList.push_front(&balll4);
	leftList.push_front(&balll5);
	leftList.push_front(&balll6);
	leftList.push_front(&balll7);
	leftList.push_front(&balll8);
	leftList.push_front(&balll9);
	leftList.push_front(&balll10);
	upList.push_front(&ballu1);
	upList.push_front(&ballu2);
	upList.push_front(&ballu3);
	upList.push_front(&ballu4);
	upList.push_front(&ballu5);
	upList.push_front(&ballu6);
	upList.push_front(&ballu7);
	upList.push_front(&ballu8);
	upList.push_front(&ballu9);
	upList.push_front(&ballu10);
	downList.push_front(&balld1);
	downList.push_front(&balld2);
	downList.push_front(&balld3);
	downList.push_front(&balld4);
	downList.push_front(&balld5);
	downList.push_front(&balld6);
	downList.push_front(&balld7);
	downList.push_front(&balld8);
	downList.push_front(&balld9);
	downList.push_front(&balld10);
	rightList.push_front(&ballr1);
	rightList.push_front(&ballr2);
	rightList.push_front(&ballr3);
	rightList.push_front(&ballr4);
	rightList.push_front(&ballr5);
	rightList.push_front(&ballr6);
	rightList.push_front(&ballr7);
	rightList.push_front(&ballr8);
	rightList.push_front(&ballr9);
	rightList.push_front(&ballr10);
	creatakey();
	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while( true ) {
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case ButtonPress:
					handleButtonPress(xinfo, event);
					break;
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case MotionNotify:
					handleMotion(xinfo, event, inside);
					break;
				case EnterNotify:
					//cout << "enter" << endl;
					inside = 1;
					break;
				case LeaveNotify:
					//cout << "leave" << endl;
					inside = 0;
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;
			}
		}
		else{
			usleep(1000000/FPS);
			try{
				handleAnimation(xinfo, inside);
			}
			catch(int drop){
				throw drop;
			}
			repaint(xinfo);
		}
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	if (argc == 1){
		FPS = 30;
		speed = 2;
	}
	else if (argc == 2){
		stringstream ss;
        	ss << argv[1];
        	ss >> playtime;
			playtime = playtime * 266;
		//cout << "2 " << FPS<< endl;
	}
	else if (argc == 3){
		stringstream ss;
        	ss << argv[1];
        	ss >> playtime;
			playtime = playtime * 266;
		stringstream ss1;
		ss1 << argv[2];
		ss1 >> speed;
	}
	else if (argc == 4){
		stringstream ss;
        	ss << argv[1];
        	ss >> playtime;
			playtime = playtime * 266;
		stringstream ss1;
			ss1 << argv[2];
			ss1 >> speed;
		stringstream ss2;
			ss2 << argv[3];
			ss2 >> hardness;
			if(hardness == 1){
				hardness = 100;
			}
			if(hardness == 2){
				hardness = 50;
			}
			if(hardness == 3){
				hardness = 25;
			}
	}
	cout << FPS <<  " FPS" << endl;
	cout << speed<< " speed level" << endl;
 	XInfo xinfo;

	initX(argc, argv, xinfo);
	try{
		eventLoop(xinfo);
	}
	catch(int gameover){
		if ( gameover == 1 || gameover == 3) {
			printf("GAME OVER\n");
			repaint(xinfo);
			GameOver(xinfo, gameover);
		}
	}
	XCloseDisplay(xinfo.display);
}
