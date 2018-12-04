# Ex11TestKeyboard

A NIF for sending keypress events to X11 using the XTestFakeKeyEvent extension.

The manpage for XTestFakeKeyEvent states:
> This extension is a minimal set of client and server extensions required to completely test the X11 server with no user intervention. This extension is not intended to support general journaling and playback of user actions.

So, you know, maybe don't use this for anything important?

Ostensibly this could (should?) work on MacOS (via XQuartz) and Windows (via ???), but it has only been tested on Linux and Macos/xquartz.

## Requirements
You'll need libx11-dev and libxtst-dev to build this NIF.

## Important Notes
The argument to the `press_keys/1` function is a list of keys and lists, where and inner list constitutes a "chord" -- that is, simultaneous keypresses. The outer list is a series of chords, performed sequentially.

_"An example would be nice"_

To type a nice series of keypresses, like "Hello, World":

```
Ex11TestKeyboard.press_keys(["H", "e", "l", "l", "o", "comma", "space", "W", "o", "r", "l", "d"])
```

To quit emacs, which is two seqeuences of simultaneous key presses (Ctrl-x, Ctrl-c), you would issue the following command:

```
Ex11TestKeyboard.press_keys([["Control_L", "x"], ["Control_L", "c"]])
```

*Key names*
They key names you provide should be parseable by the XStringToKeysym function. Check `keysymdef.h` in your libx11-dev install to see a list of key names, and drop the "XK_" bit from the front. At least, that's worked alright in my testing.

*Other notes*
One sort-of-surprising thing (for me, at least!) was that while there are key names defined in `keysymdef.h` like `XK_exclam` (exclamation mark) and `XK_question` (question mark), passing those did not result in ! or ? actually being typed. On my ANSI US-style keyboard, both of those symbols are "shifted" (with the 1 and / characters, respectively), that is, you have to hold a shift key to actually get a ! or a ?. So, to actually write "Hello, World!", it's necessary to send:
`["H", "e", "l", "l", "o", "comma", "space", "W", "o", "r", "l", "d", ["Shift_L", "exclam"]]`,

When you get tired of playing around with this in your IEx session, just run:

```
Ex11TestKeyboard.press_keys([["Control_L", "c"], ["Control_L", "c"]])
```
😀
