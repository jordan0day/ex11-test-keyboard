#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <string.h>
#include "ex11_nif.h"

static int load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info) {
  nif_context *ctx = (nif_context *)enif_alloc(sizeof(nif_context));
  if (ctx == NULL) {
    return 1;
  }

  ctx->display_name = NULL;

  *priv_data = (void *)ctx;

  ATOM_ERROR = enif_make_atom(env, "error");
  ATOM_OK = enif_make_atom(env, "ok");
  ATOM_NIL = enif_make_atom(env, "nil");
  ATOM_OPEN_DISPLAY_FAILED = enif_make_atom(env, "open_display_failed");
  return 0;
}

static void unload(ErlNifEnv* env, void* priv_data) {
  enif_free(priv_data);
  return;
}

static ERL_NIF_TERM ex11_nif_press_keys(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  // Validate the argument
  if(!enif_is_list(env, argv[0])) {
    ERL_NIF_TERM badarg = enif_make_badarg(env);
    return badarg;
  };

  unsigned int num_chords;
  if (!enif_get_list_length(env, argv[0], &num_chords)) {
    ERL_NIF_TERM badarg = enif_make_badarg(env);
    return badarg;
  }

  if (num_chords < 1) {
    ERL_NIF_TERM badarg = enif_make_badarg(env);
    return badarg;
  }

  // Open the display
  nif_context *ctx = (nif_context *)enif_priv_data(env);
  Display *display = XOpenDisplay(ctx->display_name);
  if (display == NULL) {
    return enif_make_tuple2(env, ATOM_ERROR, ATOM_OPEN_DISPLAY_FAILED);
  }

  ERL_NIF_TERM head;
  ERL_NIF_TERM rest = argv[0];
  while (enif_get_list_cell(env, rest, &head, &rest)) {
    press_key_chord(env, display, head);
  }

  XCloseDisplay(display);

  return ATOM_OK;
}

static ERL_NIF_TERM ex11_nif_set_display_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  // Since the display name includes a hostname portion, let's give ourselves a
  // bit of space, just in case.
  char display_name[2048];
  nif_context *ctx = (nif_context *)enif_priv_data(env);

  // Unset the existing name, if any
  if (ctx->display_name != NULL) {
    free(ctx->display_name);
    ctx->display_name = NULL;
  }

  int len = enif_get_string(env, argv[0], display_name, 2048, ERL_NIF_LATIN1);

  if (len != 0) {
    ctx->display_name = malloc(len * sizeof(char));
    memcpy(ctx->display_name, display_name, len);
  }

  return ATOM_OK;
}

static ERL_NIF_TERM ex11_nif_x_string_to_keysym(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  char xstring[128];

  if (enif_get_string(env, argv[0], xstring, 128, ERL_NIF_LATIN1)) {
    KeySym keysym = XStringToKeysym(xstring);
    if (keysym == 0) {
      return ATOM_NIL;
    } else {
      return enif_make_int(env, keysym);
    }
  }

  return ATOM_NIL;
}

static void press_key_chord(ErlNifEnv *env, Display *display, ERL_NIF_TERM chord) {
  unsigned int chord_length;
  if (!enif_get_list_length(env, chord, &chord_length)) {
    return;
  }

  int keys[chord_length];

  int idx = 0;
  int val;
  ERL_NIF_TERM head;
  ERL_NIF_TERM rest = chord;

  // Build our list of keys to press in the chord.
  while (enif_get_list_cell(env, rest, &head, &rest)) {
    enif_get_int(env, head, &val);

    keys[idx] = val;
    idx++;
  }

  // Perform key down events
  for (idx = 0; idx < chord_length; idx++) {
    KeyCode kc = XKeysymToKeycode(display, keys[idx]);
    XTestFakeKeyEvent(display, kc, True, 0);
  }

  // Perform key up events in reverse order
  for (idx = chord_length - 1; idx >= 0; idx--) {

    KeyCode kc = XKeysymToKeycode(display, keys[idx]);
    XTestFakeKeyEvent(display, kc, False, 0);
  }

  // Now flush our inputs
  XFlush(display);
}

static ErlNifFunc nif_funcs[] = {
  // {erl_function_name (string), erl_function_arity (int), c_function (func name), flags}
  {"press_keys", 1, ex11_nif_press_keys, ERL_NIF_DIRTY_JOB_CPU_BOUND},
  {"set_display_name", 1, ex11_nif_set_display_name, ERL_NIF_DIRTY_JOB_CPU_BOUND},
  {"x_string_to_keysym", 1, ex11_nif_x_string_to_keysym, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(Elixir.Ex11TestKeyboard.Nif, nif_funcs, load, NULL, NULL, unload)
