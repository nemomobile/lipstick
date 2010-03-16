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

# The directory where the  benchmarking commands and results are collected
$dir = "/tmp/duihome_benchmarks"

class BenchmarkTestCase < Dui::TestCase

    def initialize *args

        # Default number of times a test should be repeated
        @benchmark_runs = 10

        # Initialize the parent class
        super *args

        # Scan all public methods of this _instance_ and for every test
        # method (name starting with 'test') replace it with such that
        # will execute the test @benchmark_runs number of times
        class << self
            methods = self.public_instance_methods(true)
            tests = methods.delete_if { |name| name !~ /^test./ }
            tests.each do
                |name|
                real = "real_#{name}"

                alias_method real, name
                define_method name do
                    log_command "testcase '#{name}' started"
                    @benchmark_runs.times { |n|
                        log_command "testcase '#{name}' run #{n}"
                        self.__send__(real) 
                    }
                    log_command "testcase '#{name}' done"
                end
            end
        end
    end

    def log_command msg
        time = DateTime.now
        writestring = "#{time}  #{msg}\n"

        Dir.mkdir($dir) unless File.directory?($dir)
        File.open("#{$dir}/benchmark_commands.txt",'a') { |f| f.write(writestring) }
    end
end

class TC_BM_Switcher < BenchmarkTestCase

    # method called before any test case
    def setup
        cmd = "context-provide"
        @cmd_in, @cmd_out, @cmd_err = Open3::popen3(cmd)

        # While compositor only does compositing for windows when going back
        # to homescreen (minimizing?), we need to manually start each
        # application and click the "back" button to get the thumbnails
        3.times { |n|
            @sut.run(:name => "/usr/lib/duifw-home-tests/ta_activeapp",
                     :arguments => "-no-refresh,-id,#{n}").
                 DuiButton(:name => "HomeButton").tap
        }

        @app = @sut.application(:name => 'duihome')
        @app.call_method('startBenchmarking()')
    end

    # method called after any test case for cleanup purposes
    def teardown
        @app.call_method('stopBenchmarking()')

        # While we start applications throught Matti machinery, we might
        # as well kill them throught Matti machinery
        @sut.kill_started_processes()

        @cmd_in.close
        @cmd_out.close
        @cmd_err.close
    end

    def test_fps_when_switcher_panned
        # Pan the switcher
        @app.Switcher.flick(:Left)
        sleep 1
    end

    def test_fps_when_switcher_rotates
        # initialize
        @cmd_in.puts "add string Screen.TopEdge top"

        # change orientation to portrait
        log_command "Screen.TopEdge=right"
        @cmd_in.puts "Screen.TopEdge=right"
        sleep 5

        # change orientation to landscape
        log_command "Screen.TopEdge=top"
        @cmd_in.puts "Screen.TopEdge=top"
        sleep 5

        # reset
        @cmd_in.puts "Screen.TopEdge=top"
    end
end
