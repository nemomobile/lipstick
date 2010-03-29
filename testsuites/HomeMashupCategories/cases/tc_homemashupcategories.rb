#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: HomeMashupCategories test suite functional cases for Awidget
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

    #SuiteDomain::            Application framework
    #SuiteTimeout::           600
    #SuiteType::              Functional
    #
    #SetHardware::            true
    #SetScratchbox::          true
    #SetFeature::
    #SetTimeout::             10
    class TC_HomeMashupCategories < Dui::TestCase

    # method called before any test case
    def setup
        # get the application
        @app = @sut.application(:name => 'duihome')

        # inject the required fixture to the SUT configutaion
        fixtures = MobyUtil::Parameter[@sut.id][:fixtures]
        fixtures[:mashupcanvas] = 'fixture_mashupcanvas'

        remove_test_applets
    end

    # method called after any test case for cleanup purposes
    def teardown
        remove_test_applets
    end

    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #Test case documentation one liner to include objectives
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Create test applet entries: one in category 'test1', one in 'test2'
    #    and one in both categories
    #  * Open applet space
    #  * Open applet library (applet inventory)
    #  * Reset the applet inventory categories settings
    #  * Verify that no categories are set
    #  * Set the applet inventory categories settings to 'test2' and 'test2'
    #  * Verify that all 3 test applets are visible
    #  * Set the applet inventory categories settings to 'test1'
    #  * Verify that only the applets in category 'test1' (1,3) are visible
    #  * Set the applet inventory categories settings to 'test2'
    #  * Verify that only the applets in category 'test2' (2,3) are visible
    #  * Set the applet inventory categories settings to 'test3'
    #  * Verify that no test applets are visible
    #  * Reset the applet inventory categories settings
    #  * Close applet library
    #  * Close applet space
    #* Post-conditions
    #  * none
    #* References
    #

    def test_categories

	add_test_applet "1", "test1"
	add_test_applet "2", "test2"
	add_test_applet "all", "test1;test2"

        open_applet_space
        open_applet_inventory

        mashupCanvas = @app.DuiMashupCanvas

        # Reset applet inventory categories
        mashupCanvas.fixture("mashupcanvas", "setCategories",
                             {:categories => ""})
        list = mashupCanvas.fixture("mashupcanvas", "categories")
        verify_equal("", 0, "Cetegories list is not empty") { list }

        # Set applet inventory categories to 'test1' and 'test2'
        mashupCanvas.fixture("mashupcanvas", "setCategories",
                             {:categories => "test1,test2"})
        list = mashupCanvas.fixture("mashupcanvas", "categories")
        verify_equal("test1,test2", 0, "Cetegories did not set correctly") { list }

        # Verify that all test applets are visible
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 1'}) }
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 2'}) }
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test all'}) }

        # Set applet inventory categories to 'test1'
        mashupCanvas.fixture("mashupcanvas", "setCategories",
                             {:categories => "test1"})
        list = mashupCanvas.fixture("mashupcanvas", "categories")
        verify_equal("test1", 0, "Cetegories did not set correctly") { list }

        # Verify that only '1' and 'all' test applets are visible
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 1'}) }
        verify_false(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 2'}) }
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test all'}) }

        # Set applet inventory categories to 'test2'
        mashupCanvas.fixture("mashupcanvas", "setCategories",
                             {:categories => "test2"})
        list = mashupCanvas.fixture("mashupcanvas", "categories")
        verify_equal("test2", 0, "Cetegories did not set correctly") { list }

        # Verify that only '2' and 'all' test applets are visible
        verify_false(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 1'}) }
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 2'}) }
        verify_true(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test all'}) }

        # Set applet inventory categories to 'test3'
        mashupCanvas.fixture("mashupcanvas", "setCategories",
                             {:categories => "test3"})
        list = mashupCanvas.fixture("mashupcanvas", "categories")
        verify_equal("test3", 0, "Cetegories did not set correctly") { list }

        # Verify that no test applets are visible
        verify_false(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 1'}) }
        verify_false(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test 2'}) }
        verify_false(10) { @app.test_object_exists?('DuiAppletButton', {:text => 'Mashup Test all'}) }

        # Reset applet inventory categories
        mashupCanvas.fixture("mashupcanvas", "setCategories",
                             {:categories => ""})

        close_applet_inventory
        close_applet_space
    end

private

    # Check if the applet space is open and return true or false

    def applet_space_open?
        begin
            @app.DuiModalSceneWindow(:name => 'AppletSpaceWindow')
            true
        rescue
            false
        end
    end

    # Open applet space if it is not open already

    def open_applet_space
        if not applet_space_open?
            verify(10, "Cannot tap the applet space button") {
                # This button really needs a name
                @app.DuiButton(:text => 'Applet Space').tap
            }
            verify_true(10, "Opening applet space failed") {
                applet_space_open?
            }
        end
    end

    # Close applet space if it is open

    def close_applet_space
        if applet_space_open?
            verify(10, "Cannot tap the applet space close button") {
                @app.DuiButton(:name => 'AppletSpaceCloseButton').tap
            }
            verify_false(10, "Closing applet space failed") {
                applet_space_open?
            }
        end
    end

    # Check if the applet inventory (applet library) is open and return
    # true or false.

    def applet_inventory_open?
        begin
            @app.DuiModalSceneWindow(:name => 'DuiAppletInventoryWindow')
            true
        rescue
            false
        end
    end

    # Open applet inventory (applet library) if it is not open already.
    # This function only works if the applet space is already open.

    def open_applet_inventory
        if not applet_inventory_open?
            verify(10, "Cannot tap the applet inventory button") {
                @app.DuiButton(:name => 'DuiAppletInventoryButton').tap
            }
            verify_true(10, "Opening applet inventory failed") {
                applet_inventory_open?
            }
        end
    end

    # Close applet inventory (applet library) if it is open

    def close_applet_inventory
        if applet_inventory_open?
            verify(10, "Cannot tap the applet inventory close button") {
                @app.DuiButton(:name => 'DuiAppletInventoryCloseButton').tap
            }
            verify_false(10, "Closing applet inventory failed") {
                applet_inventory_open?
            }
        end
    end

    # Add a test applet .desktop-entry to the "/usr/share/dui/applets/"
    # directory with the name "Mashup Test #{id}" and a set of categories.
    # The test entry point to the weather applet.

    def add_test_applet id, category
        name = "/usr/share/dui/applets/mashup-test-#{id}.desktop"
        entry = (<<-EOF).gsub(/^[ \t]+/, '')
            [Desktop Entry]
            Type=DUIApplet
            Name=Mashup Test #{id}
            Icon=W-Icon-weather
            Exec=duiappletrunner
            Categories=#{category};

            [DUI]
            X-DUIApplet-Applet=libweatherapplet.so
            X-DUIApplet-Identifier=weatherappletres
        EOF

        create_file name, entry
    end

    # Remove all test applet .desktop-entry from the
    # "/usr/share/dui/applets/" directory.

    def remove_test_applets
        Dir.glob("/usr/share/dui/applets/mashup-test-*.desktop").each { |f|
            remove_file f
        }
    end
end
