#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: PersistentNotifications test suite functional tests for persistent notifications in duihome
#  * Objectives: test the functionality of persistent notifications
#
#  Author:: Janne Sakko (mailto:janne.sakko@ixonos.com)
#
#  This software, including documentation, is protected by copyright
#  controlled by Nokia Corporation. All rights are reserved. Copying,
#  including reproducing, storing, adapting or translating, any or all of
#  this material requires the prior written consent of Nokia Corporation.
#  This material also contains confidential information which may not be
#  disclosed to others without the prior written consent of Nokia.

require 'fileutils'
require 'test/unit'
require 'matti'
include MattiVerify

#SuiteDomain::            ApplicationFramework
#SuiteTimeout::           3600
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          false
#SetLogFiles::            /logs/testability/tc_persistentnotifications.log
#SetFeature::             DUI-631 in ScrumWorks Pro
#SetTimeout::             3600
#
# These tests verify functionality of persistent notifications.
class TC_PersistentNotifications < Dui::TestCase

    # method called before any test case
    def setup
      @waitForDuihome = 20
      @waitForNotifications = 30

      initialize_and_start_duihome()
    end

    # method called after any test case for cleanup purposes
    def teardown
        # Remove persistent and non-persistent notifications and initialize groups before test cases
        remove_notifications()
        initialize_groups()
    end

 
    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #Test that persistent notifications stay after rebooting duihome
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Add persistent notifications
    #  * Restart duihome
    #  * Check that persistent notifications are loaded correctly at boot
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_01_existence_of_persistent_notifications_after_rebooting_duihome

        # This is required to run only once. So it's run only at the begining of tests.
        remove_notification_manager_dir_from_config()

        #Add 3 persistent notifications
        system("#{$notificationtoolapp} -a add -p -c 1'Event Type' 'Persistent 1' 'Body' 'path_to_image'")
        system("#{$notificationtoolapp} -a add -p -c 1 'Event Type' 'Persistent 2' 'Body' 'path_to_image'")
        system("#{$notificationtoolapp} -a add -p -c 1 'Event Type' 'Persistent 3' 'Body' 'path_to_image'")

        #Verify that notifier object exists
        verify_true(5) { check_notifier_exists }

        #Verify that there is right number of notifications
        verify_equal("3", @waitForNotifications) { @homeapp.Notifier.DuiLabel.attribute('text') }

        restart_duihome()

        verify_true{ check_notifier_exists }

        verify_equal("3") { @homeapp.Notifier.DuiLabel.attribute('text') }

    end

    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Test that when there is persistent and non-persistent notifications, only persistent notifications
    # are loaded after reboot
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Add persistent and non-persistent notifications
    #  * Close duihome
    #  * Launch duihome again
    #  * Check that only persistent notifications are loaded correctly at boot
    #* Post-conditions
    #  * None
    #* References
    #  * None

    def test_02_persistent_notifications_mixed_with_nonpersistent_notifications_after_reboot

        #Add 2 persistent notifications
        system("#{$notificationtoolapp} -a add -p -c 1 'Event Type' 'Persistent 1' 'Body' 'path_to_image'")
        system("#{$notificationtoolapp} -a add -p -c 1 'Event Type' 'Persistent 2' 'Body' 'path_to_image'")

        #Add 2 non-persistent notifications
        system("#{$notificationtoolapp} -a add -c 1 'Event Type' 'Non-persistent 1' 'Body' 'path_to_image'")
        system("#{$notificationtoolapp} -a add -c 1 'Event Type' 'Non-persistent 2' 'Body' 'path_to_image'")

        #Verify that notifier object exists
        verify_true(5) { check_notifier_exists }

        #Verify that there is right number of notifications
        #Showing these notifications can be quite slow process so we give it some time
        verify_equal("4", @waitForNotifications) { @homeapp.Notifier.DuiLabel.attribute('text') }

        restart_duihome()

        #Verify that notifier object exists
        verify_true(5) { check_notifier_exists }

        verify_equal("2", 5) { @homeapp.Notifier.DuiLabel.attribute('text') }
    end

    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Test that persistent groups are loaded after rebooting duihome
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Add persistent groups sms and email
    #  * Add persistent notifications to groups
    #  * Close duihome
    #  * Launch duihome again
    #  * Check that persistent groups are loaded correctly at boot
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_03_persistent_groups_after_rebooting_duihome

        # Create a SMS persistent notification group using duinotificationtool
        system("#{$notificationtoolapp} -a add -g -c 1 -i 1 -p sms '1 new message' 'Test sms group' Icon-messages")
        # Add notification in the group
        system("#{$notificationtoolapp} -a add -i 1 -c 1 -p sms 'You have 1 new message' 'Test sms' Icon-messages")

        # Create EMAIL persistent notification group using duinotificationtool
        system("#{$notificationtoolapp} -a add -g -c 1 -i 2 -p email '1 new emails' 'Test email group' Icon-email")
        # Add notification in the group
        system("#{$notificationtoolapp} -a add -i 2 -c 1 -p email 'You have 1 new email' 'Test emails' Icon-email")

        # Verify that the notifier exists in the status area and shows total number of notifications of both groups
        verify_true(5) { check_notifier_exists }
        verify_equal("2", @waitForNotifications) { @homeapp.Notifier.DuiLabel.attribute('text') }

        restart_duihome()

        verify_true(5) { check_notifier_exists }
        verify_equal("2", 5) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Open the notification area
        verify(0) { @homeapp.Notifier.tap }

        # Check that there are two notification groups shown, one for sms notifications and other for email notifications
        verify_true(5) { check_notification_area_exists }
        verify(0){ @homeapp.DuiInfoBanner(:imageID => 'Icon-messages') }
        verify(0){ @homeapp.DuiInfoBanner(:imageID => 'Icon-email') }

        #Verify that the groups contain the right number of notifications. There should be 1 sms and 1 email
        smsGroupBody = "<p><b>1 new message</b></p><p>Test sms group</p>"
        emailGroupBody = "<p><b>1 new emails</b></p><p>Test email group</p>"
        verify_equal(smsGroupBody, 0) { @homeapp.DuiInfoBanner(:imageID => 'Icon-messages').DuiLabel.attribute('text') }
        verify_equal(emailGroupBody, 0) { @homeapp.DuiInfoBanner(:imageID => 'Icon-email').DuiLabel.attribute('text') }
   end

    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Test that removed persistent notifications are not loaded after reboot
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Add persistent notification
    #  * Close duihome
    #  * Launch duihome again
    #  * Check that persistent notifications are not loaded at boot
    #* Post-conditions
    #  * None
    #* References
    #  * None

    def test_04_remove_by_tapping_persistent_notifications

        system("#{$notificationtoolapp} -a add -p -c 1 'Event Type' 'Persistent 1' 'To be removed' Icon-email")

        verify_true(5) { check_notifier_exists }
        verify_equal("1", @waitForNotifications) { @homeapp.Notifier(:name => 'Notifier').DuiLabel.attribute('text') }

        # Open the notification area
        verify(0) { @homeapp.Notifier.tap }

        # Remove notification
        verify(5) { @homeapp.NotificationArea.DuiInfoBanner.tap }

        # Verify that notifications are removed
        verify_false(5) { check_notifier_exists }

        restart_duihome()

        # Verify that there are no notifications loaded from persistent storage
        # We give duihome some time to boot fully
        verify_false(5) { check_notifier_exists && check_notification_area_exists && check_infobanner_exists }
    end

    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Test that persistent notifications in groups are loaded after reboot
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Add sms group
    #  * Add persistent notification to sms group
    #  * Check that notification exists
    #  * Remove notification
    #  * Close duihome
    #  * Launch duihome again
    #  * Check that persistent notifications are not loaded at boot
    #* Post-conditions
    #  * None
    #* References
    #  * None

    def test_05_remove_persistent_notification_in_group

        # Create a SMS persistent notification group using duinotificationtool
        system("#{$notificationtoolapp} -a add -g -i 1 -c 1 -p sms '1 new message' 'Test sms group' Icon-messages")
        # Add notification in the group
        system("#{$notificationtoolapp} -a add -i 1 -c 1 sms 'You have 1 new message' 'Test sms' Icon-messages")

        # Verify that the notifier exists in the status area and shows total number of notifications of both groups
        verify_true() { check_notifier_exists }
        verify_equal("1", 40) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Open the notification area
        verify(0) { @homeapp.Notifier.tap }

        # Remove notification
        verify(20) { @homeapp.NotificationArea.DuiInfoBanner.tap }

        # Check that notifier is non-existent
        verify_false(5) { check_notifier_exists }

        restart_duihome()

        verify_false(10) { check_notifier_exists }
    end

    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Test that non-persistent notifications added to persistent group are persistent after reboot
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Create persistent group
    #  * Insert non-persistent notification to group
    #  * Restart duihome
    #  * Check that notification in persistent group is loaded correctly at startup
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_06_add_non_persistent_notification_to_persistent_group

        # Create a SMS persistent notification group using duinotificationtool
        system("#{$notificationtoolapp} -a add -g -i 1 -p sms 'New sms group' 'Test sms group' Icon-messages")
        verify_true(20) { !check_notifier_exists }
        # Add notification in the group
        system("#{$notificationtoolapp} -a add -i 1 sms 'You have 1 new message' 'Test sms' Icon-messages")

        # Verify that the notifier exists in the status area and shows total number of notifications of both groups
        verify_true(20) { check_notifier_exists }
        verify_equal("1", 5) { @homeapp.Notifier.DuiLabel.attribute('text') }

        restart_duihome()

        verify_true(20) { check_notifier_exists }
        verify_equal("1", 5) { @homeapp.Notifier.DuiLabel.attribute('text') }
    end

    #SubFeature::   DUI-631 in ScrumWorks Pro
    #Timeout::      200
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Test getting list of notifications that belong to a user notification id
    #
    #* Pre-conditions
    #  * dbus daemon is running
    #  * duinotificationtool is installed
    #  * duihome is installed
    #  * no notifications are already displayed
    #* Test steps
    #  * Launch duihome
    #  * Create few notifications with common id
    #  * Create few notification with different id for compare
    #  * Call notficationsIdList through duinotificationtool
    #  * Check that the right amount of notifications are returned
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_07_get_list_of_notifications_for_notification_user_id

        # Create 5 notifications
        system("#{$notificationtoolapp} -a add sms 'You have new message' 'Test sms' Icon-messages")
        system("#{$notificationtoolapp} -a add sms 'You have new message' 'Test sms' Icon-messages")
        system("#{$notificationtoolapp} -a add sms 'You have new message' 'Test sms' Icon-messages")
        system("#{$notificationtoolapp} -a add sms 'You have new message' 'Test sms' Icon-messages")
        system("#{$notificationtoolapp} -a add sms 'You have new message' 'Test sms' Icon-messages")

        verify_true(5) { check_notifier_exists }
        verify_equal("5", @waitForNotifications) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Use duinotificationtool's list handle to query notifications that belong to duinotificationtool
        # duinotificationtool returns count of notifications that belong to specified application.
        system("#{$notificationtoolapp} -l duinotificationtool")
        # Get exit status of duinotificationtool
        result = $?.exitstatus

        verify_equal(5, 10) { result }
    end
    
private

   # Ensure that there is no notifications stored before the tests start
    def remove_notification_manager_dir_from_config
        puts "Remove NM"
        homePath = ENV['HOME']
        notificationManagerPath = "#{homePath}/.config/duihome/notificationmanager"
        puts notificationManagerPath
        FileUtils.rm_r(notificationManagerPath) if File::exists?(notificationManagerPath)
    end

    # Remove persistent and non-persistent notifications with duinotificationtool
    def remove_notifications

        if check_notifier_exists
            @homeapp.Notifier.tap

            i = 1
            while notifications_in_notifier > 0 do

                system("#{$notificationtoolapp} -a remove -i #{i}")
                i += 1

            end

        end
    end

    # We remove groups that might have left from previous tests
    # We are only removing groups from id range we are going to use
    # and that might interfere with our tests
    def initialize_groups
        i = 1
        # removes ids 1-5
        amount_of_notification_groups_used = 6
        while i < amount_of_notification_groups_used do
           system("#{$notificationtoolapp} -a remove -g -i #{i}")
           i += 1
        end
    end

    # Returns number of notifications marked on notifier
    def notifications_in_notifier
        # If notifier doesn't exist anymore return 0
        if check_notifier_exists
            return Integer(@homeapp.Notifier.DuiLabel.attribute("text"))
        else
            return 0
        end
    end

    def isRunning(pid)
        if pid != 0
            return File.exists?( "/proc/"+pid+"/status" )
        end
        return false
    end

    # Returns true if notifier exists, false otherwise.
    def check_notifier_exists
        return @homeapp.test_object_exists?("Notifier")
    end

    # Returns true if notification area exists, false otherwise.
    def check_notification_area_exists
        return @homeapp.test_object_exists?("NotificationArea")
    end

    # Returs true if info banner exists in home main window, false otherwise
    def check_infobanner_exists
        return @homeapp.MainWindow.test_object_exists?("DuiInfoBanner")
    end

    def start_duihome
        begin
            puts "Connect to running duihome"
            @homeapp = @sut.application(:name => 'duihome')
        rescue
            puts "Not running ... Start duihome"
            @homeapp = @sut.run(:name => 'duihome', :arguments => '-software')
        end
    end

    def check_duihome_running
        if @homeapp != nil
          procId = 0
          procId = @homeapp.attribute('processId')
          if procId != 0
            return isRunning(procId)
          end
        end
        return false
    end

    def initialize_and_start_duihome

        start_duihome()

        #Verify that no notifications or notificationarea exist on duihome
        verify_false(5) {@homeapp.test_object_exists?("Notifier") }
        verify_false(5) {@homeapp.test_object_exists?("NotificationArea") }
        verify_false(5) {@homeapp.MainWindow.test_object_exists?("DuiInfoBanner") }

    end

    def restart_duihome

        #Close duihome
        @homeapp.close 

        start_duihome()

        verify_true(@waitForDuihome) {
          check_duihome_running()
        }

    end

end
