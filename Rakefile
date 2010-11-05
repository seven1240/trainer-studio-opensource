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
