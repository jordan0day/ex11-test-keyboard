defmodule Ex11TestKeyboard.MixProject do
  use Mix.Project

  def project do
    [
      app: :ex11_test_keyboard,
      version: "0.1.0",
      elixir: "~> 1.6",
      start_permanent: Mix.env() == :prod,
      deps: deps(),
      compilers: [:elixir_make | Mix.compilers()],
      make_targets: ["all"],
      make_clean: ["clean"],
      make_env: make_env()
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger]
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:elixir_make, "0.4.2"}
    ]
  end

  defp make_env do
    root_dir = to_string(:code.root_dir())
    erts_version = to_string(:erlang.system_info(:version))

    erts_include = Path.join([root_dir, "erts-#{erts_version}", "include"])
    ei_include = to_string(:code.lib_dir(:erl_interface, :include))
    ei_lib = to_string(:code.lib_dir(:erl_interface, :lib))

    %{
      "ERTS_INCLUDE_DIR" => System.get_env("ERTS_INCLUDE_DIR") || erts_include,
      "ERL_EI_INCLUDE_DIR" => System.get_env("ERL_EI_INCLUDE_DIR") || ei_include,
      "ERL_EI_LIBDIR" => System.get_env("ERL_EI_LIBDIR") || ei_lib
    }
  end
end
