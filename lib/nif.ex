defmodule Ex11TestKeyboard.Nif do
  @moduledoc false

  @on_load :load_nif

  @doc false
  def load_nif do
    filename = nif_filename(:os.type())

    nif_path =
      :ex11_test_keyboard
      |> :code.priv_dir()
      |> List.to_string()
      |> Path.join(filename)
      |> String.to_charlist()

    :erlang.load_nif(nif_path, 0)
  end

  @doc false
  @spec press_keys([[charlist]]) :: :ok | {:error, atom}
  def press_keys(_keys) do
    :erlang.nif_error("NIF press_keys not implemented.")
  end

  @doc false
  @spec set_display_name(charlist | nil) :: :ok
  def set_display_name(_name) do
    :erlang.nif_error("NIF set_display_name not implemented.")
  end

  @doc false
  @spec x_string_to_keysym(charlist) :: integer | nil
  def x_string_to_keysym(_xstring) do
    :erlang.nif_error("NIF x_string_to_keysym not implemented.")
  end

  defp nif_filename({:unix, :linux}) do
    proc =
      'uname -p'
      |> :os.cmd()
      |> List.to_string()
      |> String.trim()

    Path.join(["linux", proc, "ex11-kb-#{proc}"])
  end

  defp nif_filename({:unix, :darwin}) do
    Path.join("macos", "ex11-kb")
  end

  defp nif_filename(type) do
    raise "OS Type #{inspect(type)} not yet supported."
  end
end
