#!/usr/bin/ruby

# Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# Contact: Nokia Corporation (directui@nokia.com)
#
# This file is part of duihome.
#
# If you have questions regarding the use of this file, please contact
# Nokia at directui@nokia.com.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation
# and appearing in the file LICENSE.LGPL included in the packaging
# of this file.

$_MATTI_DISABLE_STATS_REPORTING = true
PATH=File.dirname(__FILE__)
CASES=File.join(PATH,"cases")
$LOAD_PATH << File.join(PATH,"..","lib")
require "dui.rb"
require "dbus"

# Loads all the testcase files
# TODO: Move to lib/dui.rb ?
Dir.entries(CASES).each do |file|
  require File.join(CASES,file)  if file =~ /^tc_.*\.rb$/
end

# Class to listen to and react to incoming D-Bus calls
class TestDBusService < DBus::Object
    def initialize(path)
        super(path)
        @messages = Array.new
    end

    dbus_interface "org.maemo.dui.test.NotificationSystemTesterInterface" do
        dbus_method :sendMessage, "in message:s" do |message|
            puts "sendMessage method called with #{message}"
            @messages[@messages.size] = message
        end
    end

    attr_accessor :messages
end

# Custom testsuite that has D-Bus listening up while tests are running
class NotificationSystemRunner < Dui::Runner
    def setup(app, arguments, attach=false)
        super(app, arguments, attach)

        $notificationtoolapp = "/usr/bin/duinotificationtool"
        verify_equal(true, nil, "#{$notificationtoolapp} not found or not executable! Test environment not properly set up.") {
            File.executable?($notificationtoolapp)
        }

        sessionBus = DBus::SessionBus.instance
        dbusService = sessionBus.request_service("org.maemo.dui.test.NotificationSystemTester")
        $testDBusObject = TestDBusService.new("/")
        dbusService.export($testDBusObject)
        dbusMain = DBus::Main.new
        dbusMain << sessionBus

        @dbusListenerThread = Thread.new do
            dbusMain.run
        end
    end

    def teardown
        @dbusListenerThread.exit

          begin
            home = MATTI.sut(:Id => 'sut_qt_maemo').application(:name => 'duihome')
            if home != nil
              pid = home.attribute('processId')

              home.close

              if File.exists?('/proc' + pid + '/status')
                Process.kill(:SIGKILL, pid.to_i)
              end
            end
          rescue
          end
        super
    end
end

# Inject test dbus service getter into notification system testcase class
class TC_NotificationSystem
    def testDBusService
        $testDBusObject
    end
end

# Start testsuite
suite = NotificationSystemRunner.new
suite.start('duihome','-software')

