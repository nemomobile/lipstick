#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: DuiAppletInstallationThroughAppletLibrary test suite
#  * Objectives: Test feature: Installing one applet from applet library
#
#  Author: Sachin Kundu (mailto:ext-sachin.kundu@nokia.com)
#
#  This software, including documentation, is protected by copyright
#  controlled by Nokia Corporation. All rights are reserved. Copying,
#  including reproducing, storing, adapting or translating, any or all of
#  this material requires the prior written consent of Nokia Corporation.
#  This material also contains confidential information which may not be
#  disclosed to others without the prior written consent of Nokia.

require 'matti'
include MattiVerify

#SuiteDomain::            Application framework
#SuiteTimeout::           600
#SuiteType::              Functional
#
#SetHardware::            true
#SetScratchbox::          true
#SetFeature::             SWP#DUI-1602
#SetTimeout::             10
#
# Tests opening applets using the applet library

# This test suite tests the functionality of applets
class TC_HomeAppletTests < Dui::TestCase
    # Removes applets from the canvas one by one as long as they exist
    def removeAllApplets
        begin
            while @app.DuiMashupCanvas.DuiContainer(:__index => 0, :__timeout => 0)
                @app.DuiMashupCanvas.DuiContainer(:__index => 0).long_tap
                begin
                    @app.DuiButton(:text => 'Remove Applet', :__timeout => 2).tap
                rescue
                    @app.DuiButton(:text => '!! Remove Applet', :__timeout => 2).tap
                end
            end
        rescue
        end
    end

    # method called before any test case
    def setup
        # get the application
        @app = @sut.application(:name => 'duihome')

        # Clear any existing applets from the canvas
        removeAllApplets
    end

    # method called after any test case for cleanup purposes
    def teardown
    end

    # Pans the applet library to make the button with the text passed as an argument
    # to become visible.
    def scrollTo(buttonToScrollTo)
        # Scroll the applet library so that the requested installation source button becomes visible
        until @app.DuiAppletButton(:text => buttonToScrollTo).attribute('visible') == 'true'
            @app.DuiModalSceneWindow(:name => 'DuiAppletInventoryWindow').DuiPannableViewport.gesture(:Up, 0.5, 200)
        end
    end


    #SubFeature::   SWP#DUI-1602
    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Instantiating installed applets from the applet library
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Accesses applet library
    #  * Flick to find the weather applet
    #  * Selects the weather applet
    #  * Verifies weather applet was loaded
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_1_instantiate_applet_from_applet_library
        # Action 1: User opens applet library
        verify(20) {@app.DuiButton(:name => 'DuiAppletInventoryButton', :__timeout => 1).tap}

	scrollTo("Weather")
        
	# Instantiate Weather applet
        verify(20) {@app.DuiAppletButton(:text => 'Weather').tap}
        
	# Verify there is an applet on home screen
	verify(20) {@app.DuiContainer(:title=> 'Weather today')}
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Opening of applet object menu using long tap on applets
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Instantiate weather applet by calling test_1_instantiate_applet_from_applet_library
    #  * Long tap on applet container
    #  * Verify that object menu appears
    #  * Verify default actions are present in the object menu
    #  * Perform an action assigned to the buttons
    #  * Verify actions assigned to object menu button works(remove applet)
    #  * Instantiate applet again using test_1_instantiate_applet_from_applet_library
    #  * initiate long tap on WeatherButton
    #  * Check that this time context sensitive menu appears
    #  * Remove the applet
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_2_long_tap_applet_launches_applet_object_menu
	test_1_instantiate_applet_from_applet_library
	# Initiate object menu on container
        verify(20) {@app.DuiContainer(:title=> 'Weather today').long_tap}
        # Verify object menu appeared
        verify(20) {@app.DuiObjectMenu}
        # Verify presence of default actions
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Remove Applet')}
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Settings')}
        # Verify action works
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Remove Applet').tap}
        # Verify no applet anymore
        verify_false(20) {@app.test_object_exists?("DuiContainer",{:title => "Weather today"})}        

	test_1_instantiate_applet_from_applet_library
	# Initiate object menu on Weather button
	verify(20) {@app.DuiContainer(:title=> 'Weather today').long_tap_object(200,200)}
        # Verify object menu appeared
	verify(20) {@app.DuiObjectMenu}
        # Verify presence of extra context sensitive actions
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Remove Applet')}
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Settings')}
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Toggle generic style')}
        # Verify action works
        verify(5) {@app.DuiObjectMenu.DuiButton(:text=> 'Remove Applet').tap}
        # Verify no applet anymore
        verify_false(20) {@app.test_object_exists?("DuiContainer",{:title => "Weather today"})}    
    end
end
