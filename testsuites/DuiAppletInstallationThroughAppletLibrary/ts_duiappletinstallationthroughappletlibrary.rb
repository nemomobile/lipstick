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

        @progressMutex = Mutex.new
        @progressContinuation = ConditionVariable.new
    end

    def informDownloadProgress(upTo = 100)
        DownloadProgress(0, upTo)
        sleep(2)
        DownloadProgress(upTo / 2, upTo)
        sleep(2)
        DownloadProgress(upTo, upTo)
    end

    def informInstallationProgress(package, upTo = 100)
        OperationProgress('Install', package, 0)
        @lastOperationProgress = 0
        sleep(2)
        OperationProgress('Install', package, upTo / 2)
        @lastOperationProgress = upTo / 2
        sleep(2)
        OperationProgress('Install', package, upTo)
        @lastOperationProgress = upTo
    end

    def informInstallationComplete(package)
        desktopentry = "[Desktop Entry]\n"\
                       "Type=DUIApplet\n"\
                       "Name=Weather\n"\
                       "Icon=W-Icon-weather\n"\
                       "Exec=duiappletrunner\n"\
                       "[DUI]\n"\
                       "X-DUIApplet-Applet=libweatherapplet.so\n"\
                       "X-DUIApplet-Identifier=weatherappletres\n";
        File.open("/tmp/fakeappletpackage.desktop", 'w') {|f| f.write(desktopentry) }

        OperationComplete('Install', package, '')
    end

    def informInstallationError(package, errStr)
        OperationComplete('Install', package, errStr)
    end

    def continueProgress
        @progressMutex.synchronize {
            @progressContinuation.signal
        }
    end

    dbus_interface 'com.nokia.package_manager' do
        dbus_method :InstallFile, 'in package:s' do |package|
            puts "InstallFile called with #{package}"
        end

        dbus_method :Install, 'in package:s' do |package|
            case package
            when 'fakeappletpackage-installable.deb'
                @progressMutex.synchronize {
                    informInstallationProgress(package)
                    @progressContinuation.wait(@progressMutex)
                    informInstallationComplete(package)
                }
            when 'fakeappletpackage-uninstallable.deb'
                @progressMutex.synchronize {
                    informInstallationProgress(package, 50)
                    @progressContinuation.wait(@progressMutex)
                    informInstallationError(package, 'com.nokia.package_manager.Error.OperationFailed')
                    # should return a d-bus error to caller but no way to do it in ruby-dbus at the moment
                }
            when 'fakeappletpackage-notdownloadable.deb'
                @progressMutex.synchronize {
                    informInstallationProgress(package, 50)
                    @progressContinuation.wait(@progressMutex)
                    informInstallationError(package, 'com.nokia.package_manager.Error.Internal')
                    # should return a d-bus error to caller but no way to do it in ruby-dbus at the moment
                }
            when 'fakeappletpackage-metadataonly.deb'
                @progressMutex.synchronize {
                    informInstallationProgress(package, 0)
                    @progressContinuation.wait(@progressMutex)
                    informInstallationError(package, 'com.nokia.package_manager.Error.Internal')
                    # should return a d-bus error to caller but no way to do it in ruby-dbus at the moment
                }
            when 'fakeappletpackage-nonexistent.deb'
                informInstallationError(package, 'com.nokia.package_manager.Error.NoCandidate')
                # should return a d-bus error to caller but no way to do it in ruby-dbus at the moment
            end
        end

        dbus_method :FetchPackageData, 'in package:s, in version:s, out package:a{sv}' do |package, version|
            if package != 'fakeappletpackage-nonexistent'
                [ { 'Maemo-Desktop-File' => ['s', '/tmp/fakeappletpackage.desktop'] } ]
            else
                []
            end
        end

        dbus_method :Operation, 'in package:s, out operation:s' do |package|
            case package
            when 'fakeappletpackage-installable.deb'
                [""] 
            when 'fakeappletpackage-inprogress.deb'
                ["Install"] 
            when 'fakeappletpackage-nonexistent.deb'
                [""] 
            end
        end

        dbus_signal :DownloadProgress, 'already:i, total:i'
        dbus_signal :OperationProgress, 'operation:s, pkg:s, percentage:i'
        dbus_signal :OperationComplete, 'operation:s, pkg:s, error:s'
    end

    attr_accessor :lastOperationProgress
end

# Custom testsuite that has D-Bus listening up while tests are running
class CustomRunner < Dui::Runner
    def setup(app, arguments, attach=false)
        super(app, arguments, attach)
        bus = DBus::SystemBus.instance
        dbusService = bus.request_service('com.nokia.package_manager')
        $testDBusObject = TestDBusService.new('/com/nokia/package_manager')
        dbusService.export($testDBusObject)
        dbusMain = DBus::Main.new
        dbusMain << bus

        @dbusListenerThread = Thread.new do
            dbusMain.run
        end
    end

    def teardown
        @dbusListenerThread.exit

        if File.exist?(ENV['HOME'] + "/.config/duihome/appletcanvas.data")
            File.delete(ENV['HOME'] + "/.config/duihome/appletcanvas.data")
        end

        super
    end
end

# Inject test dbus service getter into the testcase class
class TC_DuiAppletInstallationThroughAppletLibrary
    def testDBusService
        $testDBusObject
    end
end

# Delete any previous mashup canvas configuration
Dir.mkdir(ENV['HOME'] + "/.config") unless File.exist?(ENV['HOME'] + "/.config")
Dir.mkdir(ENV['HOME'] + "/.config/duihome") unless File.exist?(ENV['HOME'] + "/.config/duihome")
if File.exist?(ENV['HOME'] + "/.config/duihome/appletcanvas.data")
    File.delete(ENV['HOME'] + "/.config/duihome/appletcanvas.data")
end

suite = CustomRunner.new
# NOTICE: Dont pass -hardware or -software anymore unless you explicitly
# require either of them, hw acceleration will be automatically probed
# at runtime via dui.rb library
suite.start('duihome', nil, true)
