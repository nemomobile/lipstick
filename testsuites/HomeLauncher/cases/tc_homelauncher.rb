#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: testhome test suite functional cases for Awidget
#  * Objectives: test the functionality of Awidget
#
#  Author:: Pankaj Saharan (mailto:pankaj.saharan@nokia.com), Janne Sakko (mailto:janne.sakko@ixonos.com)
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
#SetFeature::             DUI-1949 in ScrumWorks Pro
#SetTimeout::             100
#
# These tests verify launcher paging and launcher buttons ordering.
class TC_homelauncher < Dui::TestCase

    $launcher_landscape_mode = true

    # method called before any test case
    def setup
        @@landscape_mode = $launcher_landscape_mode
        @@button_name = 'Test App'
        @@number_format = "%02d"
        @@size_range = 2
        @@buttons_on_a_page = 12

        @@launcher_open_verification_time = 10
        @@launcher_close_verification_time = 10

        @@add_app_verify_time = 5

        # General verify timeout
        @@verify_time = 5

        # Gestures
        @@number_of_tries = 5
        @@gesture_duration = 1 #seconds
        @@gesture_button = :Left
        @@gesture_distance = 600

        # Verify timeout after panning with gesture
        @@gesture_sleep = 5

        launch_duihome

        @tempPath = ENV['TEMP'] || ENV['TMP'] || '/tmp'
        @totalPath = @tempPath + "/tc_homelauncher*.desktop"
        Dir.glob(@totalPath).each {|f| File.delete(f) }
    end

    # method called after any test case for cleanup purposes
    def teardown
        Dir.glob(@totalPath).each {|f| File.delete(f) }
    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
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

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher is opened and closed from the ToggleLauncherButton
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher and verify that it is open
    #  * Close launcher and verify that it is closed
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_01_launcher_opening_and_closing

        open_launcher

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher buttons are placed correctly to launcher pages
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applications until there is one full page of new applications
    #  * Verify that buttons are in expected grid form
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_02_ordering_of_buttons_on_a_page

        open_launcher

        startIndex = fill_missing_pages

        add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 5],
                                            [6, 7, 8, 9,10,11] )

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher pages can be panned with gestures and faulty gestures are ignored
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applications until there is one page full of new applications
    #  * Pan right to that page and verify that applications on that page are visible
    #  * Add application to create next page
    #  * Pan to the next page and verify application in that page is visible
    #  * Pan right to the page full of new applications and verify that applications are visible
    #  * Verify that pan gestures to up and down do nothing
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_03_panning_with_gestures

        open_launcher

        startIndex = fill_missing_pages

        nextIndex = add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        # Test gestures that should do nothing
        pan_with_gesture(:Left, @@gesture_distance)

        # Verify that new full page is still visible
        verify_true(@@gesture_sleep, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        # Add application so that new page is spawned
        add_application(nextIndex)

        pan_with_gesture(:Left, @@gesture_distance)

        verify_true(@@gesture_sleep, "Launcher button should be visible") { launcher_button_visible(nextIndex) }
        verify_false(@@verify_time, "Launcher button should not be visible") { launcher_button_visible(startIndex) }

        pan_with_gesture(:Right, @@gesture_distance)

        verify_false(@@gesture_sleep, "Launcher button should not be visible") { launcher_button_visible(nextIndex) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        # Gesture should do nothing
        pan_with_gesture(:Up, @@gesture_distance)

        verify_false(@@gesture_sleep, "Launcher button should not be visible") { launcher_button_visible(nextIndex) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        # Gesture should do nothing
        pan_with_gesture(:Down, @@gesture_distance)

        verify_false(@@gesture_sleep, "Launcher button should not be visible") { launcher_button_visible(nextIndex) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher buttons are kept in launcher when launher is closed and opened again
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applications until there is one full page of new buttons
    #  * Check that applications are in expected grid
    #  * Close and reopen launcher
    #  * Check that applications are in expected grid
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_04_keeping_button_order_when_launcher_closed_and_reopened

        open_launcher

        startIndex = fill_missing_pages

        add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 5],
                                            [6, 7, 8, 9,10,11] )

        close_launcher

        open_launcher

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 5],
                                            [6, 7, 8, 9,10,11] )

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that when last button is removed from the page, the page is removed
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applications until there is one full page of new buttons
    #  * Add one application that new page is created
    #  * Pan to the last page of launcher that has one button
    #  * Remove that last button
    #  * Verify that last page is removed and view is automatically panned to previous page
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_05_removing_button_from_launcher_with_multiple_pages

        open_launcher

        initialPages = count_launcher_pages

        startIndex = fill_missing_pages

        nextIndex = add_new_full_page(startIndex)

        add_application(nextIndex)

        move_view_to_button(nextIndex)

        verify_true(@@verify_time, "Number of launcher pages is incorrect") { count_launcher_pages == (initialPages + 2) }
        verify_true(@@verify_time, "Number of launcher buttons is incorrect") { count_launcher_buttons == ((initialPages+1)*@@buttons_on_a_page + 1) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(nextIndex) }

        remove_application(nextIndex)

        verify_false(@@verify_time, "Launcher button should not exist") { launcher_button_exist(nextIndex) }
        verify_true(@@verify_time, "Number of launcher pages is incorrect") { count_launcher_pages == (initialPages+1) }
        verify_true(@@verify_time, "Number of launcher buttons is incorrect") { count_launcher_buttons == ((initialPages+1)*12) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that when button is removed from a page, empty space left at the end of the page
    # Verifies that when all applications are removed from a page, that page is removed
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applications so that there is a check page, one full page and a one button on a next page
    #  * Pan to the full page and remove one button from the middle of page
    #  * Verify that correct button is removed and button and page count are correct
    #  * Verify that left buttons are in expected grid formation
    #  * Pan to the last page that has one button
    #  * Remove all buttons from previous page
    #  * Verify that page and button counts are correct
    #  * Verify that panning right move the view back to check page not to empty page
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_06_remove_button_from_middle_of_the_page_with_multiple_pages

        open_launcher

        initialPages = count_launcher_pages

        checkIndex = fill_missing_pages

        startIndex = add_new_full_page(checkIndex)

        nextIndex = add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        add_application(nextIndex)

        verify_true(@@verify_time, "Number of launcher pages is incorrect") { count_launcher_pages == initialPages + 3 }
        verify_true(@@verify_time, "Number of launcher buttons is incorrect") { count_launcher_buttons == ((initialPages+2)*@@buttons_on_a_page + 1) }

        remove_application((startIndex + 5))

        verify_false(@@verify_time, "Launcher button should not exist") { launcher_button_exist(startIndex + 5) }
        verify_true(@@verify_time, "Number of launcher pages is incorrect") { count_launcher_pages == (initialPages + 3) }
        verify_true(@@verify_time, "Number of launcher buttons is incorrect") { count_launcher_buttons == ((initialPages+2)*12) }
        verify_true(@@verify_time, "Lacunher button should be visible") { launcher_button_visible(startIndex) }

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 6],
                                            [7, 8, 9,10,11   ])

        pan_with_gesture(:Left, @@gesture_distance)

        sleep @@gesture_sleep
        tempIndex = startIndex
        @@buttons_on_a_page.times {
            remove_application(tempIndex)
            tempIndex += 1
        }

        # Lastly added button should still be visible on third page, because view is on it
        verify_true(@@gesture_sleep, "Launcher button should be visible") { launcher_button_visible(nextIndex) }

        # View is panned to the right where there page was emptied empty page.
        pan_with_gesture(:Right, @@gesture_distance)

        verify_true(@@gesture_sleep, "Number of launcher pages is incorrect") { count_launcher_pages == (initialPages + 2) }

        # View should be panned "over" the empty page
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(checkIndex) }

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher buttons are placed correctly to launcher pages in portrait mode
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Set orientation to portrait
    #  * Open launcher
    #  * Add applications so that there is one full page
    #  * Verify that buttons are correctly placed in the grid
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_07_launcher_button_order_in_portrait_mode

        ctx = open_context_provider
        set_orientation_portrait(ctx)

        open_launcher

        startIndex = fill_missing_pages

        add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2],
                                            [3, 4, 5],
                                            [6, 7, 8],
                                            [9,10,11] )

        close_launcher

        close_context_provider(ctx)

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher pages can be panned with gestures and faulty gestures are ignored in portrait mode
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Set orientation to portrait
    #  * Open launcher
    #  * Add applications until there is one page full of new applications
    #  * Pan down to that page and verify that applications that page are visible
    #  * Add application to create next page
    #  * Pan to the next page and verify application in that page is visible
    #  * Pan up to the page full of new applications and verify that applications are visible
    #  * Verify that pan gestures to right and left do nothing
    #  * Close launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_08_panning_with_gestures_in_portrait_mode

        ctx = open_context_provider
        set_orientation_portrait(ctx)

        open_launcher

        startIndex = fill_missing_pages

        nextIndex = add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        # Gesture does nothing
        pan_with_gesture(:Up, @@gesture_distance)

        verify_true(@@gesture_sleep, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        add_application(nextIndex)

        pan_with_gesture(:Up, @@gesture_distance)

        verify_true(@@gesture_sleep, "Launcher button should be visible") { launcher_button_visible(nextIndex) }
        verify_false(@@verify_time, "Launcher button should not be visible") { launcher_button_visible(startIndex) }

        pan_with_gesture(:Down, @@gesture_distance)

        verify_false(@@gesture_sleep, "Launcher button should not be visible") { launcher_button_visible(nextIndex) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        pan_with_gesture(:Left, @@gesture_distance)

        verify_false(@@gesture_sleep, "Launcher button should not be visible") { launcher_button_visible(nextIndex) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        pan_with_gesture(:Right, @@gesture_distance)

        verify_false(@@gesture_sleep, "Launcher button should not be visible") { launcher_button_visible(nextIndex) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(startIndex) }

        close_context_provider(ctx)

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that when orientation is changed when launcher is closed the order of the launcher buttons is updated correctly
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applications until there is one page full of new applications
    #  * Pan down to that page and verify that applications are correctly placed to a grid
    #  * Close launcher and rotate screen to portrait mode
    #  * Open launcher and verify that buttons are correctly placed to a portrait grid
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_09_changing_orientation_when_launcher_is_closed

        ctx = open_context_provider

        open_launcher

        startIndex = fill_missing_pages

        add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 5],
                                            [6, 7, 8, 9,10,11] )
        close_launcher

        set_orientation_portrait(ctx)

        open_launcher

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2],
                                            [3, 4, 5],
                                            [6, 7, 8],
                                            [9,10,11] )

        close_context_provider(ctx)

        close_launcher

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that order of buttons in launcher is maintained when duihome is rebooted
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add applicaitons until there is one full page of new buttons
    #  * Verify that buttons are in expected grid formation
    #  * Close and start duihome
    #  * Verify that buttons are in expected grid formation
    #  * Close duihome
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_10_keeping_button_order_when_duihome_is_restarted

        open_launcher

        startIndex = fill_missing_pages

        add_new_full_page(startIndex)

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 5],
                                            [6, 7, 8, 9,10,11] )

        close_duihome

        launch_duihome

        open_launcher

        move_view_to_button(startIndex)

        verify_buttons_in_grid( startIndex, [0, 1, 2, 3, 4, 5],
                                            [6, 7, 8, 9,10,11] )

        close_duihome

    end

    #SubFeature::   DUI-1949 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies that launcher buttons are updated correctly when duihome is rebooted
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open launcher
    #  * Add application with some known test application index
    #  * Close duihome
    #  * Remove "known" application
    #  * Add new application some known text application index
    #  * Re-start duihome
    #  * Verify that removed application doesn't appear in launcher
    #  * Verify that installed application appears in launcher
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_11_updating_buttons_correctly_when_applications_are_added_and_removed_when_duihome_not_running

        open_launcher

        startIndex = fill_missing_pages
        testIndex98 = startIndex + 98
        testIndex99 = startIndex + 99

        add_application(testIndex98)

        move_view_to_button(testIndex98)
        verify_true(@@verify_time, "Launcher button should exist") { launcher_button_exist(testIndex98) }
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(testIndex98) }

        close_duihome

        remove_application(testIndex98)
        add_application(testIndex99)

        launch_duihome

        open_launcher

        verify_false(@@verify_time, "Launcher button should not exist") { launcher_button_exist(testIndex98) }
        verify_true(@@verify_time, "Launcher button should exist") { launcher_button_exist(testIndex99) }

        move_view_to_button(testIndex99)
        verify_true(@@verify_time, "Launcher button should be visible") { launcher_button_visible(testIndex99) }

        close_duihome

    end

private

    def add_apps_until_count(count)
        # add buttons so that total number of buttons is 'count'
        initialApps = count_launcher_buttons
        addedApps = count - initialApps
        addedApps.times { |index|
            add_application(count - (index + 1))
        }
        return addedApps
    end

    # An auxilary function that returns the current count of LauncherButton
    # objects
    def count_launcher_buttons
        pages = count_launcher_pages

        buttons = 0
        pages.times { |index|
            buttons += count_launcher_buttons_on_page(index)
        }

        return buttons
    end

    # An auxilary function that returns the current count of LauncherPage
    # objects
    def count_launcher_pages()
        pagedvw = @app.Launcher.PagedViewport( :objectName => 'LauncherPagedViewport' ).DuiWidget( :objectName => '' ).QGraphicsLayout( :__index => 0 )
        return pagedvw.attribute('count').to_i
    end

    # An auxilary function that returns the current count of LauncherButton
    # objects in LauncherPage
    def count_launcher_buttons_on_page(page_index)
        page = @app.LauncherPage(:__index => page_index)
        return page.QGraphicsLayout.attribute('count').to_i
    end

    # Adds test application to launcher
    # index is indentifier of application
    # return test application name
    def add_application( index )
        number = @@number_format % index
        file_path = @tempPath + "/tc_homelauncher#{number}.desktop"
        add_app_desktop_entry( file_path, index )
        verify_true(@@add_app_verify_time, "Adding application failed") { launcher_button_exist(index) }
    end

    # add test app desktop entry for launcher
    # return current number of test app buttons
    def add_app_desktop_entry( path_to_new_desktop_file, index )
        name = test_app_name(index)
        desktopentry = "[Desktop Entry]\n"\
                       "Type=Application\n"\
                       "Name=#{name}\n"\
                       "Icon=icon-l-video\n"\
                       "Exec=foo\n"\
                       "OnlyShowIn=X-DUI;\n";

        File.open(path_to_new_desktop_file, 'w') {|f| f.write(desktopentry) }
    end

    def remove_application(index)
        number = @@number_format % index
        filePath = @tempPath + "/tc_homelauncher#{number}.desktop"

        begin
            File.delete(filePath)
        rescue
            return false
        end

        return true
    end

    def launcher_is_open
        open = false
        begin
            # TODO: does not yet check VISIBLE
            @app.Launcher()
            open = true
        rescue
        end
        return open
    end

    def launcher_button_visible(index)
        visible = false
        launcherButton = @app.Launcher.LauncherButton( :text => test_app_name(index) )
        begin
            launcherButton.attribute('x')
            visible = true
        rescue
        end
        return visible
    end

    def launcher_button_exist(index)
        exists = false
        begin
            @app.Launcher.LauncherButton( :text => test_app_name(index) )
            exists = true
        rescue
        end
        return exists
    end

    def test_app_name(index)
        number = @@number_format % index
        return "#{@@button_name} #{number}"
    end

    def launcher_button(index)
        @app.Launcher.LauncherButton( :text => test_app_name(index) )
    end

    # Calculates width and height range of button
    # name is the name of the button
    # range is a value that is added and removed from size to get the max and min values of range
    # return range of width and range of height
    def get_button_size_range(name, range)
        width = button_width_by_name(name)
        height = button_height_by_name(name)

        width_max = width + range
        width_min = width - range

        height_max = height + range
        height_min = height - range

        return (width_min ... width_max), (height_min ... height_max)
    end

    # Usage example:
    # verify_buttons_in_grid ( startIndex, [0, 1, 2],
    #                                      [3, 4, 5],
    #                                      [6, 7, 8] )
    # Verifies that buttons are in specified order.
    # Ordering is checked by button indexes.
    # startIndex is the first index of the series, indexes inserted in to grid are added to that
    # So real indexes for buttons are [startIndex +0, startIndex+1, startIndex+2] and so on.
    # FIXME: Doesn't yet check x position of buttons that
    #           are in same column and in different rows
    def verify_buttons_in_grid( startIndex, *buttons )
        if !buttons.empty? then
            rows = buttons.size

            rowButtonY = 0
            buttons.each { |row|
                checkButtonX = 0
                checkButtonY = rowButtonY
                sameRowButtonY = 0

                row.each { |rowIndex|
                    index = startIndex + rowIndex
                    button = LauncherButton.new(test_app_name(index), @app)
                    verify_true(@@verify_time, "Button x position check failed") { button.x > checkButtonX }
                    verify_true(@@verify_time, "Button y position check failed") { button.y > checkButtonY }

                    if sameRowButtonY != 0 then
                        verify_true(@@verify_time, "Buttons on a row are not aligned to same y position") { button.y == sameRowButtonY }
                    end

                    sameRowButtonY = button.y
                    checkButtonX = button.x
                }
                checkButtonY = sameRowButtonY
                rowButtonY = sameRowButtonY
            }

        end
    end

    def close_duihome
        home = @sut.application(:name => 'duihome')
        if home != nil
            pid = home.attribute('processId')
            home.close
            if File.exists?('/proc' + pid + '/status')
                Process.kill(:SIGKILL, pid.to_i)
        end
    end

    def launch_duihome
        begin
            @app = @sut.application(:name => 'duihome')
        rescue
            @app = @sut.run(:name => 'duihome')
        end
    end

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

    def open_launcher(time = @@launcher_open_verification_time)
        if !launcher_is_open then
            @app.DuiButton( :name => 'ToggleLauncherButton' ).tap
        end
        verify_true(time, "Opening launcher failed") { launcher_is_open }
    end

    def close_launcher(time = @@launcher_close_verification_time)
        if launcher_is_open then
            @app.DuiButton( :name => 'ToggleLauncherButton' ).tap
        end
        verify_false(time, "Closing launcher failed") { launcher_is_open }
    end

    # Fills the last page in launcher with buttons
    # Returns the number of buttons added
    def fill_missing_pages
        # The number of applications in launcher when tests are run is not predefined
        # so tests are currently designed to conform to any number of applications
        lastPageIndex = (count_launcher_pages - 1)

        initialApps = count_launcher_buttons_on_page(lastPageIndex)
        missingButtons = @@buttons_on_a_page - initialApps

        # Fill page that might lack buttons so tests can be started from a clean page
        missingButtons.times { |index|
            add_application(index)
        }

        return missingButtons
    end

    # Fills new empty page with buttons
    # Returns next available index
    def add_new_full_page(index)
        @@buttons_on_a_page.times {
            add_application(index)
            index+=1
        }
        return index
    end

    # Moves view so that the button with given index is visible
    def move_view_to_button(startIndex, moves = @@number_of_tries)
        result = false
        moves.times {
            if !launcher_button_visible(startIndex) then
                @app.Launcher.gesture(:Left, 2, 600, :Left)
            else
                result = true
                break
            end
        }
        return result
    end

    def pan_with_gesture(direction, distance)
        @app.Launcher.gesture(direction, @@gesture_duration, distance, @@gesture_button)
    end

end

class LauncherButton
    def initialize(name, app)
        @name = name
        @button = app.Launcher.LauncherButton( :text => @name )
        set_landscape_mode($launcher_landscape_mode)
    end

    def set_landscape_mode(mode)
        if mode then
            @attrb_pos_y = 'y'
            @attrb_pos_x = 'x'
        else
            @attrb_pos_y = 'x'
            @attrb_pos_x = 'y'
        end
    end

    def x
        @button.attribute(@attrb_pos_x).to_i
    end

    def y
        @button.attribute(@attrb_pos_y).to_i
    end

    def width
        @button.attribute('width').to_i
    end

    def height
        @button.attribute('height').to_i
    end

end
