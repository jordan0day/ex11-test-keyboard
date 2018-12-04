#ifndef EX11_NIF_H
#define EX11_NIF_H

#include <erl_nif.h>

typedef struct {
  char *display_name;
} nif_context;

// Atoms
static ERL_NIF_TERM ATOM_ERROR;
static ERL_NIF_TERM ATOM_OK;
static ERL_NIF_TERM ATOM_NIL;
static ERL_NIF_TERM ATOM_OPEN_DISPLAY_FAILED;

// One argument -- a list of embedded lists. The outer list represents a series
// of key presses. For example, [["a"], ["B"], ["c"]] would represent "Press A".
// "Release a". "Press B". "Release B". "Press c". "Release c".
// The inner list represents simultaneous key presses, like ctrl-x. So
// [["Control_L", "x"]] would represent "Press left control". "Press x".
// "Release x". "Release left control".
//
// So to exit emacs, you're send an argument of
// [["Control_L", "x"], ["Control_L", "c"]].
// To exit vim... well, who knows?
//
// The actual values contained are not strings, but rather KeySym values. These
// are obtained by using the XStringToKeySym/1 function.
static ERL_NIF_TERM ex11_nif_press_keys(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

// One argument -- an erlang string representing a display name, or nil. This
// display name is used in calls made to XOpenDisplay/1 during
// ex11_nif_press_keys/3 calls.
// A display name takes the format [hostname]:number[.screen_number]
// For typical use, a string like ":0" or ":0.1" generally works.
// If this is not set, whatever value (if any) is present in the DISPLAY
// environment variable will be used in calls to XOpenDisplay/1.
// Returns :ok
static ERL_NIF_TERM ex11_nif_set_display_name(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

static ERL_NIF_TERM ex11_nif_x_string_to_keysym(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

static void press_key_chord(ErlNifEnv *env, Display *display, ERL_NIF_TERM chord_list);

#endif
