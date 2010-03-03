#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: DuiApplicationExtensionArea test suite functional cases for application extension area functionality
#  * Objectives: test the functionality of application extensions
#
#  Author:: Pankaj Saharan(mailto:pankaj.saharan@nokia.com)
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
#SetFeature::             SWP#DUI-2093
#SetTimeout::             10
#
# Tests SWP#DUI-2093: Initial Test suite for checking the functionality of application extension area
class TC_DuiApplicationExtensionArea < Dui::TestCase

    # method called before any test case
    def setup
    # get the application
      @app = @sut.application(:name => 'applicationextensiondemo')
    end

    # method called after any test case for cleanup purposes
    def teardown
    end

    #SubFeature::   SWP#DUI-2093
    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Test for checking if the application extension area is visible with loading of in-process extensions
    #
    #* Pre-conditions
    #  * applicationextensiondemo started with the application extensions loaded
    #* Test steps
    #  * Verify that application extension area is shown
    #  * Verify that application extension area contains the loaded application extensions
    #* Post-conditions
    #  * None
    #* References
    #  * None

def test_1_duiapplicationextensionarea_load_in_process_application_extensions
       verify(5) { @app.DuiApplicationExtensionArea }
       verify(5) { @app.DuiSlider( :name => 'demo_slider' ).DuiSliderGroove().DuiWidget().drag(:Right, 50) }
       verify_equal("Value from extension: 6 low", 10) { @app.DuiLabel( :name => 'ExtensionValueLabel' ).attribute('text') }
       verify(2) {@app.DuiButton(:text => 'Reset Extensions').tap}
       verify_equal("Value from extension: 0 low", 10) { @app.DuiLabel( :name => 'ExtensionValueLabel' ).attribute('text') }
    end

    #SubFeature::   SWP#DUI-2093
    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Test for checking if the application extension area loads and removes out-of-process extensions
    #
    #* Pre-conditions
    #  * applicationextensiondemo started with the application extensions loaded
    #* Test steps
    #  * Verify that application extension area is shown
    #  * Verify that no application extension runner process is running if no out-of-process extension is loaded
    #  * Verify that application extension runner process is running when out-of-process extension is loaded
    #  * Remove out-of-process extension desktop file and verify that runner process is terminated
    #* Post-conditions
    #  * None
    #* References
    #  * None

    def test_2_duiapplicationextensionarea_load_out_process_application_extensions
       verify(5) { @app.DuiApplicationExtensionArea }
       count = count_runner_process()
       assert_equal( "0", count )
       add_app_desktop_entry("/tmp/demoextension3.desktop")
       sleep 10
       count = count_runner_process()
       assert_equal( "1", count )

       delete_file("/tmp/demoextension3.desktop")
       sleep 5
       count = count_runner_process()
       assert_equal( "0", count )
   end

    #SubFeature::   SWP#DUI-2093
    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Test for checking if the application extension area removes in-process extensions
    #
    #* Pre-conditions
    #  * applicationextensiondemo started with the application extensions loaded
    #* Test steps
    #  * Verify that application extension area is shown
    #  * Verify that application extension area contains the loaded application extensions
    #  * Verify that application extension area does not contain the removed application extensions
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_3_duiapplicationextensionarea_remove_in_process_application_extensions
       verify(5) { @app.DuiApplicationExtensionArea }
       verify(5) { @app.DuiSlider( :name => 'demo_slider' ) }
       # remove the demo extension .desktop file
       delete_file("/tmp/demoextension2.desktop")
       verify_equal("Value from extension: 0", 10) { @app.DuiLabel( :name => 'ExtensionValueLabel' ).attribute('text') }
       verify(5) { @app.DuiApplicationExtensionArea }
       verify(5) { @app.DuiSlider( :name => 'demo_slider' ) }
       delete_file("/tmp/demoextension.desktop")
       verify_false(10) {
       @app.test_object_exists?("DuiSlider", {:name => 'demo_slider'})
      }
    end

private

    # Helper method for deleting a given file
    def delete_file(fileName)
        if File.exist?(fileName)
            File.delete(fileName)
        end
    end

    # add test app desktop entry for out-of-process application extensions
    def add_app_desktop_entry( path_to_new_desktop_file )
        desktopentry = "[Desktop Entry]\n"\
                       "Type=DUIApplicationExtension\n"\
                       "Name=DemoExtension3\n"\
                       "Exec=duiapplicationextensionrunner\n"\
                       "\n"\
                       "[X-DUI]\n"\
                       "DUIApplicationExtension-Interface=com.nokia.dui.core.DemoApplicationExtensionInterface/1.0\n"\
                       "DUIApplicationExtension-Extension=libdemoextension3.so\n";

        File.open(path_to_new_desktop_file, 'w') {|f| f.write(desktopentry) }
    end

    # Helper method for returning the number of runner processes running
    # return number of currently running application extension runner process(es)
    def count_runner_process()
        count = `ps -ef | grep duiapplicationextensionrunner|grep -v grep | wc -l`
        return count.strip
    end

end
