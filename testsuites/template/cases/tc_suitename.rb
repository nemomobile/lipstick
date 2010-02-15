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

##### REMOVE EMPTY TAGS BEFORE COMMITTING!
#SuiteDomain::            Check test.xml definition from pmo wiki
#SuiteTimeout::           3600
#SuiteType::              (Functional | Integration | Security | Performance | Benchmark | Resource Utilization | Robustness | Recoverability | Usability)
#
#SetPreSteps::            
#SetPostSteps::           
#SetHardware::            true
#SetScratchbox::          false
#SetLogFiles::            
#SetFeature::             (defined by the team in ScrumWorks Pro or Accept360)
#SetTimeout::             3600
#
# Suite description goes here. 
# Do not leave default description
class TC_SuiteName < Dui::TestCase

    # method called before any test case
    def setup
    # get the application
      @app = @sut.application(:name => 'ta_suitename')
    end

    # method called after any test case for cleanup purposes
    def teardown
    end

    #SubFeature::   (defined by the team in ScrumWorks Pro or Accept360)
    #Timeout::      100
    #Type::         (Functional | Integration | Security | Performance | Benchmark | Resource Utilization | Robustness | Recoverability | Usability)
    #Requirement::  NONE
    #Manual::       false
    #Level::        (Component | Feature | System | Product)
    #Test case documentation one liner to include objectives
    #
    #* Pre-conditions
    #  * ta_suitename started
    #* Test steps
    #  * tap on xxx then zzzz has changed to AAA
    #  * tap on xxx then ssss has changed to BBB
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_suitename0_fail
      verify_true(1, "Implement your testcases here, this is just a placeholder that will always fail!") {
        false
      }
    end
end
