# Eleutian Trainer Studio Application Opensource Edition, a VoIP client with Webkit and Flash support.
# Copyright (C) 2011, Eleutian Technology, LLC <http://www.eleutian.com>
# 
# Version: Apache License 2.0
# 
# The content of this file is licensed under the Apache License, Version 2.0. (the "License").
# You may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.apache.org/licenses/
# 
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
# 
# The Original Code is Eleutian Trainer Studio Application Opensource Edition.
# 
# The Initial Developer of the Original Code is
# Jadob Lewallen <jacob@eleutian.com>
# Portions created by the Initial Developer are Copyright (C)
# the Initial Developer. All Rights Reserved.
# 
# Contributor(s):
# 

require 'pathname'
require 'rbconfig'
$Win32 = Config::CONFIG['host_os'] =~ /mswin|mingw/

class Paths
  def self.QT
    Pathname.new("C:/QT/4.7.0/bin")
  end

  def self.jom
    Paths.QT.join("../../2010.05/bin/jom.exe") 
  end

  def self.freeswitch
    Pathname.new("../freeswitch")
  end

  def self.project
    Pathname.new(__FILE__).dirname
  end

  def self.project_file
    project.join("trainer_studio.pro")
  end

  def self.build
    Pathname.new("../trainer-studio-build-desktop")
  end

  def self.debug
    build.join("debug")
  end

  def self.qmake
    if $Win32
      Paths.QT.join("qmake.exe -r -spec win32-msvc2008") 
    else
      "qmake -r -spec macx-g++"
    end
  end

  def self.make
    if $Win32
      jom
    else
      "make"
    end
  end
end

def qmake(*args)
  sh "#{Paths.qmake} #{args}"
end

def make(*args)
  sh "#{Paths.make} #{args}"
end

task :platform_env do
end

task :platform_deps do
  if $Win32
    # I'd like this to be in the qmake file... -jlewallen
    libraries = []
    libraries << [ "QtCored4.dll", "QtWebKitd4.dll", "QtGuid4.dll", "QtNetworkd4.dll", "QtXmld4.dll", "phonond4.dll" ].map do |name|
      Paths.QT.join(name)
    end
    libraries << [ "FreeSwitch.dll", "libteletone.dll", "libapr.dll", "libaprutil.dll" ].map do |name|
      Paths.freeswitch.join("Win32/Debug").join(name)
    end
    libraries.flatten!
    cp libraries, Paths.debug, :verbose => true
  end
end

task :env => :platform_env do
  mkdir_p "../trainer-studio-build-desktop"
end

task :deps => [ :env, :platform_deps ] do
end

task :build => :env do
  Dir.chdir(Paths.build) do
    qmake Paths.project
    make
  end
end

task :clean => :env do
  Dir.chdir(Paths.build) { make "clean" }
end

task :run => :build do
  if $Win32
    Dir.chdir(Paths.debug) { sh "TrainerStudio.exe" }
  else
    Dir.chdir(Paths.build) { sh "open TrainerStudio.app" }
  end
end

task :default => :build do
end
