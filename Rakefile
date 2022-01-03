# frozen_string_literal: true

MRUBY_URL = 'https://github.com/mruby/mruby/archive/3.0.0.zip'

task default: ['mruby']

file 'mruby.zip' do
  sh "wget -c -O mruby.zip #{MRUBY_URL}"
end

directory 'mruby' => 'mruby.zip' do
  sh 'unzip mruby.zip'
  dirname = Dir['mruby*'].find { File.directory?(_1) }
  FileUtils.mv dirname, 'mruby'
end

file 'mruby/build_config/tml.rb' => ['mruby', 'build_config.rb'] do
  FileUtils.cp 'build_config.rb', 'mruby/build_config/tml.rb'
end

desc 'Build mruby with our build config'
task build_mruby: ['mruby', 'mruby/build_config/tml.rb'] do
  ENV['MRUBY_CONFIG'] = 'tml'
  Dir.chdir 'mruby'
  sh 'rake'
end

file 'mruby/bin/mrbc' => :build_mruby
