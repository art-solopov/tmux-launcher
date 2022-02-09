# frozen_string_literal: true

MRUBY_URL = 'https://github.com/mruby/mruby/archive/3.0.0.zip'
PKG_PATH = 'pkg'
BUILD_CONFIG = 'tml'
MRUBY_PATH = "#{PKG_PATH}/mruby"
BUILD_CONFIG_PATH = "#{MRUBY_PATH}/build_config/#{BUILD_CONFIG}.rb"
MRBC_PATH = "#{MRUBY_PATH}/bin/mrbc"
MRUBY_CONFIG_PATH = "#{MRUBY_PATH}/bin/mruby-config"
LIBYAML_PATH = "#{MRUBY_PATH}/build/host/mrbgems/mruby-yaml/libyaml/build/lib/libyaml.a"

task default: :compile_script

directory "build"
directory "pkg"

file MRUBY_PATH => "pkg" do
  sh "wget -c -O #{PKG_PATH}/mruby.zip #{MRUBY_URL}"
  sh "unzip #{PKG_PATH}/mruby.zip -d #{PKG_PATH}"
  dirname = Dir["#{PKG_PATH}/mruby-*"].first
  FileUtils.mv dirname, "#{PKG_PATH}/mruby"
  FileUtils.rm("#{PKG_PATH}/mruby.zip")
end

file BUILD_CONFIG_PATH => [MRUBY_PATH, 'build_config.rb'] do |t|
  FileUtils.cp 'build_config.rb', t.name
end

file "#{MRUBY_PATH}/bin" => [MRUBY_PATH, BUILD_CONFIG_PATH]
file "#{MRUBY_PATH}/build" => "#{MRUBY_PATH}/bin"

file MRBC_PATH => [MRUBY_PATH, BUILD_CONFIG_PATH] + Dir.glob('ext/**/*.rb') + Dir.glob('ext/**/*.c') do
  ENV['MRUBY_CONFIG'] = BUILD_CONFIG
  Dir.chdir(MRUBY_PATH) { sh 'rake' }
end

desc 'Build mruby with our build config'
task mruby_build: MRBC_PATH

file "#{PKG_PATH}/mainrb.c" => ['main.rb', MRBC_PATH] do |t|
  sh "#{MRBC_PATH} -Bmainrb -o #{t.name} main.rb"
end

desc 'Compile script'
task compile_script: ["#{PKG_PATH}/mainrb.c", 'runner.c', 'build'] + Dir.glob("#{MRUBY_PATH}/build/host/**/*.a") do
  cflags = `#{MRUBY_CONFIG_PATH} --cflags`.strip
  ldflags = `#{MRUBY_CONFIG_PATH} --ldflags`.strip
  libs = `#{MRUBY_CONFIG_PATH} --libs`.strip
  # sh "gcc -std=c99 -I. -Imruby/include runner.c #{LIB_PATH} #{LIBYAML_PATH} -lm -o build/tmux-launch"
  sh "gcc #{cflags} -I. runner.c #{ldflags} #{libs} -o build/tmux-launch"
end
