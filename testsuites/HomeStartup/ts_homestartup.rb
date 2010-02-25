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
require "runFixup.rb"
require 'dbus'

# Loads all the testcase files
# TODO: Move to lib/dui.rb ?
Dir.entries(CASES).each do |file|
  require File.join(CASES,file)  if file =~ /^tc_.*\.rb$/
end

# Custom testsuite that listens 'ready' D-Bus signal while tests are running
class HomeStartupRunner < Dui::Runner
    def setup(app, arguments, attach=false)
        # initialize home ready signal listening
        $isReady = false
        systemBus = DBus::SystemBus.instance
        obj = DBus::ProxyObject.new(systemBus, "com.nokia.duihome", "/com/nokia/duihome")
        obj_if = DBus::ProxyObjectInterface.new(obj, "com.nokia.duihome.readyNotifier" )
        obj_if.on_signal(systemBus, "ready"){
          $isReady = true
        }
        dbusMain = DBus::Main.new
        dbusMain << systemBus
        @dbusListenerThread = Thread.new do
             dbusMain.run
        end

        super(app, arguments, attach)
   end

    def teardown
        @dbusListenerThread.exit
        super
    end
end

suite = HomeStartupRunner.new
suite.start(nil,nil)
