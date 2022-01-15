# frozen_string_literal: true

module TmuxLauncher
  Result = Struct.new(:pid, :status, :err_io)

  class TmuxError < StandardError
  end

  class << self
    def new_session(name, path)
      Dir.chdir(path) do
        run_tmux_command %W[new -d -s #{name}]
      end
    end

    def set_option(session, name, value)
      run_tmux_command %W[set-option -t #{session} #{name} #{value}]
    end

    private

    def run_tmux_command(args)
      cmd_status = _run_tmux_command(*args)
      raise TmuxError, "Tmux error: #{cmd_status.err_io.read}" unless cmd_status.status.zero?
    end
  end
end
