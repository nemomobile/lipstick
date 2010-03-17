#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: Switcher overview test suite functional cases for duihome
#  * Objectives: test the functionality of duihome switcher overview
#
#  Author:: Janne Sakko <janne.sakko@ixonos.com>
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

#SuiteDomain::            ApplicationFramework
#SuiteTimeout::           700
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          false
#SetLogFiles::            /logs/testability/tc_switcheroverview.log
#SetFeature::             DUI-2011 in ScrumWorks Pro
#SetTimeout::             700

# These tests verify duihome switcher functionality
class TC_switcheroverview < Dui::TestCase

    def setup
        @home  = @sut.application(:name => 'duihome')
        @@screen_width = @home.MainWindow.attribute('width').to_i
        @@landscape_mode = true
    end

    def teardown
        close_all_applications_by_name('ta_homeswitcher')
        close_all_applications_by_name('ta_callui')
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
    #  * Check that the switcher overview layout is visible on the desktop
    #  * Assert that the switcher overview is empty (contains no buttons)
    #* Post-conditions
    #  * Switcher overview is started
    def test_01_switcher_overview_is_shown
        verify { @home.Switcher( :name => 'OverviewSwitcher' ) }
        close_all_applications_by_name('ta_homeswitcher')
        close_all_applications_by_name('ta_callui')
        verify_equal(0) { count_switcher_buttons }
    end

    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Check that the buttons are correctly orederer tp rows and columns with different number of buttons
    # Check that correct order is maintained in portrait-to-lansdscape and in landscape-to-portrait rotation
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 2 buttons (total: 2 buttons)
    #  * Verify correct order (in rows and columns) and size of buttons
    #  * Add 1 button (total: 3 buttons)
    #  * Verify correct order (in rows and columns) and size of buttons
    #  * Add 1 button (total: 4 buttons)
    #  * Verify correct order (in rows and columns) and size of buttons
    #  * Add 2 buttons (total: 6 buttons)
    #  * Verify correct order (in rows and columns) and size of buttons
    #* Post-conditions
    #  * None
    def test_02_buttons_are_in_correct_columns_and_rows_in_overview_switcher_with_different_button_numbers
        create_switcher_application(0)
        create_switcher_application(1)
        # Now Switcher overview buttons in large size

        verify_equal(2) { count_switcher_buttons }

        # Expected: one row, two columns
        verify_true { button_x(0) < button_x(1) }
        verify_true { button_y(0) == button_y(1) }

        # Verify that buttons are same size
        verify_true { button_width(0) == button_width(1) }
        verify_true { button_height(0) == button_height(1) }

        # Store size for later comparison
        button_0_size_width_large = button_width(0)
        button_0_size_height_large = button_height(0)

        create_switcher_application(2)
        verify_equal(3) { count_switcher_buttons }
        # Now Switcher overview buttons in medium size

        # Expected: one row, three columns
        verify_true { button_x(0) < button_x(1) && button_x(1) < button_x(2) }
        verify_true { button_y(0) == button_y(1) && button_y(1) == button_y(2) }

        # Check that size of buttons is correctly updated
        verify_true { button_0_size_width_large > button_width(0) && button_0_size_height_large > button_height(0) }
        # Verify that buttons are same size
        count_switcher_buttons.times { |index|
            verify_true { button_width(0) == button_width(index) }
            verify_true { button_height(0) == button_height(index) }
        }

        create_switcher_application(3)
        create_switcher_application(4)
        verify_equal(5) { count_switcher_buttons }

        # Expected: two rows, three columns
        verify_true { button_x(0) < button_x(1) && button_x(1) < button_x(2) }
        verify_true { button_y(0) == button_y(1) && button_y(1) == button_y(2) }
        verify_true { button_y(3) > button_y(0) && button_y(4) > button_y(1) }
        verify_true { button_x(3) < button_x(4) }
        verify_true { button_x(0) == button_x(3) && button_x(1) == button_x(4) }

        create_switcher_application(5)

        verify_equal(6) {count_switcher_buttons }
        # Full page

        # Expected: two rows, three columns
        verify_true { button_x(0) < button_x(1) && button_x(1) < button_x(2) }
        verify_true { button_y(0) == button_y(1) && button_y(1) == button_y(2) }
        verify_true { button_x(3) < button_x(4) && button_x(4) < button_x(5) }
        verify_true { button_y(3) == button_y(4) && button_y(4) == button_y(5) }
        verify_true { button_x(0) == button_x(3) && button_x(1) == button_x(4) && button_x(2) == button_x(5) }
        verify_true { button_y(3) > button_y(0) }

        # Verify that buttons are same size
        count_switcher_buttons.times { |index|
            verify_true { button_width(0) == button_width(index) }
            verify_true { button_height(0) == button_height(index) }
        }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test launching random applications from switcher overview
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 6 buttons to switcher overview which is a full page.
    #  * Randomly launch applications by tapping switcher overview buttons
    #  * Verify that right applications are launched
    #* Post-conditions
    #  * None
    def start_03_application_from_switcher_overview
        added_buttons = 6

        buttons = Array.new
        added_buttons.times { |index|
            pid, name = create_switcher_application(index)
            buttons << name
        }

        6.times {
            tap_to_button_and_verify_application_launched(buttons[rand(6)])
        }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Check that a switcher overview button is removed from switcher overview
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 6 buttons.
    #  * Remove one button from the upper row
    #  * Verify that switcher buttons are compressed to fill left empty space
    #* Post-conditions
    #  * None
    def test_04_remove_button_from_switcher_overview

        buttons = Array.new
        6.times { |index|
            pid, name = create_switcher_application(index)
            buttons << pid
        }

        verify_equal(6) { count_switcher_buttons }

        # Buttons in small size
        verify_true { button_y(0) < button_y(3) }
        verify_true { button_x(0) == button_x(3) }

        # Verify buttons same size
        count_switcher_buttons.times { |index|
            verify_true { button_width(0) == button_width(index) }
            verify_true { button_height(0) == button_height(index) }
        }

        # Store button size
        button_0_size_width_small = button_width(0)
        button_0_size_height_small = button_height(0)

        ### Remove buttons
        [5,4,3,2].each { |index|
            remove_application_by_pid(buttons[index])
        }

        verify_equal(2) { count_switcher_buttons }

        # Buttons in large size
        verify_true { button_0_size_width_small < button_width(0) }
        verify_true { button_0_size_height_small < button_height(0) }

        # Verify buttons same size
        count_switcher_buttons.times { |index|
            verify_true { button_width(0) == button_width(index) }
            verify_true { button_height(0) == button_height(index) }
        }

        # Buttons in large size
        verify_true { button_x(0) < button_x(1) }
        verify_true { button_y(0) == button_y(1) }

        verify_true { button_width(0) == button_width(1) }
        verify_true { button_height(0) == button_height(1) }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that switcher overview pages can be panned with gestures
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 13 buttons. That's enough buttons to span over three pages.
    #  * Pan switcher to second page with gesture
    #  * Verify that second apge is viewable
    #* Post-conditions
    #  * None
    def test_05_pages_are_panned_with_gesture
        added_buttons = 13

        added_buttons.times { |index|
            create_switcher_application(index)
        }

        verify_equal(added_buttons) { count_switcher_buttons }

        # Verify that correct switcher buttons are visible for the page that is viewed
        # Buttons in first page
        # Buttons have x position value, when they are visible on the screen
        verify { button_x(0) }
        verify { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify_not { button_x(12) }

        # Test gestures that should do nothing
        5.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Right, 2, 600, :Left) }

        # Verif that buttons in first page are still viewable
        verify { button_x(0) }
        verify { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify_not { button_x(12) }

        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left)

        # Verif that buttons in second page are still viewable
        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify { button_x(6) }
        verify { button_x(11) }
        verify_not { button_x(12) }

        # Test gestures that should do nothing
        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Right, 2, 100, :Left)
        2.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Down, 2, 400, :Left) }
        2.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Up, 2, 400, :Left) }

        # Buttons in second page
        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify { button_x(6) }
        verify { button_x(11) }
        verify_not { button_x(12) }

        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left)

        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify { button_x(12) }

        3.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left) }
        2.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 200, :Left) }
        2.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left) }

        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify { button_x(12) }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that when last item is removed from the page, switcher overview is panned to the first page
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 7 buttons
    #  * Remove last button that is on the second page
    #  * Verify that switcher overview is panned to the first page
    #* Post-conditions
    #  * None
    def test_06_pages_are_panned_automatically_when_page_is_emptied
        added_buttons = 7
        index_to_remove = 6

        buttons = Array.new
        added_buttons.times { |index|
            pid, name = create_switcher_application(index)
            buttons << pid
        }

        verify_equal (added_buttons) { count_switcher_buttons }

        # Move to the second/last page
        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left)
        verify { button_x(6) }
        verify_not { button_x(0) }
        verify_not { button_x(5) }

        remove_application_by_pid(buttons[index_to_remove])

        verify_equal (added_buttons-1) { count_switcher_buttons }

        verify { button_x(0) }
        verify { button_x(5) }
        verify_not { button_x(6) }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that activating callui application places the callui switcher overview button
    # to the first place in first page and switcher overview is also panned to first page.
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 14 buttons. That's over three pages.
    #  * Add callui non-activated application
    #  * Activate callui application call mode
    #  * Verify that callui button is moved to be the first button in switcher overview
    #  * Verify that switcher overview is panned to the first page
    #* Post-conditions
    #  * None
    def test_07_activating_call_ui_application
        added_buttons = 13

        added_buttons.times { |index|
            create_switcher_application(index)
        }

        create_callui_application(added_buttons)

        verify { button_x(0) }
        verify { button_x(5) }
        verify_not { @home.SwitcherButton(:text => 'Ta_CallUi-' + added_buttons.to_s).attribute('x').to_i }

        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left)

        # In second page
        verify_not { button_x(5) }
        verify { button_x(6) }
        verify { button_x(11) }
        verify_not { button_x(12) }

        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left)

        # In third page
        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify { button_x(12) }
        verify { @home.SwitcherButton(:text => 'Ta_CallUi-' + added_buttons.to_s).attribute('x').to_i }

        @sut.application(:name => 'ta_callui').DuiButton( :text => 'Ta_CallUi-' + added_buttons.to_s ).tap

        verify { button_x(0) }
        verify { button_x(4) }
        verify_not { button_x(5) }
        verify { @home.SwitcherButton(:text => 'Ta_CallUi-' + added_buttons.to_s + ' call' ).attribute('x').to_i }

        callui_x_activated = @home.SwitcherButton(:text => 'Ta_CallUi-' + added_buttons.to_s + ' call' ).attribute('x').to_i
        callui_y_activated = @home.SwitcherButton(:text => 'Ta_CallUi-' + added_buttons.to_s + ' call' ).attribute('y').to_i

        (0..4).each { |index|
            verify_true { callui_x_activated <= button_x(index) }
            verify_true { callui_y_activated <= button_y(index) }
        }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that application is removed when switcher overview button's close button is pressed
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 6 buttons.
    #  * Tap close button of switcher overview button
    #  * Verify that correct button and application is removed
    #* Post-conditions
    #  * None
    def test_08_close_application_from_switcher_overview_button
        added_buttons = 6
        remove_button_index = 2

        buttons = Array.new
        button = Struct.new(:name, :pid)
        added_buttons.times { |index|
            pid, name = create_switcher_application(index)
            buttons << button.new(name, pid)
        }

        verify_equal(added_buttons) { count_switcher_buttons }

        verify_true { application_exists_by_pid(buttons[remove_button_index].pid) }

        name = buttons[remove_button_index].name
        button_width = button_width(remove_button_index)
        @home.SwitcherButton(:text => name).tap_object(button_width-15, 0+15)

        verify_equal(added_buttons-1) { count_switcher_buttons }

        verify_false { application_exists_by_pid(buttons[remove_button_index].pid) }
        verify_false { button_exists(remove_button_index) }
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that switcher overview buttons are ordered correctly in portrait mode
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 2 buttons.
    #  * Verify that buttons are in correct order and place
    #  * Verify that buttons are same size
    #  * Add 2 buttons
    #  * Verify that buttons are in correct order and place
    #  * Verify that size of the buttons are reduced
    #  * Verify that buttons are same size
    #  * Add 2 buttons
    #  * Verify that buttons are in correct order and place
    #  * Verify that size of the buttons are reduced
    #  * Verify that buttons are same size
    #* Post-conditions
    #  * None
    def test_09_buttons_are_in_correct_columns_and_rows_in_overview_switcher_with_different_button_numbers_in_portrait_mode

        # Index of size compare button
        button_index = 0
        # Tolerated error margin
        range = 2

        ctx = open_context_provider
        set_orientation_portrait(ctx)

        create_switcher_application(0)
        create_switcher_application(1)

        verify_equal(2) { count_switcher_buttons }

        # Buttons in large size
        verify_true { button_x(0) < button_x(1) }
        verify_true { button_y(0) == button_y(1) }

        verify_true { button_width(0) == button_width(1) && button_height(1) == button_height(1) }

        button_0_width_large = button_width(0)
        button_0_height_large = button_height(0)

        create_switcher_application(2)
        create_switcher_application(3)
        verify_equal(4) { count_switcher_buttons }

        # Buttons in medium size
        verify_true { button_0_width_large > button_width(0) && button_0_height_large > button_height(0) }

        verify_true { button_x(0) < button_x(1) }
        verify_true { button_x(2) < button_x(3) }
        verify_true { button_y(0) == button_y(1) }
        verify_true { button_y(2) == button_y(3) }
        verify_true { button_y(0) < button_y(2) }
        verify_true { button_x(0) == button_x(2) }
        verify_true { button_x(1) == button_x(3) }

        width_range, height_range = get_button_size_range(button_index, range)
        count_switcher_buttons.times { |index|
            verify_true { width_range.include?(button_width(index)) }
            verify_true { height_range.include?(button_height(index)) }
        }

        button_0_width_medium = button_width(0)
        button_0_height_medium = button_height(0)

        create_switcher_application(4)
        create_switcher_application(5)
        verify_equal(6) { count_switcher_buttons }

        #Buttons in small size
        verify_true { button_0_width_medium > button_width(0) && button_0_height_medium > button_height(0) }

        verify_true { button_x(0) < button_x(1) }
        verify_true { button_x(2) < button_x(3) }
        verify_true { button_x(4) < button_x(5) }
        verify_true { button_y(0) == button_y(1) }
        verify_true { button_y(2) == button_y(3) }
        verify_true { button_y(4) == button_y(5) }
        verify_true { button_y(0) < button_y(2) && button_y(2) < button_y(4) }
        verify_true { button_x(0) == button_x(2) }
        verify_true { button_x(1) == button_x(3) }
        verify_true { button_x(2) == button_x(4) }
        verify_true { button_x(3) == button_x(5) }

        width_range, height_range = get_button_size_range(button_index, range)
        count_switcher_buttons.times { |index|
            verify_true { width_range.include?(button_width(index)) }
            verify_true { height_range.include?(button_height(index)) }
        }

        close_context_provider(ctx)
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that pages can be panned with gestures in portrait mode
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 13 buttons. That's three pages.
    #  * Pan through pages with gestures
    #  * Verify that incorrect gestures does nothing
    #  * Verify that correct buttons are viewaple for different pages
    #* Post-conditions
    #  * None
    def test_10_panning_multiple_pages_with_gestures_in_portrait_mode
        ctx = open_context_provider
        set_orientation_portrait(ctx)

        added_buttons = 13

        added_buttons.times { |index|
            create_switcher_application(index)
        }

        # First page
        verify { button_x(0) }
        verify { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify_not { button_x(12) }

        # Test gestures that should do nothing
        3.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Down, 2, 400, :Left) }
        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 600, :Left)
        3.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Left, 2, 20, :Left) }

        # First page
        verify { button_x(0) }
        verify { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify_not { button_x(12) }

        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Up, 2, 400, :Left)

        #second page
        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify { button_x(6) }
        verify { button_x(11) }
        verify_not { button_x(12) }

        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Up, 2, 400, :Left)

        #third page
        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify { button_x(12) }

        # Test gestures that should do nothing
        3. times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Up, 2, 400, :Left) }
        @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Right, 2, 600, :Left)

        #third page
        verify_not { button_x(0) }
        verify_not { button_x(5) }
        verify_not { button_x(6) }
        verify_not { button_x(11) }
        verify { button_x(12) }

        close_context_provider(ctx)
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that closing application from swticher overview button, the application is closed, button is removed
    # and palcement of other buttons are reordered in portrait mode
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 13 buttons. That's three pages.
    #  * Pan to the last page with gestures
    #  * Close the application from the last switcher overview button
    #  * Verify that correct application is closed, button is removed and view is panned to second page
    #  * Close the application from the 6th switcher overview button
    #  * Verify that correct application is closed, button is removed and other buttons are reordered correctly
    #* Post-conditions
    #  * None
    def test_11_close_application_from_switcher_overview_button_in_portrait_mode
        ctx = open_context_provider
        set_orientation_portrait(ctx)

        added_buttons = 13

        buttons = Array.new
        button = Struct.new(:name, :pid)
        added_buttons.times { |index|
            pid, name = create_switcher_application(index)
            buttons << button.new(name, pid)
        }

        2.times { @home.Switcher(:objectName => 'OverviewSwitcher').gesture(:Up, 2, 400, :Left) }

        verify { button_x(12) }
        verify_true { application_exists_by_pid(buttons.at(12).pid) }

        name = buttons[12].name
        button_width = button_width(12)
        button_height = button_height(12)
        @home.SwitcherButton(:text => name).tap_object(button_width - 15, button_height - 15)

        verify_equal(added_buttons-1) { count_switcher_buttons }

        verify_false { application_exists_by_pid(buttons[12].pid) }
        verify_not { button_x(12) }
        verify_false { button_exists(12) }
        verify { button_x(6) }
        verify { button_x(11) }
        verify_not { button_x(5) }

        button_11_x_old = button_x(11)
        button_11_y_old = button_y(11)
        button_8_x_old = button_x(8)
        button_8_y_old = button_y(8)

        name = buttons[6].name
        button_width = button_width(6)
        button_height = button_height(6)
        @home.SwitcherButton(:text => name).tap_object(button_width - 15, button_height - 15)

        verify_equal(added_buttons - 2) { count_switcher_buttons }

        verify_false { application_exists_by_pid(buttons[6].pid) }
        verify_not { button_x(6) }
        verify_false { button_exists(6) }
        verify { button_x(7) }
        verify { button_x(11) }
        verify_not { button_x(5) }

        verify_true { button_y(7) == button_y(8) }
        verify_true { button_y(9) == button_y(10) }
        verify_true { button_x(7) == button_x(9) && button_x(9) == button_x(11) }
        verify_true { button_x(8) == button_x(10) }

        close_context_provider(ctx)
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that when removing buttons in portrait mode button sizes are changed accordingly
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 6 buttons.
    #  * Verify that buttons are same size
    #  * Remove 2 buttons
    #  * Verify that button size has changed to medium
    #  * Verify that buttons are same size
    #  * Remove 2 buttons
    #  * Verify that button size has changed to large
    #  * Verify that buttons are same size
    #* Post-conditions
    #  * None
    def test_12_when_removing_buttons_from_portrait_mode_button_size_is_correctly_handled

        button_index = 0
        range = 2

        ctx = open_context_provider
        set_orientation_portrait(ctx)

        buttons = Array.new
        6.times { |index|
            pid, name = create_switcher_application(index)
            buttons << pid
        }
        verify_equal(6) { count_switcher_buttons }

        width_range, height_range = get_button_size_range(button_index, range)
        count_switcher_buttons.times { |index|
            verify_true { width_range.include?(button_width(index)) }
            verify_true { height_range.include?(button_height(index)) }
        }

        #Buttons in small size
        button_0_size_width_small = button_width(0)
        button_0_size_height_small = button_height(0)

        remove_application_by_pid(buttons[5])
        remove_application_by_pid(buttons[4])

        verify_equal(4) { count_switcher_buttons }

        #Buttons in medium size
        verify_true { button_0_size_width_small < button_width(0) && button_0_size_height_small < button_width(0) }
        width_range, height_range = get_button_size_range(button_index, range)
        count_switcher_buttons.times { |index|
            verify_true { width_range.include?(button_width(index)) }
            verify_true { height_range.include?(button_height(index)) }
        }

        button_0_size_width_medium = button_width(0)
        button_0_size_height_medium = button_height(0)

        remove_application_by_pid(buttons[3])
        remove_application_by_pid(buttons[2])

        verify_equal(2) { count_switcher_buttons }

        # Buttons in large size
        verify_true { button_0_size_width_medium < button_width(0) && button_0_size_height_medium < button_width(0) }
        width_range, height_range = get_button_size_range(button_index, range)
        verify_true { width_range.include?(button_width(1)) }
        verify_true { height_range.include?(button_height(1)) }

        close_context_provider(ctx)
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that when buttons is removed from switcher other buttons are ordered correctly in portrait and in landscape mode
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Add 6 buttons.
    #  * Pan to the last page with gestures
    #  * Close the application from the last switcher overview button
    #  * Verify that correct application is closed, button is removed and view is panned to second page
    #  * Close the application from the 6th switcher overview button
    #  * Verify that correct application is closed, button is removed and other buttons are reordered correctly
    #* Post-conditions
    #  * None
    def test_13_that_when_button_is_removed_from_switcher_buttons_are_ordered_correctly_in_portrait_and_in_landscape_mode
        ctx = open_context_provider

        remove_button_index = 1

        buttons = Array.new
        6.times { |index|
            pid, name = create_switcher_application(index)
            buttons << pid
        }

        set_orientation_portrait(ctx)

        # Portrait mode
        verify_true { button_x(0) == button_x(2) }
        verify_true { button_x(2) == button_x(4) }
        verify_true { button_y(0) == button_y(1) }
        verify_true { button_y(2) == button_y(3) }
        verify_true { button_y(4) == button_y(5) }

        remove_application_by_pid(buttons[remove_button_index])

        verify_false { button_exists(remove_button_index) }
        verify_equal(5) { count_switcher_buttons }

        set_orientation_landscape(ctx)

        #Landscape mode
        verify_true { button_x(0) == button_x(4) }
        verify_true { button_x(2) == button_x(5) }
        verify_true { button_y(0) == button_y(2) }
        verify_true { button_y(2) == button_y(3) }
        verify_true { button_y(4) == button_y(5) }

        close_context_provider(ctx)
    end

private
    # Helper function that returns handles to std_in, std_out and std_err
    # in a convinient struct that can be passed to helper functions
    # set_orientation_landscape and set_orientation_portrait
    #
    # Must be closed with close_context_provider
    def open_context_provider
        # use context provider to change orientation
        cmd = "context-provide"
        cmd_in, cmd_out, cmd_err = Open3::popen3(cmd)
        cmd_in.puts "add string Screen.TopEdge top"

        context_provider = Struct.new(:cmd_in, :cmd_out, :cmd_err)

        return context_provider.new(cmd_in, cmd_out, cmd_err)
    end

    def set_orientation_landscape(context_provider)
        @@landscape_mode = true
        context_provider.cmd_in.puts "Screen.TopEdge=top"
    end

    def set_orientation_portrait(context_provider)
        @@landscape_mode = false
        context_provider.cmd_in.puts "Screen.TopEdge=right"
    end

    def close_context_provider(context_provider)
        set_orientation_landscape(context_provider)
        context_provider.cmd_in.close
        context_provider.cmd_out.close
        context_provider.cmd_err.close
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

        return count.to_i
    end

    def switcher_buttons_array
        count = 0
        buttons = Array.new
        buttonStruct = Struct.new(:text, :x, :y, :w, :h)
        begin
            button = @home.SwitcherButton(:__index => count)
            buttons << buttonStruct.new(button.attribute('text'), button.attribute('x'), button.attribute('y'),
                                                button.attribute('width'), button.attribute('height') )
            count = count + 1
        rescue NoMethodError, MobyBase::TestObjectNotFoundError
            button = nil
        end while button

        return buttons
    end

    def close_all_applications_by_name(name)
        begin
            app = @sut.application( :name => name )
            app.close
        rescue MobyBase::TestObjectNotFoundError
            app = nil
        end while app
    end

    def create_switcher_application(index)
        name = 'Ta_HomeSwitcher-' + index.to_s
        @sut.run(:name => '/usr/lib/duifw-home-tests/ta_homeswitcher',
        :arguments => '-software,-id,' + index.to_s)

        return @sut.application.attribute('processId'), name
    end

    def create_callui_application(index)
        title = 'Ta_CallUi-' + index.to_s
        @sut.run(:name => '/usr/lib/duifw-home-tests/ta_callui',
        :arguments => title)

        return @sut.application.attribute('processId'), title
    end

    def remove_application_by_pid(pid)
        begin
            @sut.application(:objectName => 'ta_homeswitcher', :id => pid.to_s).close
        rescue
            puts 'No application to remove with index ' + pid.to_s
        end
    end

    def application_exists_by_pid(pid)
        exists = false
        begin
            exists = true if @sut.application(:objectName => 'ta_homeswitcher', :id => pid.to_s)
        rescue
        end
        return exists
    end

    def tap_to_button_and_verify_application_launched (name)
        @home.SwitcherButton( :text => name ).tap
        verify_equal(name, 10) { @sut.application.DuiApplicationWindow.attribute('windowTitle') }
    end

    def button_visible(index)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).attribute('visible')
    end

    # Gets test object from switcher overview button by name determined by it's index
    def button_by_index(index)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s )
    end

    def button_exists(index)
        exists = false
        begin
            exists = true if @home.SwitcherButton(:text => 'Ta_HomeSwitcher-' + index.to_s)
        rescue
        end
        return exists
    end

    # Returns the x position of button
    # Returns the y postition in portraitmode for convient usage
    def button_x(index)
        attrb ='x'
        attrb = 'y' if !@@landscape_mode
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).attribute(attrb).to_i
    end

    def button_y(index)
        value = 0
        if !@@landscape_mode then
            value = @@screen_width - @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).attribute('x').to_i
        else
            value = @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).attribute('y').to_i
        end
        return value

    end

    def button_width(index)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).attribute('width').to_i
    end

    def button_height(index)
        @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).attribute('height').to_i
    end

    # Calculates width and height range of button
    # index is the index of button
    # range is a value that is added and removed from size to get the max and min values of range
    # return range of width and range of height
    def get_button_size_range(index, range)
        width = button_width(index)
        height = button_height(index)

        width_max = width + range
        width_min = width - range

        height_max = height + range
        height_min = height - range

        return (width_min ... width_max), (height_min ... height_max)
    end

end
