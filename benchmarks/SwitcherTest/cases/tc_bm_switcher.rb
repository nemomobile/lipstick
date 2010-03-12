#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: testhome test suite functional cases for Awidget
#  * Objectives: test the functionality of Awidget
#
#  Author:: Sachin Kundu (mailto:ext-sachin.kundu@nokia.com)
#
#  This software, including documentation, is protected by copyright
#  controlled by Nokia Corporation. All rights are reserved. Copying,
#  including reproducing, storing, adapting or translating, any or all of
#  this material requires the prior written consent of Nokia Corporation.
#  This material also contains confidential information which may not be
#  disclosed to others without the prior written consent of Nokia.

require 'test/unit'
require 'matti'
include MattiVerify
require 'open3'

class BenchmarkTestCase < Dui::TestCase

    def initialize *args

        # Default number of times a test should be repeated
        @benchmark_runs = 10

        # Initialize the parent class
        super *args

        # Scan all public methods of this class and for every test method
        # (name starting with 'test') replace it with such that will execute
        # the test @benchmark_runs number of times
        methods = self.class.public_instance_methods(true)
        tests = methods.delete_if { |name| name !~ /^test./ }
        tests.each do
            |name|
            real = "real_#{name}"

            self.class.class_eval {
                alias_method real, name
                define_method name do
                    @benchmark_runs.times { |n|
                        self.__send__(real) 
                    }
                end
            }
        end
    end
end

class TC_BM_Switcher < BenchmarkTestCase

    # method called before any test case
    def setup
        @app = @sut.application(:name => 'duihome')
        # Execute the switcherfps.sh script to load and check the programs loaded.
        # While compositor only does compositing for windows when going back
        # to homescreen (minimizing?), we need to manually start each
        # application and click the "back" button to get the thumbnails
        #
        #system("../programloader.sh ../programlist.txt")
        
        3.times { |n|
            @sut.run(:name => "/usr/lib/duifw-home-tests/ta_activeapp",
                     :arguments => "-no-refresh,-id,#{n}").
                 DuiButton(:name => "HomeButton").tap
        }

        @sut.execute_shell_command("../toucher.sh &")
    end

    # method called after any test case for cleanup purposes
    def teardown
        # While we start applications throught Matti machinery, we might
        # as well kill them throught Matti machinery
        #
        #system("../killer.sh ../programlist.txt")
        
        @sut.kill_started_processes()

        @sut.execute_shell_command("pkill toucher.sh")
    end

def test_fps_when_switcher_panned
    # Pan the switcher
    @app.Switcher.flick(:Left)
    write_benchmark_command_to_file("test_fps_when_switcher_panned","@app.Switcher.flick(:Left)")
    sleep 2
end

def test_fps_when_switcher_rotates
    # use context provider to change orientation
    cmd = "context-provide"
    cmd_in, cmd_out, cmd_err = Open3::popen3(cmd)

    write_benchmark_command_to_file("test_fps_when_switcher_rotates","cmd_in.puts add string Screen.TopEdge top")
    cmd_in.puts "add string Screen.TopEdge top"

    # change orientation to portrait
    write_benchmark_command_to_file("test_fps_when_switcher_rotates","cmd_in.puts Screen.TopEdge=right")
    cmd_in.puts "Screen.TopEdge=right"
    sleep 5
    
    # change orientation to landscape
    write_benchmark_command_to_file("test_fps_when_switcher_rotates","cmd_in.puts Screen.TopEdge=top")
    cmd_in.puts "Screen.TopEdge=top"
    sleep 5

    # reset
    cmd_in.puts "Screen.TopEdge=top"

    cmd_in.close
    cmd_out.close
    cmd_err.close

end

private 
def write_benchmark_command_to_file(testcase,command)
    #Put the current command and the time it was issued
    time = DateTime.now
    writestring = "#{testcase}  #{command}  #{time}\n"
    File.open("/tmp/duihome_benchmarks/benchmark_commands.txt",'a') {|f| f.write(writestring)}
end
end
