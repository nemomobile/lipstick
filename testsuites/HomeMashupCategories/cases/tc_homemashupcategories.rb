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
    end

    # method called after any test case for cleanup purposes
    def teardown
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
    #  * verify that applet library button is visible
    #  * verify tap on the applet library button
    #* Post-conditions
    #  * duihome should now be in applet library window
    #  * verify that no categories are defined by default
    #* References
    #  
    
    def test_appletinventory
    appletInventoryButton = @app.DuiButton(:name => 'DuiAppletInventoryButton') 
    verify_equal('true', 5) { appletInventoryButton.attribute('visible') }
    verify { appletInventoryButton.tap }
    appletInventory = @app.DuiAppletInventory
    list = appletInventory.call_method("categories()")
    verify_equal(nil, 5) { list }
    # Not possible to test now as Matti does not support QStringList datatype as parameter   
    # Check that applets are arranged as per categories defined
    # categories = ['cateogry1', 'category2', 'category3']
    # appletInventory.call_method(setCategories(categories))
    # list1 = appletInventory.call_method("categories")
    #verify_equal(3, 5) { list1.count() }
    end
end
