# -*- coding: utf-8 -*-
#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: NotificationSystem test suite functional cases for Notification System end-to-end functionality
#  * Objectives: Test the end-to-end use cases of Notification System. Exercise both libdui and duihome.
#
#  Author:: Tuomas Järvensivu (mailto:tuomas.jarvensivu@nokia.com)
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
#SuiteTimeout::           3600
#SuiteType::              Functional

#SetHardware::            true
#SetScratchbox::          false
#SetFeature::             DUI-239
#SetTimeout::             1800
#
# This test suite tests the functionality of notification subsytem.
# There are two types of notifications events. Application events and Sytem events.
# Application events are sent over the DBus. System notifications are sent by trusted plugins only
# which are preloaded by home. Notifications are collected into groups(sms, email etc).
# Notifications are shown in info banners, which disappear automatically after a set timeout.
# Notifications then reside into the notification area. There is also a notifier which displays
# number of notifications currently not acted upon. When the status area is tapped, a
# motification area is displayed which has all the current notifications. On clicking any notification
# an action for that notification is triggered and the notification is dismissed.

class TC_NotificationSystem < Dui::TestCase

    # method called before any test case
    def setup
        @homeapp = @sut.application(:name => 'duihome')

	sleep 5
        # Clear D-Bus message array
        testDBusService.messages.clear

        # Verify that notification notifier is not visible in home and no notification area exists
        verify_false(0) { check_notifier_exists && check_notification_area_exists }
        verify_false(0) { check_infobanner_exists || @homeapp.test_object_exists?("QGraphicsView") }
    end

    def count_infobanners
        @homeapp.Notifier.tap
        count = 0
        begin
	    banner = @homeapp.MainWindow.NotificationArea.DuiInfoBanner(:__index => count)
	    count = count + 1
	rescue NoMethodError, MobyBase::TestObjectNotFoundError
	    banner = nil
	end while banner
	return count
    end

    # method called after any test case for cleanup purposes
    def teardown
        if(!check_notifier_exists)
            return
        end
        noOfBanners = count_infobanners
        i = 0
        @homeapp.Notifier.tap
        while i <  noOfBanners do
            @homeapp.MainWindow.NotificationArea.DuiInfoBanner(:__index => 0).tap
            i= i+1
        end
    end



    #SubFeature::   DUI-239
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component

    # Verifies that the notification area and the notification notifier work.
    #
    #* Pre-conditions
    #  * /usr/bin/duihome started
    #  * There are no notifications in home
    #  * duinotificationtool exists
    #* Test steps
    #  * Create a new notification using duinotificationtool
    #  * Verify that the notifier exists
    #  * Open the notification area
    #  * Verify that the notification area opened and the notification is visible
    #  * Close the notification area by tapping outside it
    #  * Verify that the notification area closed
    #  * Reopen the notification area
    #  * Tap the notification
    #  * Verify that the notification area is closed and the notifier is not visible
    #* Post-conditions
    #  * The notification was dismissed, the notification area is closed and the notifier is not visible

    def test_notificationarea_interaction
        # Create a new notification using duinotificationtool
        system("#{$notificationtoolapp} -a add press 'Test' 'Notification' image.png")

        # Verify that the notifier exists in the status area and shows one notification inside it.
        # Timeout of 5 seconds is given to make sure notification animates and updates the notifier
        verify_true(5) { check_notifier_exists && !check_notification_area_exists }

        verify_equal("1", 0) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Open the notification area
        verify(0) { @homeapp.Notifier.tap }

        # Verify that notification area is opened and has a notification inside it.
        verify_true(5) { check_notification_area_exists && check_infobanner_exists }

        # Tap outside the notification area and the area should close
	    verify { @homeapp.Desktop.tap_object(1,400) }
        verify_false(5) { check_notification_area_exists }
        verify_true(0) { check_notifier_exists }


        # Open the notification area again
        verify(0) { @homeapp.Notifier.tap }

        # Verify that it has opened again and has the notification
        verify_true(5) { check_notification_area_exists && check_infobanner_exists }

        # Tap the info banner
        verify(0) { @homeapp.NotificationArea.DuiInfoBanner.tap }

        # Verify that info banner is removed when its tapped and the notification area is closed
        verify_false(5) { check_notifier_exists && check_notification_area_exists && check_infobanner_exists }
    end

    #SubFeature::   DUI-239
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component

    # Verifies adding group, adding notifications in a group, group updation functionality
    #
    #* Pre-conditions
    #  * /usr/bin/duihome started
    #  * There are no notifications in home
    #  * duinotificationtool exists
    #* Test steps
    #  * Create a new notification group using duinotificationtool
    #  * Add notification in the group
    #  * Verify that the notifier exists in the status area and shows one notification inside it.
    #  * Add more notifications to the group and update the group
    #  * Verify that notifier exists and count is updated
    #  * Create another notification group using duinotificationtool
    #  * Verify that the notifier exists in the status area and shows total number of notifications of both groups
    #  * Add more notifications to the second group and update the group
    #  * Verify that notifier exists and count is updated
    #  * Open the notification area
    #  * Check that there are two notification groups shown for each defined event type
    #  * Verify that the groups contain the right number of notifications.
    #  * ap on the notification groups and verify that notification area as well as notifier is removed.
    #* Post-conditions
    #  * The notification was dismissed, the notification area is closed and the notifier is not visible
    def test_notification_count_with_groups

        # Create a new notification group using duinotificationtool
        system("#{$notificationtoolapp} -a add -g -c 1 sms '1 new message' 'Test sms group' Icon-messages")
        @groupid = $?.exitstatus
        # Add notification in the group
        system("#{$notificationtoolapp} -a add -i #{@groupid} -c 1 sms 'You have 1 new message' 'Test sms' Icon-messages")

        # Verify that the notifier exists in the status area and shows one notification inside it.
        verify_true(5) { check_notifier_exists && !check_notification_area_exists }
        verify_equal("1", 0) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Add more notifications to the group and update the group
        system("#{$notificationtoolapp} -a add -i #{@groupid} -c 3 sms 'You have 3 new message' 'Test sms' Icon-messages")
        system("#{$notificationtoolapp} -a update -g -i #{@groupid} -c 4 sms 'You have 4 new message' 'Test sms' Icon-messages")

        # Verify that notifier exists and count is updated
        verify_true(0) { check_notifier_exists }
        verify_equal("4", 5) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Create another notification group using duinotificationtool
        system("#{$notificationtoolapp} -a add -g -c 2 email '2 new emails' 'Test email group' Icon-email")
        @groupid2 = $?.exitstatus
        # Add some notifications in the group
        system("#{$notificationtoolapp} -a add -i #{@groupid2} -c 2 email 'You have 2 new emails' 'Test emails' Icon-email")

        # Verify that the notifier exists in the status area and shows total number of notifications of both groups
        verify_true(5) { check_notifier_exists && !check_notification_area_exists }
        verify_equal("6", 5) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Add more notifications to the second group and update the group
        system("#{$notificationtoolapp} -a add -i #{@groupid2} -c 5 email 'You have 3 new emails' 'Test emails' Icon-email")
        system("#{$notificationtoolapp} -a update -g -i #{@groupid2} -c 7 email 'You have 4 new emails' 'Test emails' Icon-email")

        # Verify that notifier exists and count is updated
        verify_true(0) { check_notifier_exists }
        verify_equal("11", 0) { @homeapp.Notifier.DuiLabel.attribute('text') }

        # Open the notification area
        verify(0) { @homeapp.Notifier.tap }

        # Check that there are two notification groups shown, one for sms notifications and other for email notifications
        verify_true(5) { check_notification_area_exists }
        verify_true(0) { @homeapp.MainWindow.NotificationArea.test_object_exists?("DuiInfoBanner", { :imageID => 'Icon-messages' }) }
        verify_true(0) { @homeapp.MainWindow.NotificationArea.test_object_exists?("DuiInfoBanner", { :imageID => 'Icon-email' }) }

        #Verify that the groups contain the right number of notifications. There should be 4 sms and 7 emails
        smsGroupBody = "<p><b>You have 4 new message</b></p><p>Test sms</p>"
        emailGroupBody = "<p><b>You have 4 new emails</b></p><p>Test emails</p>"
        verify_equal(smsGroupBody, 0) { @homeapp.MainWindow.NotificationArea.DuiInfoBanner(:imageID => 'Icon-messages').attribute('bodyText') }
        verify_equal(emailGroupBody, 0) { @homeapp.MainWindow.NotificationArea.DuiInfoBanner(:imageID => 'Icon-email').attribute('bodyText') }

        # Tap on the notification groups and verify that notification area as well as notifier is removed.
        verify(0) { @homeapp.MainWindow.NotificationArea.DuiInfoBanner(:imageID => 'Icon-messages').tap }
        verify(0) { @homeapp.MainWindow.NotificationArea.DuiInfoBanner(:imageID => 'Icon-email').tap }
        verify_false(5) { check_notifier_exists && check_notification_area_exists && check_infobanner_exists }
    end

    #SubFeature::   DUI-1223
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component

    # Verifies that D-Bus actions with notifications work.
    # Verifies that a new DuiInfoBanner is shown on duihome when a notification is
    # added. Verifies that when a DuiInfoBanner is tapped the configured action
    # is triggered which leads into D-Bus call to the set D-Bus interface.
    #
    #* Pre-conditions
    #  * D-Bus session bus is set-up
    #  * /usr/bin/duihome started
    #  * There are no notifications in home
    #  * duinotificationtool exists
    #* Test steps
    #  * Create a new notification using duinotificationtool
    #  * Verify that a new DuiInfoBanner is created to the duihome NotificationArea
    #  * Tap the new DuiInfoBanner on the duihome NotificationArea
    #  * Verify that the configured D-Bus call is made
    #* Post-conditions
    #  * sendMessage() D-Bus interface exposed from this testsuite is called
    #* References
    #  * DUI-1223
    def test_sending_and_triggering_notification

        # Create a new notification
        logMessage = "notificationTriggered"
        system("#{$notificationtoolapp} -a add press 'This notification sends' '#{logMessage} to sendMessage() D-Bus interface.' image.png org.maemo.dui.test.NotificationSystemTester / org.maemo.dui.test.NotificationSystemTesterInterface sendMessage '#{logMessage}'")

        # Open the notification area
        verify(5) { @homeapp.Notifier.tap }

        # Verify that there's one DuiInfoBanner in the NotificationArea
        verify_true(5) { check_infobanner_exists }

        # Tap the info banner
        verify { @homeapp.NotificationArea.DuiInfoBanner.tap }

        # Verify that info banner is removed when its tapped
        verify_false(1) { check_infobanner_exists }

        # Verify that D-Bus call was made when notification was tapped and that
        # correct message was sent to D-Bus service exposed from this testsuite.
        verify_equal([logMessage]) {
            testDBusService.messages
        }
    end

     #SubFeature::   DUI-1883
     #Timeout::      100
     #Type::         Functional
     #Requirement::  NONE
     #Manual::       false
     #Level::        Component

     # Test system notifications
     # Verifies that system notification are shown when a system notification is spawned
     # Fake plugin has a file system watcher which watches /tmp/systemnotifications for changes.
     # When a change is detected a system notification is spawned.
     #* Pre-conditions
     #  * /usr/lib/duihome/systemnotifications/fakesystemnotificationsplugin.so exists
     #  * /usr/bin/duihome started
     #  * There are no notifications in home
     #* Test steps
     #  * Create a new notification using touch on file system
     #  * Verify that a new DuiInfoBanner is visible
     #  * cleanup the file system. Note this generates another system notification
     #* Post-conditions
     #  * - The notification was dismissed, no notification info banner exists.
     #* References
     #  * DUI-1883

     def test_SystemNotifications_Notification_Banner_Appears

       create_system_notification

       verify_true(5) { check_infobanner_exists  }

       verify { @homeapp.MainWindow.DuiInfoBanner.tap }

       verify_false(5) { check_infobanner_exists }

       cleanup_system_notification

       # Removing the dummy file above in cleanup, spawns a system notification again.
       # Tapping this notificaion to remove it from screen, so that other tests are not
       # fudged.
       @homeapp.MainWindow.DuiInfoBanner.tap
       verify_false(1) { check_infobanner_exists }
     end

    #SubFeature::   DUI-1445
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Component
    #
    # Test notifications during video recording
    # Verifies that notification are not shown when video recording is on.
    # Verifies that new notifications are not added to homes notification are
    # when area is visible. Verifies that system notitifications are shown in full screen case.
    #* Pre-conditions
    #  * Open3 is installed (package libipc-run3-perl)
    #  * /usr/lib/duihome/systemnotifications/fakesystemnotificationsplugin.so exists
    #  * context-provide is installed (from package contextkit-utils)
    #  * D-Bus session bus is set-up
    #  * /usr/bin/duihome started
    #  * There are no notifications in home
    #  * duinotificationtool exists
    #* Test steps
    #  * Create a new notification using duinotificationtool
    #  * Verify that a new DuiInfoBanner is visible
    #  * Change context-property User.Mode to 'recording'
    #  * Create a new notification using duinotificationtool
    #  * Verify that a new DuiInfoBanner is NOT visible
    #  * Launch an application
    #  * Verify that a new DuiInfoBanner is NOT visible
    #  * Change context-property User.Mode to 'normal'
    #  * Create a new notification using duinotificationtool
    #  * Verify that a new DuiInfoBanner is visible again
    #* Post-conditions
    #  * -
    #* References
    #  * DUI-1445
    def test_No_Notifications_During_Video_Recording

      cmd = "context-provide"
      cmd_in, cmd_out, cmd_err = Open3::popen3(cmd)

      # test that notification come when Use.Mode is some normal mode
      cmd_in.puts "add string Use.Mode normal"
      system("#{$notificationtoolapp} -a add press 'Test' 'Notification' image.png")
      verify_true(5) { check_infobanner_exists  }

      # Open the notification area
      verify(5) { @homeapp.Notifier.tap }

      # Tap the info banner
      verify { @homeapp.NotificationArea.DuiInfoBanner.tap }

      # check that notification is gone
      verify_true(10) { !check_infobanner_exists  }

      # test that notification DOESN'T come when Use.Mode is recording
      cmd_in.puts "Use.Mode=recording"
      system("#{$notificationtoolapp} -a add press 'This shouldnt be visible' 'Notification' image.png")
      verify_true(1) { !check_infobanner_exists  }

      # check that notification was added to notification area still
      verify_equal("1", 10) { @homeapp.Notifier.DuiLabel.attribute('text') }
      verify(5) { @homeapp.Notifier.tap }
      verify { @homeapp.NotificationArea.DuiInfoBanner.tap }

      # test that notification DOESN'T come when Use.Mode is recording and application is shown
      @homeapp.DuiButton(:name => 'ShowLauncherButton').tap
      verify(5){ @homeapp.LauncherButton(:text => 'Demos').tap }
      verify(5){ @homeapp.LauncherButton(:text => 'Widgets Gallery').tap }
      system("#{$notificationtoolapp} -a add press 'This shouldnt be visible' 'Notification' image.png")
      @notificationid2 = $?.exitstatus
      verify_true(1) { !check_infobanner_exists  }
      system("#{$notificationtoolapp} -a remove -i #{@notificationid2}")
      @homeapp.DuiButton(:name => 'ShowLauncherButton').tap

      # test that system notifications are still(always) shown.
      test_SystemNotifications_Notification_Banner_Appears

      # test that normal notifications are still shown when Use.Mode != recording and application is shown
      cmd_in.puts "Use.Mode=normal"
      system("#{$notificationtoolapp} -a add press 'Test' 'Notification' image.png")
      verify_true(5) { check_infobanner_exists  }

      # Open the notification area
      verify(5) { @homeapp.Notifier.tap }

      # Tap the info banner
      verify { @homeapp.NotificationArea.DuiInfoBanner.tap }

      verify_true(10) { !check_infobanner_exists  }

      # close streams
      cmd_in.close
      cmd_out.close
      cmd_err.close

      sleep 1
    end

private 
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

    # Creates a system notification by creating a dummy file.
    def create_system_notification
       @sut.execute_shell_command("touch /tmp/systemnotification/a")
    end

    # Deletes the dummy file, this will again trigger a system notification.
    def cleanup_system_notification
       @sut.execute_shell_command("rm -rf /tmp/systemnotification/a")
    end
    # Test updating notification
    #   - inside notificationarea
    #   - outside notificationarea
    # Test removing notification
    #   - from home main window
    #   - from compositor window
    # Test triggering groups

end
