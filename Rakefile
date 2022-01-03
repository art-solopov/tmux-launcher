# frozen_string_literal: true

MRUBY_URL = 'https://github.com/mruby/mruby/archive/3.0.0.zip'
BUILD_CONFIG = 'tml'
BUILD_CONFIG_PATH = "mruby/build_config/#{BUILD_CONFIG}.rb"
MRBC_PATH = 'mruby/bin/mrbc'

task default: ['mruby']

file 'mruby.zip' do
  sh "wget -c -O mruby.zip #{MRUBY_URL}"
end

directory 'mruby' => 'mruby.zip' do
  sh 'unzip mruby.zip'
  dirname = Dir['mruby*'].find { File.directory?(_1) }
  FileUtils.mv dirname, 'mruby'
end

file BUILD_CONFIG_PATH => ['mruby', 'build_config.rb'] do |t|
  FileUtils.cp 'build_config.rb', t.name
end

file MRBC_PATH => ['mruby', BUILD_CONFIG_PATH] do
  ENV['MRUBY_CONFIG'] = BUILD_CONFIG
  Dir.chdir 'mruby'
  sh 'rake'
end

desc 'Build mruby with our build config'
task mruby_build: MRBC_PATH

file 'mainrb.c' => ['main.rb', MRBC_PATH] do |t|
  sh "#{MRBC_PATH} -Bmainrb -o #{t.name} main.rb"
end

desc 'Compile script'
task compile_script: 'mainrb.c'
