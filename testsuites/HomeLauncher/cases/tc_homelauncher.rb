#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: testhome test suite functional cases for Awidget
#  * Objectives: test the functionality of Awidget
#
#  Author:: Pankaj Saharan (mailto:pankaj.saharan@nokia.com)
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

#SuiteDomain::            Application framework
#SuiteTimeout::           3600
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          false
#SetFeature::             DUI-2180 in ScrumWorks Pro
#SetTimeout::             100
#
# These tests verify duihome launch and startup.
class TC_homelauncher < Dui::TestCase

    # method called before any test case
    def setup
        @app = @sut.application(:name => 'duihome')
        @tempPath = ENV['TEMP'] || ENV['TMP'] || '/tmp'
	@totalPath = @tempPath + "/tc_homelauncher*.desktop"
	Dir.glob(@totalPath).each {|f| File.delete(f) }
    end

    # method called after any test case for cleanup purposes
    def teardown
	Dir.glob(@totalPath).each {|f| File.delete(f) }
    end


    #SubFeature::   DUI-2180 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that search button is available on the home screen and taps on it
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Check whether the search button is visible in the homescreen
    #  * Tap on search button
    #* Post-conditions
    #  * None
    #* References
    #  * None
if false # FIXME: no navigation button in current implementation
    def test_search_button_tap
        # Close launcher if it's open
        begin
            launcherButton = @app.DuiButton(:name => 'ShowLauncherButton',
                                            :iconID => 'Icon-home')
            launcherButton.tap
        rescue
        end

        searchButton = @app.DuiButton(:name => 'NavigationButton')
        verify { searchButton.tap }
    end
end
    #SubFeature::   DUI-2180 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Opens up Launcher Menu of Home Screen and navigates through the items available
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Check whether the launcher button is visible in the homescreen
    #  * Tap on launcher button
    #  * Verifly that the launcher opened
    #  * Add a desktop entry by the name of 'Test App'
    #  * Verify that the number of launcher entries by the name of 'Test App'
    #    inreased by one
    #  * Add another desktop entry by the name of 'Test App'
    #  * Verify that the number of launcher entries by the name of 'Test App'
    #    inreased by one
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_launcher
        launcherButton =  @app.DuiButton(:name => 'ShowLauncherButton')
        verify_equal('true', 5) { launcherButton.attribute('visible') }

        verify { launcherButton.tap }

        number_of_test_app_entries = add_app_desktop_entry(@tempPath + "/tc_homelauncher1.desktop")
        verify_equal(number_of_test_app_entries, 10) { count_launcher_buttons(:text => 'Test App') }

        number_of_test_app_entries = add_app_desktop_entry(@tempPath + "/tc_homelauncher2.desktop")
        verify_equal(number_of_test_app_entries, 10) { count_launcher_buttons(:text => 'Test App') }

        @app.DuiButton(:name => 'ShowLauncherButton').tap
    end

    #SubFeature::   DUI-2180 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verify that all buttons are still visible on screen
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * open launcher
    #  * verify that all buttons in launcher are visible on screen
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_launcher_buttons_visibility
        @app.DuiButton(:name => 'ShowLauncherButton').tap

        verify_launcher_buttons_visibility('landscape')

        @app.DuiButton(:name => 'ShowLauncherButton').tap
    end

    #SubFeature::   DUI-2180 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verify that all buttons are still visible on screen after orientations
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * open launcher
    #  * add more desktop entries to be sure that more than one column is full
    #  * change orientation to portrait
    #  * verify that all buttons are visible on screen
    #  * change orientation to landscape and go to homescreen
    #  * change orientation to portrait while in homescreen
    #  * open launcher
    #  * verify that all buttons are visible on screen
    #* Post-conditions
    #  * None
    #* References
    #  * Verifies bug NB#155106
    def test_launcher_buttons_visibility_with_orientation_changes
        @app.DuiButton(:name => 'ShowLauncherButton').tap
        sleep 5

        entry_amount = 0
        # add some more launcher buttons
        while 5 > entry_amount do
            add_app_desktop_entry("#{@tempPath}/tc_homelauncher%d.desktop" % entry_amount)
            entry_amount += 1
        end
        # use context provider to change orientation
        cmd = "context-provide"
        cmd_in, cmd_out, cmd_err = Open3::popen3(cmd)
        cmd_in.puts "add string Screen.TopEdge top"
        # change orientation to portrait
        cmd_in.puts "Screen.TopEdge=right"
        sleep 5
        verify_launcher_buttons_visibility('portrait')
        cmd_in.puts "Screen.TopEdge=top"
        sleep 5
        # verify that orientation is changed also when launcher is not shown
        @app.DuiButton(:name => 'ShowLauncherButton').tap
        sleep 10
        # while launcher not shown, change orientation to landscape
        cmd_in.puts "Screen.TopEdge=right"
        sleep 5
        @app.DuiButton(:name => 'ShowLauncherButton').tap
        sleep 5
        verify_launcher_buttons_visibility('portrait')

        cmd_in.puts "Screen.TopEdge=top"
        # close streams
        cmd_in.close
        cmd_out.close
        cmd_err.close

        @app.DuiButton(:name => 'ShowLauncherButton').tap
    end

private

    def verify_launcher_buttons_visibility( orientation )
        if orientation == 'landscape'
            mainWindow_width = @app.MainWindow.attribute('width')
        else
            mainWindow_width = @app.MainWindow.attribute('height')
        end

        # verify that all buttons in launcher are visible
        i = 0
        begin
            launcherButton = @app.Launcher.LauncherButton( :__index => i )
            verify_equal('true', 5) { launcherButton.attribute('visible') }
            if orientation == 'landscape'
                button_pos = launcherButton.attribute('x')
            else
                button_pos = launcherButton.attribute('y')
            end
            verify_true{ button_pos < mainWindow_width }
            i += 1
        rescue NoMethodError, MobyBase::TestObjectNotFoundError
            launcherButton = nil
        end while launcherButton
        # check that there actually were launcherbuttons in launcher
       # verify_true(10) { i > 0 }
    end

    # An auxilary function that returns the current count of LauncherButton
    # objects
    def count_launcher_buttons(args = {})
        count = 0
        begin
            args[:__index] = count

            button = @app.LauncherButton(args)
            count += 1
        rescue NoMethodError, MobyBase::TestObjectNotFoundError
            button = nil
        end while button

        return count
    end

    # add test app desktop entry for launcher
    # return current number of test app buttons
    def add_app_desktop_entry( path_to_new_desktop_file )
        desktopentry = "[Desktop Entry]\n"\
                       "Type=Application\n"\
                       "Name=Test App\n"\
                       "Icon=icon-l-video\n"\
                       "Exec=foo\n"\
                       "OnlyShowIn=X-DUI;\n";

        # Make no assumptions about already present entries with the
        # name "Test App"
        n = count_launcher_buttons(:text => 'Test App')

        File.open(path_to_new_desktop_file, 'w') {|f| f.write(desktopentry) }

        return n + 1
    end
end
