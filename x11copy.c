#include "x11copy.h"
Display * display;
Window window;
Atom targets_atom, text_atom, UTF8, XA_ATOM = 4, XA_STRING = 31;

void XCopy(Atom selection, char * text, int size) {
	XEvent event;
	Window owner;
	XSetSelectionOwner (display, selection, window, 0);
	if (XGetSelectionOwner (display, selection) != window) return;
	//while (1) {
		XNextEvent (display, &event);
		switch (event.type) {
			case SelectionRequest:
			if (event.xselectionrequest.selection != selection) break;
			XSelectionRequestEvent * xsr = &event.xselectionrequest;
			XSelectionEvent ev = {0};
			int R = 0;
			ev.type = SelectionNotify, ev.display = xsr->display, ev.requestor = xsr->requestor,
			ev.selection = xsr->selection, ev.time = xsr->time, ev.target = xsr->target, ev.property = xsr->property;
			if (ev.target == targets_atom) R = XChangeProperty (ev.display, ev.requestor, ev.property, XA_ATOM, 32,
					PropModeReplace, (unsigned char*)&UTF8, 1);
			else if (ev.target == XA_STRING || ev.target == text_atom)
				R = XChangeProperty(ev.display, ev.requestor, ev.property, XA_STRING, 8, PropModeReplace, text, size);
			else if (ev.target == UTF8)
				R = XChangeProperty(ev.display, ev.requestor, ev.property, UTF8, 8, PropModeReplace, text, size);
			else ev.property = None;
			if ((R & 2) == 0) XSendEvent (display, ev.requestor, 0, 0, (XEvent *)&ev);
			break;
			case SelectionClear:
			return;
		}
	//}
}
Atom XCopyInit() {
  display = XOpenDisplay(0);
  int N = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, N), 0, 0, 1, 1, 0,
  BlackPixel(display, N), WhitePixel(display, N));
  targets_atom = XInternAtom(display, "TARGETS", 0);
  text_atom = XInternAtom(display, "TEXT", 0);
  UTF8 = XInternAtom(display, "UTF8_STRING", 1);
  if (UTF8 == None) UTF8 = XA_STRING;
  return XInternAtom(display, "CLIPBOARD", 0);
}
