defmodule Ex11TestKeyboard do
  @moduledoc """
  This module wraps access to a NIF used to
  """

  @type key_sym :: integer

  alias Ex11TestKeyboard.Nif

  @doc """
  A list of keys (or chords). Returns :ok on success,
  {:error, :open_display_failed} if the X display could not be opened, or
  {:error, [invalid keys]} if invalid keys were provided.
  The list of valid keys can be determined by examining keysymdef.h in your
  libx11-dev package.

  Example:
  > press_keys(["NOT A KEY"])
  {:error, ["NOT A KEY"]}
  > press_keys(["1", "space", ["Shift_L", "plus"], "space", "1"])
  :ok
  (though "1 + 1" should show up in your terminal too, if everything has gone right!)
  """
  @spec press_keys([String.t() | [String.t()]]) :: :ok | {:error, atom | [String.t()]}
  def press_keys(chord_list) do
    symbol_chords = for chord <- chord_list, do: get_chord_symbols(chord)

    # Check if there were any invalid key strings
    errors =
      symbol_chords
      |> List.flatten()
      |> Enum.filter(&match?({:error, _}, &1))
      |> Enum.map(fn {:error, str} -> str end)

    if errors != [] do
      {:error, errors}
    else
      Nif.press_keys(symbol_chords)
    end
  end

  @doc """
  Set the display name to which the X11 keyboard events will be sent.
  The format is: [hostname]:display number[.screen number].
  Arguments are typically something like for a local display ":0" or ":0.0".
  Pass nil to unset any previously-set values. If this value is not set, the
  value of the DISPLAY environment variable is used when opening a display.
  """
  @spec set_display_name(String.t() | nil) :: :ok
  def set_display_name(nil), do: Nif.set_display_name(nil)

  def set_display_name(name) when is_binary(name) do
    name
    |> String.to_charlist()
    |> Nif.set_display_name()
  end

  @spec get_chord_symbols(String.t() | [String.t()]) :: [key_sym | {:error, String.t()}]
  defp get_chord_symbols(key) when is_binary(key) do
    get_chord_symbols([key])
  end

  defp get_chord_symbols(chord) when is_list(chord) do
    for key_string <- chord do
      erl_str = String.to_charlist(key_string)

      case Nif.x_string_to_keysym(erl_str) do
        sym when is_integer(sym) ->
          sym

        _ ->
          {:error, key_string}
      end
    end
  end
end
