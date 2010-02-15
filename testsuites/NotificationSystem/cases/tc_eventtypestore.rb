#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: SuiteName test suite functional cases for Awidget
#  * Objectives: test the functionality of Awidget
#
#  Author:: Your Name (mailto:your.name@nokia.com)
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

#SuiteDomain::            Check test.xml definition from pmo wiki
#SuiteTimeout::           3600
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          false
#SetFeature::             DUI-1672 in ScrumWorks Pro
#SetTimeout::             3600
#
# Thes test suite verifies that the eventype system is working and can be extended
# with custom/ 3rd party event types as well.
class TC_DuiEventTypeStore < Dui::TestCase

    # method called before any test case
    def setup
      @homeapp = @sut.application(:name => 'duihome')
    end

    # method called after any test case for cleanup purposes
    def teardown
    end

    #SubFeature::   DUI-1672 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #Tests that the event type store can provide correct icon ids when a predefined event
    #type is used
    #
    #* Pre-conditions
    #  * duinotificationtool available
    #* Test steps
    #  * send message with valid event type with duinotificationtool
    #  * tap on notifier compoent to bring out the dui info banner
    #  * verify that the dui info banner contains proper iconId.
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_predefined_event_type_id_pass
      system("#{$notificationtoolapp} -a add email 'Test' 'Notification' foo")
      verify_true(5) {
        @homeapp.test_object_exists?("Notifier", {:name => 'Notifier'})
      }

      @notifier = @homeapp.Notifier()
      @notifier.tap()

      verify_true(5) {
        @homeapp.test_object_exists?("DuiPannableViewport", {:name => 'NotificationAreaViewport', :enabled => 'false'})
      }

      verify_true(1) {
        @homeapp.test_object_exists?("DuiLabel", {:name => 'EventInfoBannerBody', :enabled => 'true'})
      }

      verify_equal("<p><b>Test</b></p><p>Notification</p>", 1, "Wrong text in infobanner") {
        @homeapp.DuiLabel(:name => "EventInfoBannerBody").attribute("text")
      }


      verify_equal("icon-l-email", 1, "Wrong icon in info banner") {
        @homeapp.DuiInfoBanner.attribute("iconId")
      }
      @homeapp.DuiInfoBanner.tap()
      sleep 1
    end


    #SubFeature::   DUI-1672 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #Tests that the event type store can provide the default icon id when the given
    #event type is defined in the system
    #
    #* Pre-conditions
    #  * duinotificationtool available
    #* Test steps
    #  * send message with valid event type with duinotificationtool
    #  * tap on notifier compoent to bring out the dui info banner
    #  * verify that the dui info banner contains proper iconId.
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_default_icon_id_pass
      system("#{$notificationtoolapp} -a add foo-bar 'Notification that' 'should use default icon' foo")
      verify_true(5) {
        @homeapp.test_object_exists?("Notifier", {:name => 'Notifier'})
      }

      @notifier = @homeapp.Notifier()
      @notifier.tap()

      verify_true(5) {
        @homeapp.test_object_exists?("DuiPannableViewport", {:name => 'NotificationAreaViewport', :enabled => 'false'})
      }

      verify_true(1) {
        @homeapp.test_object_exists?("DuiLabel", {:name => 'EventInfoBannerBody', :enabled => 'true'})
      }

      verify_equal("<p><b>Notification that</b></p><p>should use default icon</p>", 1, "Wrong text in infobanner") {
        @homeapp.DuiLabel(:name => "EventInfoBannerBody").attribute("text")
      }

      verify_equal("default", 1, "Wrong icon in info banner") {
        @homeapp.DuiInfoBanner.attribute("iconId")
      }
      @homeapp.DuiInfoBanner.tap()
      sleep 1
    end


    #SubFeature::   DUI-1672 in ScrumWorks Pro
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    #Tests that the event type store can provide the custom icon id
    #
    #* Pre-conditions
    #  * duinotificationtool available
    #* Test steps
    #  * send message with a cusomt event type with duinotificationtool
    #  * tap on notifier compoent to bring out the dui info banner
    #  * verify that the dui info banner contains proper iconId.
    #* Post-conditions
    #  * None
    #* References
    #  * None
#     def test_custom_event_type_icon_id_pass
#       @custom_event_file = "/usr/share/dui/notifications/eventtypes/my-custom-event.conf"
#       @line = "iconId=my-custom-icon-id"
#       File.open(@custom_event_file, 'w') {|f| f.write(@line) }

#       system("#{$notificationtoolapp} -a add my-custom-event 'Custom' 'notification' foo")
#       verify_true(5) {
#         @homeapp.test_object_exists?("Notifier", {:name => 'Notifier'})
#       }

#       @notifier = @homeapp.Notifier()
#       @notifier.tap()

#       verify_true(5) {
#         @homeapp.test_object_exists?("DuiPannableViewport", {:name => 'NotificationAreaViewport', :enabled => 'false'})
#       }

#       verify_true(1) {
#         @homeapp.test_object_exists?("DuiLabel", {:name => 'EventInfoBannerBody', :enabled => 'true'})
#       }

#       verify_equal("<p><b>Custom</b></p><p>notification</p>", 1, "Wrong text in infobanner") {
#         @homeapp.DuiLabel(:name => "EventInfoBannerBody").attribute("text")
#       }


#       verify_equal("my-custom-icon-id", 1, "Wrong icon in info banner") {
#         @homeapp.DuiInfoBanner.attribute("iconId")
#       }

#       @homeapp.DuiInfoBanner.tap()
#       sleep 1
#       File.delete(@custom_event_file)
#     end

end
