#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: HomeSwitcher test suite functional cases for duihome
#  * Objectives: test the functionality of duihome switcher
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
require 'nokogiri'

#SuiteDomain::            ApplicationFramework
#SuiteTimeout::           500
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          false
#SetLogFiles::            /logs/testability/tc_switcherbuttonorder.log
#SetFeature::             DUI-2005 in ScrumWorks Pro
#SetTimeout::             500

# These tests verify duihome switcherbuttons ordering and callui functionality
# These tests should work if there is switcher applications/buttons left from earlier tests
class TC_switcherbuttonorder < Dui::TestCase

    def setup
        @home  = @sut.application(:name => 'duihome')
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
    #  * Check that the switcher is visible on the desktop
    #  * Close all applications that have same names that are used in these tests
    #* Post-conditions
    #  * None
    def test_01_switcher_is_shown
        verify(0, "Switcher not found") { @home.Switcher }

        close_all_applications_by_name('ta_homeswitcher')
        close_all_applications_by_name('ta_callui')
    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Check that the when button is added to the switcher it's placed at the end of the switcher list
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Launch 4 applications
    #  * Add one application
    #  * Verify that added button is last button switcher
    #* Post-conditions
    #  * None
    def test_02_add_new_switcherbutton_at_the_end_of_the_switcher

        added_buttons = 4

        added_buttons.times { |index|
            create_switcher_application(index)
        }

        # Test that there is at least added buttons in switcher
        # There can be switcher buttons from previous (failed) test suites
        verify_true { count_switcher_buttons >= added_buttons }

        # Add one button
        create_switcher_application(added_buttons)

        # Assures that buttons are added to switcher
        verify_true { count_switcher_buttons >= (added_buttons + 1) }

        added_button_name = 'Ta_HomeSwitcher-' + added_buttons.to_s
        added_button_pos = @home.SwitcherButton(:text => added_button_name).attribute('pos')

        switcher_buttons_array.each { |button|
            # Iterates through every button switcher, hence the greater than or equal
            verify_true { added_button_pos.to_i >= button[:pos] }
        }

    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test that order of the switcherbuttons remains the same after the removal of switcherbutton
    # from middle of the switcher
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Launch 5 applications
    #  * Remove one application
    #  * Verify that one switcher button is removed
    #  * Verify that the order of the switcher buttons haven't changed
    #* Post-conditions
    #  * None
    def test_03_test_removing_switcherbutton_from_the_middle_of_the_switcher

        app_to_delete_index = 2
        added_buttons = 5

        application = Struct.new(:pos, :buttonName, :appPid)

        applications = Array.new

        added_buttons.times { |index|
            pid, name = create_switcher_application(index)
            pos = @home.SwitcherButton(:text => name).attribute('pos')
            applications << application.new(pos.to_i, name, pid)
        }

        verify_false { applications.empty? }

        button_count_before_delete = count_switcher_buttons
        verify_true { button_count_before_delete >= added_buttons }

        # Get buttons in switcher before removal of one button.
        buttons_before_removal = switcher_buttons_array
        # Sort buttons by their positions
        buttons_before_removal = buttons_before_removal.sort_by { |button| button[:pos] }

        # Delete button
        deleted_button_name = applications[app_to_delete_index].buttonName
        remove_application_by_pid(applications[app_to_delete_index].appPid)

        verify_equal(button_count_before_delete - 1) { count_switcher_buttons }

        # Get buttons in switcher after the removal of button
        buttons_after_removal = switcher_buttons_array
        buttons_after_removal = buttons_after_removal.sort_by { |button| button[:pos] }

        # Get names from before and after button arrays
        button_names_before = buttons_before_removal.map do |button|
            button[:buttonName]
        end

        button_names_after = buttons_after_removal.map do |button|
            button[:buttonName]
        end

        # Delete name of the button that was removed from the before array
        # so that it should match to the after array.
        # This doesn't affect relational positions of buttons
        button_names_before.delete(deleted_button_name)

        # Before and after name arrays should now be same
        verify_true { button_names_before.eql?(button_names_after) }

    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test launching application with call mode on
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Launch 3 applications
    #  * Launch callui test application with callmode on
    #  * Verify that the switcherbutton of callui application is first button in switcher
    #* Post-conditions
    #  * None
    def test_04_test_launching_callui

        basic_apps = 3

        basic_apps.times { |index|
            create_switcher_application(index)
        }

        call_mode = 'call'
        create_callui_application_in_callmode(basic_apps, call_mode)

        verify_true { count_switcher_buttons >= (basic_apps +1) }

        callui_pos = @home.SwitcherButton(:text => 'Ta_CallUi-' + basic_apps.to_s + ' ' + call_mode ).attribute('pos')

        switcher_buttons_array.each { |button|
            verify_true { callui_pos.to_i <= button[:pos] }
        }

    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test activating callmode of callui application after launching of callui application
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Launch 3 applications
    #  * Launch callui test application with callmode off
    #  * Activate callmode of callui
    #  * Verify that the switcherbutton of callui application is first button in switcher
    #* Post-conditions
    #  * None
    def test_05_test_activating_call_ui

        basic_apps = 3

        basic_apps.times { |index|
            create_switcher_application(index)
        }

        create_callui_application(basic_apps)

        verify_true { count_switcher_buttons >= (basic_apps + 1) }

        callui_pos = @home.SwitcherButton(:text => 'Ta_CallUi-' + basic_apps.to_s ).attribute('pos')

        # Check that call ui is initially placed last on the switcher list
        switcher_buttons_array.each { |button|
            verify_true { callui_pos.to_i >= button[:pos]  }
        }

        # Activate callui
        @sut.application(:name => 'ta_callui').DuiButton( :text => 'Ta_CallUi-' + basic_apps.to_s ).tap

        callui_pos_activated = @home.SwitcherButton(:text => 'Ta_CallUi-' + basic_apps.to_s + ' call' ).attribute('pos')

        # Check that activated call ui is first on the switcher list
        switcher_buttons_array.each { |button|
            verify_true { callui_pos_activated.to_i <= button[:pos]  }
        }

    end

    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test when callmode is activated in callui application the switcher is
    # panned so that switcher button for callui application is viewable
    #
    #* Pre-conditions
    #  * duihome is started
    #* Test steps
    #  * Launch 7 applications. Thats more than enough buttons to assure that not all buttons are viewable at the same time.
    #  * Launch callui test application with callmode off
    #  * Activate callmode of callui
    #  * Verify that the switcherbutton of callui application is first button in switcher
    #  * Verify that callui is viewable
    #* Post-conditions
    #  * None
    def test_06_panning_switcher_buttons_to_callui

        basic_apps = 7

        basic_apps.times { |index|
            create_switcher_application(index)
        }

        create_callui_application(basic_apps)

        verify_true { count_switcher_buttons >= (basic_apps+1) }

        # Shift focus to the end of the switcher button list
        basic_apps.times { |index|
            @home.SwitcherButton( :text => 'Ta_HomeSwitcher-' + index.to_s ).gesture(:Left, 2, 400, :Left)
        }

        # Verify that callui switcher button is currently viewable
        verify { @home.SwitcherButton( :text => 'Ta_CallUi-' + basic_apps.to_s ).attribute('x') }

        @sut.application(:name => 'ta_callui').DuiButton( :text => 'Ta_CallUi-' + basic_apps.to_s ).tap

        callui_pos_activated = @home.SwitcherButton(:text => 'Ta_CallUi-' + basic_apps.to_s + ' call' ).attribute('pos')

        # Verify that activated callui button is placed first in the switcher
        switcher_buttons_array.each { |button|
            verify_true { callui_pos_activated.to_i <= button[:pos] }
        }

        # Swticher should be panned so that callui is viewable
        # Verify that callui switcher button is viewable
        verify { @home.SwitcherButton( :text => 'Ta_CallUi-' + basic_apps.to_s + ' call' ).attribute('x') }

    end

    #Auxiliary functions
    def remove_application_by_pid(pid)
        begin
            @sut.application(:objectName => 'ta_homeswitcher', :id => pid.to_s).close
        rescue
            puts 'No application to remove with index ' + pid.to_s
        end
    end

    def create_callui_application_in_callmode(index, callmode)
        title = 'Ta_CallUi-' + index.to_s
        @sut.run(:name => '/usr/lib/duifw-home-tests/ta_callui',
        :arguments => "#{title}, #{callmode}")

        return @sut.application.attribute('processId'), title
    end

    def create_callui_application(index)
        title = 'Ta_CallUi-' + index.to_s
        @sut.run(:name => '/usr/lib/duifw-home-tests/ta_callui',
        :arguments => title)

        return @sut.application.attribute('processId'), title
    end

    def create_switcher_application(index)
        name = 'Ta_HomeSwitcher-' + index.to_s
        @sut.run(:name => '/usr/lib/duifw-home-tests/ta_homeswitcher',
        :arguments => '-software,-id,' + index.to_s)

        return @sut.application.attribute('processId'), name
    end

    # An auxiliary function that returns the current count of SwitcherButton
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

    def switcher_buttons_array
        count = 0
        buttons = Array.new
        begin
            button = @home.SwitcherButton(:__index => count)
            buttons << { :buttonName => button.attribute('text'), :pos => button.attribute('pos').to_i }
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

end
