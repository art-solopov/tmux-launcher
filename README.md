# tmux-launcher
Tmux launcher app written in mruby (yes, it needs a much better name).

# Usage

In order to use the app, you need a Yaml file named `tmux-launcher.yaml` in
your home directory. The file should look like this:

```yaml
session_name:
  path: session_path # Should be a full path
  options:
    key_1: value_1
    key_2: value_2
    # ...
```

Then, when you run `tmux-launcher session_name`, it'll create a new tmux session called
`session_name` in the path `session_path` with options as declared in the config file.

# Support

It should run on any Unix-like system that supports mruby, understands `fork`, `execvp`, `execlp` and `pipe`.

# Building

You'll need gcc and ruby for building. Clone the repository and run `rake`. It should install mruby for you,
link the build configuration and build everything.

# Contributing

Contributions welcome! Just don't be an ass to other contributors.
