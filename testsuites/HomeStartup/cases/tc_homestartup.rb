#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: testhome test suite functional cases for Awidget
#  * Objectives: test the functionality of Awidget
#
#  Author:: Artem Egorkine <ext-artem.egorkine@nokia.com>
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

#SuiteDomain::            ApplicationFramework
#SuiteTimeout::           500
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          true
#SetLogFiles::            /logs/testability/tc_homestartup.log
#SetFeature::             -
#SetTimeout::             500
#
# These tests verify duihome launch and startup.
class TC_homestartup < Dui::TestCase

    # method called before any test case
    def setup
       # Make sure diuhome is not running
       close_duihome
    end

    # method called after any test case for cleanup purposes
    def teardown
    end

    #SubFeature::   -
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that 'ready' dbus signal has been emitted when duihome is
    # started
    #
    #* Pre-conditions
    #  * system dbus-daemon running
    #* Test steps
    #  * start duihome with upstart
    #  * verify that ready signal is emitted
    #  * verify that duihome process is stopped
    #  * kill duihome
    #* Post-conditions
    #  * None
    #* References
    #  * Bug 147480
    #

    def test_01_ready_signal_with_upstart

        $isReady = false

        pid = fork { exec("duihome --upstart") }
        verify_true(10) { $isReady }

        Process.waitpid(pid, Process::WUNTRACED)
        verify_true { $?.stopped? }

        Process.kill(:SIGKILL, pid)
        Process.wait(pid)
    end

    #SubFeature::   -
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that 'ready' dbus signal has been emitted when duihome is
    # started
    #
    #* Pre-conditions
    #  * system dbus-daemon running
    #* Test steps
    #  * run duihome (without upstart)
    #  * verify that ready signal is emitted
    #  * verify that process does not stop
    #  * kill duihome
    #* Post-conditions
    #  * None
    #* References
    #  * Bug 147480
    #
    def test_02_ready_signal

        $isReady = false

        pid = fork { exec("duihome") }
        verify_true(10) { $isReady }

        # wait to be sure that duihome is up&running
        sleep 30

        Process.waitpid(pid, Process::WNOHANG)
        # if process state has not been changed, $? should be nil
        verify_true { $? == nil }
        Process.kill(:SIGKILL, pid)
        Process.wait(pid)
    end

private

    def close_duihome
        home = @sut.application(:name => 'duihome')
        if home != nil
            pid = home.attribute('processId')

            home.close

            if File.exists?('/proc' + pid + '/status')
                Process.kill(:SIGKILL, pid.to_i)
            end
        end
    end
end
