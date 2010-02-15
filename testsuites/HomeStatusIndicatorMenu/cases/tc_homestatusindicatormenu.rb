#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: HomeStatusIndicatorMenu test suite functional cases for duistatusindicatormenu application.
#  * Objectives: test the functionality of duistatusindicatormenu application.
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


    #SuiteDomain::            Application framework
    #SuiteTimeout::           600
    #SuiteType::              Functional
    #
    #SetHardware::            true
    #SetScratchbox::          true
    #SetFeature::             
    #SetTimeout::             10
    class TC_HomeStatusIndicatorMenu < Dui::TestCase

    # method called before any test case
    def setup
        @duihome = @sut.application(:name => 'duihome')
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
    # Verifies application startup from duihome. Swiping the status bar makes the status indicator menu appear
    #
    #* Pre-conditions
    #  * None.
    #* Test steps
    #  * Swipe the status bar
    #  * Verify that duistatusindicatormenu application is started and is in the foreground
    #  * Tap the close button on the status indicator menu
    #  * Check that duihome is back as the top application
    #* Post-conditions
    #  * None 
    #* References
    #  * None.

    def test_swipe_statusbar_launches_statusindicatormenu_and_closing_using_cross_button
		@duihome.StatusArea.flick(:Down)
   
        # Verify that duistatusindicatormenu application is started and is in the foreground
        verify_equal("duistatusindicatormenu", 10) { @sut.application.attribute('objectName') }
		verify_equal("duistatusindicatormenu", 10) { @sut.application.attribute('objectName') }
	
		@duiStatusIndicatorMenuApplication = @sut.application(:name=> 'duistatusindicatormenu')
		# Close the status indicator menu
		@duiStatusIndicatorMenuApplication.DuiButton(:name => 'CloseButton').tap
        # Check that application is (closed)minimized after clicking on the close button and home is back as top
		verify_equal("duihome", 10) { @sut.application.attribute('objectName') }    
	end
    
    #SubFeature::   -
    #Timeout::      100
    #Type::         Functional
    #Requirement::  NONE
    #Manual::       false
    #Level::        Feature
    # Verifies the functionality of settings button of the duistatusindicatormenu application
    #
    #* Pre-conditions
    #  * duihome started 
    #* Test steps
    #  * Flick the status bar to open the status indicator menu
    #  * Click on the settings button
    #  * Check that the duicontrolpanel application is started and is in the foreground
    #  * Close the control panel application
    #* Post-conditions
    #  * Control panel application is closedi and duihome is back on top.
    #* References
    #  * None.
    def test_settings_button_functionality
        @duihome.StatusArea.flick(:Down)

        @duiStatusIndicatorMenuApplication = @sut.application(:name=> 'duistatusindicatormenu')
	    # Check settings button is present, and then tap it.
	    settingsButton = @duiStatusIndicatorMenuApplication.DuiButton(:name => 'StatusIndicatorMenuSettingsButton')
	    verify(0) { settingsButton.tap }

        # Check that the controlpanel application is started and is in the foreground  
	    verify_equal("duicontrolpanel", 10) { @sut.application.attribute('objectName') }

        # close the control panel application
	    @sut.application.close

   	    # Back to duihome
	    verify_equal("duihome", 10) { @sut.application.attribute('objectName') }                                
    end
end
