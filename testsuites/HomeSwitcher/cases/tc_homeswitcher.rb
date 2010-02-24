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
#SetFeature::             DUI-1952 in ScrumWorks Pro
#SetTimeout::             500

# These tests verify duihome switcher functionality
class TC_homeswitcher < Dui::TestCase

    @@screen_width = 0

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
        @@screen_width = @home.MainWindow.attribute('width').to_i
        verify(0, "Switcher not found") { @home.Switcher }
        verify_equal(0, 0, "Switcher should contain no buttons") { count_switcher_buttons }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #
    # Test switcher button launching and closing corresponding application
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Check that the switcher buttons are added as applications are started
    #  * Verify that only one button is added per one application/window
    #  * Check that tapping a switcher button brings the corresponding application to front
    #  * Check that tapping a close button within the switcher button closes the corresponding application
    #  * Verify that only one button is removed when a window closes
    #  * Verify that the correct switcher button is removed
    #* Post-conditions
    #  * None
    def test_02_open_and_close_applications_via_switcher_buttons

        # Verify that when an application is started, an appropriate button
        # (and only one such) is added to the switcher
        5.times { |m|
            name = 'Ta_HomeSwitcher-' + m.to_s

            @sut.run(:name => '/usr/lib/duifw-home-tests/ta_homeswitcher',
                            :arguments => '-software,-id,' + m.to_s)

            verify(10, "Switcher button not found") { @home.SwitcherButton(:text => name) }

            verify_equal(m + 1, 0, "Buttons number mismatch") { count_switcher_buttons }
        }

        # Verify that if a switcher button is tapped, the correct application
        # is brought to front
        tap_to_foreground_and_verify('Ta_HomeSwitcher-0')

        # two right-to-left gestures to bring Ta_HomeSwitcher-2 to the middle
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-0' ).gesture(:Left, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-1' ).gesture(:Left, 2, 400, :Left)
        tap_to_foreground_and_verify('Ta_HomeSwitcher-2')

        # one right-to-left gesture to bring Ta_HomeSwitcher-3 to the middle
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-2' ).gesture(:Left, 2, 400, :Left)
        tap_to_foreground_and_verify('Ta_HomeSwitcher-3')

        # two left-to-right gestures to bring Ta_HomeSwitcher-1 to the middle
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-3' ).gesture(:Right, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-2' ).gesture(:Right, 2, 400, :Left)
        tap_to_foreground_and_verify('Ta_HomeSwitcher-1')

        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-1' ).gesture(:Left, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-2' ).gesture(:Left, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-3' ).gesture(:Left, 2, 400, :Left)
        tap_to_foreground_and_verify('Ta_HomeSwitcher-4')

        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-4' ).gesture(:Right, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-3' ).gesture(:Right, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-2' ).gesture(:Right, 2, 400, :Left)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-1' ).gesture(:Right, 2, 400, :Left)

        # Verify that if a close button on an appropriate switcher button
        # is tapped, the application closes and the corrent switcher button
        # is removed
        buttons_num = count_switcher_buttons
        buttons_num.times { |m|
            name = 'Ta_HomeSwitcher-' + m.to_s
            # TODO
            # In current implementation of switcher the title of focused button is not always drawn
            # until switcher button is tapped once. This behaviour will propaply change and then
            # this tap can be removed.
            @home.SwitcherButton(:text => name).tap

            # Tap the close button of the switcher button
            button_width = @home.SwitcherButton(:text => name ).attribute('width')
            # Button dimensions are acquired from Matti Visualizer. Style changes may affect this test.
            @home.SwitcherButton(:text => name).tap_object(button_width.to_i-15, 0+15)

            verify_not { @home.SwitcherButton(:text => name) }
            verify_equal(buttons_num - 1 -m, 0, "Buttons number mismatch") { count_switcher_buttons }
        }
    end


    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #
    # Test switcher button gestures
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Launch test applications
    #  * Check that switcher button returns to it's original place when using gesture that's too short
    #  * Move switcher buttons with gestures and check that the buttons move to right places
    #* Post-conditions
    #  * None
    def test_03_panning_switcher_buttons_using_gestures

        #create 5 switcher buttons
        5.times { |m|
            name = 'Ta_HomeSwitcher-' + m.to_s
            @sut.run(:name => '/usr/lib/duifw-home-tests/ta_homeswitcher',
                            :arguments => '-software,-id,' + m.to_s)
        }

        # gesture that's too short, switcher buttons should return to original position
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-0' ).gesture(:Left, 2, 100, :Left)

        # This is the starting position: (assuming the buttons returned to their original positions after the short gesture above)
        # Ta_HomeSwitcher-0 in the middle, Ta_HomeSwitcher-1 on the right, others not visible.
        middle_button_x = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-0' ).attribute('x')
        right_button_x = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-1' ).attribute('x')

        # Verify switcher buttons positions
        # Buttons are assumed to be in certain position ranges regardles of possible changes in style
        verify_true { ((0..@@screen_width/2) === middle_button_x.to_i) && right_button_x.to_i > @@screen_width/2 }

        # 4 gestures brings us to the end of the list, 5th one shouldn't move buttons
        5.times { |m|
            button = 'Ta_HomeSwitcher-' + m.to_s
            @home.SwitcherButton( :text => button ).gesture(:Left, 2, 400, :Left)
        }

        # after gestures Ta_HomeSwitcher-3 should be on the left and Ta_HomeSwitcher-4 in the middle in the screen
        left_button_x = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-3' ).attribute('x')
        middle_button_x = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-4' ).attribute('x')
        verify_true{ left_button_x.to_i < 0 && (0..@@screen_width/2) === middle_button_x.to_i  }

        # 4 gestures brings us to the beginning of the list, 5th one shouldn't move buttons
        [4, 3, 2, 1, 0].each { |m|
            button = 'Ta_HomeSwitcher-' + m.to_s
            @home.SwitcherButton( :text => button ).gesture(:Right, 2, 400, :Left)
        }

        middle_button_x = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-0' ).attribute('x')
        right_button_x = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-1' ).attribute('x')
        verify_true { ((0..@@screen_width/2) === middle_button_x.to_i) && right_button_x.to_i > @@screen_width/2 }

    end

private
    # An auxiliary function that taps a switcher button and checks that
    # the right application is brought to foreground
    def tap_to_foreground_and_verify (name)
        @home.SwitcherButton( :text => name ).tap
        verify_equal(name, 10) { @sut.application.DuiApplicationWindow.attribute('windowTitle') }
    end

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
            app = @sut.application( :name => 'ta_homeswitcher' )
            app.close
        rescue MobyBase::TestObjectNotFoundError
            app = nil
        end while app
    end
end
