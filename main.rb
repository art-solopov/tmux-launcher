# frozen_string_literal: true

class Main
  class ConfigLoadError < StandardError; end
  class SessionConfigMissingError < StandardError; end
  class SessionConfigInvalidError < StandardError; end

  BIN_NAME = 'tmux-launch'

  HELP = <<~HELP
    Usage:
      #{BIN_NAME} [name] - launches tmux session named [name].
      #{BIN_NAME} - launches tmux session based on current working directory
      #{BIN_NAME} -h - shows this help
  HELP

  def self.call
    new(ARGV).call
  end

  def initialize(argv)
    @argv = argv
  end

  def call
    return print_help if @argv[1] == '-h'

    @session = @argv.size > 1 ? @argv[1] : session_name_from_cwd
    launch_session
    set_session_options
    0
  rescue ConfigLoadError
    puts "File #{config_file} not found or not valid YAML"
    -1
  rescue SessionConfigMissingError
    puts "Session #{@session} not described in the config file"
    -2
  rescue SessionConfigInvalidError
    puts "Session #{@session} improperly configured"
    -3
  rescue TmuxLauncher::TmuxError => e
    puts e.message
    -32
  end

  private

  def print_help
    puts HELP
    0
  end

  def launch_session
    TmuxLauncher.new_session(@session, session_config.fetch('path'))
  rescue KeyError
    raise SessionConfigInvalidError
  end

  def set_session_options
    options = session_config['options']
    return if options.to_a.empty?

    options.each do |name, value|
      TmuxLauncher.set_option @session, name, value
    end
  end

  def session_name_from_cwd
    File.basename(CWD)
  end

  def config_file
    # TODO: OS detection
    File.join(ENV['HOME'], 'tmux-launcher.yaml')
  end

  def load_config
    YAML.load(File.read(config_file))
  rescue RuntimeError
    raise ConfigLoadError
  end

  def session_config
    @session_config ||= load_config.fetch(@session)
  rescue KeyError
    raise SessionConfigMissingError
  end
end
