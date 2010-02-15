#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: DuiApplicationExtensionArea test suite functional cases for application extension area functionality
#  * Objectives: test the functionality of application extensions 
#
#  Author:: Your Name (mailto:pankaj.saharan@nokia.com)
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
#SetFeature::             SWP#DUI-2190
#SetTimeout::             10
#
# Tests SWP#DUI-2190: Initial Test suite for checking the functionality of application extension area
class TC_DuiApplicationExtensionArea < Dui::TestCase

    # method called before any test case
    def setup
    # get the application
      @app = @sut.application(:name => 'duiapplicationextensiontester')
    end

    # method called after any test case for cleanup purposes
    def teardown
    @app.close
    end

    #SubFeature::   SWP#DUI-2190
    #Timeout::      60
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Test for checking if the application extension is loaded fine and is visible
    #
    #* Pre-conditions
    #  * duiapplicationextensiontester started with the application extension loaded
    #* Test steps
    #  * Verify that application extension area is shown
    #  * Verify that application extension area contains the loaded application extension
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_duiapplicationextensionarea1_load_application_extension
       verify(5) { @app.DuiApplicationExtensionArea }
       verify(5) { @app.DuiAppletHandle(:appletTitle => 'Weather Today') }
    end
end
