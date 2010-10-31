require 'pathname'

namespace :win32 do
end

namespace :macx do
end

class Paths
  def self.QT
    Pathname.new("C:/QT/4.7.0/bin")
  end

  def self.freeswitch
    Pathname.new("../freeswitch")
  end

  def self.build
    Pathname.new("../trainer-studio-build-desktop/debug")
  end
end

def qmake(*args)
  path = Paths.QT.join("qmake.exe") 
  sh "#{path} #{args}"
end

def jom(*args)
  path = Paths.QT.join("../../2010.05/bin/jom.exe") 
  sh "#{path} #{args}"
end

task :default => :build do
end

task :env do
  mkdir_p "../trainer-studio-build-desktop"
end

task :deps => :env do
  libraries = []
  libraries << [ "QtCored4.dll", "QtWebKitd4.dll", "QtGuid4.dll", "QtNetworkd4.dll", "QtXmld4.dll", "phonond4.dll" ].map do |name|
    Paths.QT.join(name)
  end
  libraries << [ "FreeSwitch.dll", "libteletone.dll", "libapr.dll", "libaprutil.dll" ].map do |name|
    Paths.freeswitch.join("Win32/Debug").join(name)
  end
  libraries.flatten!
  cp libraries, Paths.build, :verbose => true
end

task :build => :env do
  Dir.chdir "../trainer-studio-build-desktop" do
    qmake "../trainer-studio/trainer_studio.pro -r -spec win32-msvc2008"
    jom
  end
end

task :clean => :env do
  Dir.chdir "../trainer-studio-build-desktop" do
    jom "clean"
  end
end
