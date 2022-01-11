# frozen_string_literal: true

module TmuxLauncher
  Result = Struct.new(:pid, :status, :err_io)

  class << self
    private

    def load_config
      YAML.load File.read(config_path)
    end

    def config_path
      File.join(ENV['HOME'], 'tmux-launcher.yaml')
    end
  end
end
