#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: HomeSwitcher test suite functional cases for duihome
#  * Objectives: test the functionality of duihome switcher
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
#SetLogFiles::            /logs/testability/tc_homeswitcher.log
#SetFeature::             DUI-1988 in ScrumWorks Pro
#SetTimeout::             500

# These tests verify duihome switcher functionality
class TC_homeswitcher < Dui::TestCase

    def setup
	@home  = @sut.application(:name => 'duihome')
    end

    def teardown
	close_all_ta
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Check that the switcher is being displayed
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Check that the switcher is visible on the desktop
    #  * Assert that the switcher is empty (contains no buttons)
    #* Post-conditions
    #  * None
    def test_01_switcher_is_shown
	verify(0, "Switcher not found") {
	    @home.Switcher
	}

	verify_equal(0, 0, "Switcher should contain no buttons") {
	    count_switcher_buttons
	}

    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #
    # Check the switcher functionality
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Check that the switcher buttons are added as applications are started
    #  * Assert that only one button is added per one application/window
    #  * Check that tapping a switcher button brings the corresponding application to front
    #  * Check that tapping a close button within the switcher button closes the corresponding application
    #  * Assert that only one button is removed when a window closes
    #  * Assert that the correct switcher button is removed
    #* Post-conditions
    #  * None
    def test_02_switcher_window_opening_switching_and_closing
	n = count_switcher_buttons

	# Verify that when an application is started, an appropriate button
	# (and only one such) is added to the switcher
	5.times { |m|
	    name = 'Ta_HomeSwitcher-' + m.to_s

	    @sut.run(:name => '/usr/lib/duifw-home-tests/ta_homeswitcher',
		     :arguments => '-software,-id,' + m.to_s)

	    verify(30, "Switcher button not found") {
		@home.SwitcherButton(:text => name)
	    }

	    verify_equal(n + m + 1, 0, "Buttons number mismatch") {
		count_switcher_buttons
	    }
	}

	# Verify that if a switcher button is tapped, the correct application
	# is brought to front
	[2, 4, 3, 0, 1].each { |m|
	    name = 'Ta_HomeSwitcher-' + m.to_s

	    @home.SwitcherButton(:text => name).tap
	    #@sut.application(:name => 'duihome').SwitcherButton(:text => name).tap
	    verify_equal(name, 10) {
		@sut.application.DuiApplicationWindow.attribute('windowTitle')
	    }
	}

	# Verify that if a close button on an appropriate switcher button
	# is tapped, the application closes and the corrent switcher button
	# is removed
	n = count_switcher_buttons

	[0, 2, 3, 1, 4].each_with_index { |m, i|
	    name = 'Ta_HomeSwitcher-' + m.to_s

	    # "duihome/themes/style/default.css" defines switcher button size
	    # as 195x108 in landsape mode. Consider a close button of size
	    # 24x24 in the top-right corner. The following tap will get it
	    # right in the middle.
	    @home.SwitcherButton(:text => name).tap_object(195-12, 0+12)

	    verify_not(10, "Switcher button found") {
		@home.SwitcherButton(:text => name)
	    }

	    verify_equal(n - i - 1, 0, "Buttons number mismatch") {
		count_switcher_buttons
	    }
	}
    end

private
    # An auxilary function that returns the current count of SwitcherButton
    # objects in the DuiHome object being tested
    def count_switcher_buttons
	count = 0
	begin
	    button = @home.SwitcherButton(:__index => count)
	    count = count + 1
	rescue NoMethodError, MobyBase::TestObjectNotFoundError
	    button = nil
	end while button

	return count
    end

    # An auxilary function that closes all test helpers
    def close_all_ta
	begin
	    app = @sut.appication(:name => 'ta_homeswitcher', :__index => 0)
	    app.close
	rescue NoMethodError
	    app = nil
	end while app
    end
end
