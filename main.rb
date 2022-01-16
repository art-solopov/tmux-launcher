# frozen_string_literal: true

module TmuxLauncher
  def self.call(argv)
    Main.new(argv).call
  end

  class Main
    def initialize(argv)
      @argv = argv
    end

    def call
      p @argv
      p config_file
    end

    private

    def config_file
      # TODO: OS detection
      File.join(ENV['HOME'], 'tmux-launcher.yaml')
    end
  end
end
