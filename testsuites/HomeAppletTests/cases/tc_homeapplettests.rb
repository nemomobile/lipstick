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
require 'utils.rb'
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

    # method called before any test case
    def setup
        # get the application
        @app = @sut.application(:name => 'duihome')
    end

    # method called after any test case for cleanup purposes
    def teardown
	@app.DuiButton(:text => 'Applet Space').tap
        noOfContainers = count_containers
	i = 0
        while i <  noOfContainers do
	    @app.DuiContainer(:__index => 0).long_tap
	    begin
                @app.DuiObjectMenu.DuiButton(:text => 'Remove').tap
            rescue
                @app.DuiObjectMenu.DuiButton(:text => '!! Remove Applet').tap
            end
        i = i + 1
       end
       @app.DuiButton(:text => 'Applet Space').tap
   end
   
   # Counts and returns the number of containers in the applet space
   
   def count_containers
       count = 0
       begin
           container = @app.DuiContainer(:__index => count)
           count = count + 1
       rescue NoMethodError, MobyBase::TestObjectNotFoundError
           container = nil
       end while container
       return count
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
    #Timeout::      600
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Instantiating installed applets from the applet library
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Open the applet space
    #  * Accesses applet library
    #  * Flick to find the weather applet
    #  * Selects the weather applet
    #  * Verifies weather applet was loaded
    #  * Close the applet space
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_1_instantiate_applet_from_applet_library
        # Action 1: User opens applet library
        @app.DuiButton(:text => 'Applet Space').tap
	@app.DuiButton(:name => 'DuiAppletInventoryButton').tap
	scrollTo("Weather")
	# Instantiate Weather applet
        @app.DuiAppletButton(:text => 'Weather').tap
	verify {@app.DuiContainer(:title=> 'Weather Today')}
        @app.DuiButton(:text => 'Applet Space').tap
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      600
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
    #  * Close the applet space
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_2_long_tap_applet_launches_applet_object_menu
	test_1_instantiate_applet_from_applet_library
        @app.DuiButton(:text => 'Applet Space').tap
	# Initiate object menu on container
        @app.DuiContainer(:title=> 'Weather Today').long_tap
        # Verify object menu appeared
        verify {@app.DuiObjectMenu}
        # Verify presence of default actions
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Remove')}
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Settings')}
        # Verify action works
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Remove').tap}
        # Verify no applet anymore
        verify_false {@app.test_object_exists?("DuiContainer",{:title => "Weather Today"})}        
        @app.DuiButton(:text => 'Applet Space').tap
	test_1_instantiate_applet_from_applet_library
        @app.DuiButton(:text => 'Applet Space').tap
	# Initiate object menu on Weather button
	@app.DuiContainer(:title=> 'Weather Today').long_tap_object(200,200)
        # Verify object menu appeared
	verify {@app.DuiObjectMenu}
        # Verify presence of extra context sensitive actions
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Remove')}
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Settings')}
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Toggle generic style')}
        # Verify action works
        verify {@app.DuiObjectMenu.DuiButton(:text=> 'Remove').tap}
        # Verify no applet anymore
        verify_false {@app.test_object_exists?("DuiContainer",{:title => "Weather Today"})}
        @app.DuiButton(:text => 'Applet Space').tap   
    end
end
