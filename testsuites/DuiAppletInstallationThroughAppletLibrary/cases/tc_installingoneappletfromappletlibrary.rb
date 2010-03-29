#  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
#  All rights reserved.
#
#  * Description: DuiAppletInstallationThroughAppletLibrary test suite
#  * Objectives: Test feature: Installing one applet from applet library
#
#  Author: Aki Koskinen (mailto:aki.koskinen@nokia.com)
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
#SetScratchbox::          false
#SetFeature::             SWP#DUI-1602
#SetTimeout::             100
#
# Tests SWP#DUI-1602: Installing one applet from applet library
class TC_DuiAppletInstallationThroughAppletLibrary < Dui::TestCase
    # Removes applets from the canvas one by one as long as they exist
    def removeAllApplets
        begin
            while @app.DuiMashupCanvas.DuiContainer(:__index => 0, :__timeout => 5)
                @app.DuiMashupCanvas.DuiContainer(:__index => 0).long_tap
                begin
                    @app.DuiButton(:text => 'Remove Applet', :__timeout => 5).tap
                rescue
                    @app.DuiButton(:text => '!! Remove Applet', :__timeout => 5).tap
                end
            end
        rescue
        end
    end

    # method called before any test case
    def setup
        # remove the test applet .desktop file
        if File.exist?("/tmp/fakeappletpackage.desktop")
            File.delete("/tmp/fakeappletpackage.desktop")
        end

        # get the application
        @app = @sut.application(:name => 'duihome')

        # Clear any existing applets from the canvas
        removeAllApplets
    end

    # method called after any test case for cleanup purposes
    def teardown
    end

   

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Show installation source
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Accesses applet library
    #  * Verifies that the fake installation source is there
    #  * Closes the applet library
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_1_show_installation_source
        # Action 1: User opens applet library
        verify(5) {accessInstallationSource}

        # Check the prestate
        # - Applet library is shown with one installation source
        verify(5) {@app.DuiContainer(:name => 'DuiAppletInventorySourceContainer').InstallationSourceWidget}
        # - Mock installation source can provide five applets (in separate packages): Installable, UnInstallable, UnDownloadable, MetadataOnly, NonExistent
        verify(5) {@app.DuiButton(:text => 'Installable')}
        verify(5) {@app.DuiButton(:text => 'Uninstallable')}
        verify(5) {@app.DuiButton(:text => 'Not Downloadable')}
        verify(5) {@app.DuiButton(:text => 'Metadata Only')}
        verify(5) {@app.DuiButton(:text => 'Non-existent')}

        # Close the applet inventory
        @app.DuiButton(:name => 'DuiAppletInventoryCloseButton').tap
    end

    # Opens applet library.
    # Pans, if needed, so that the required button is visible.
    # Taps the installation button.
    def startInstallationProcess(buttonText)
        # Prerequisite: Test 1
        accessInstallationSource

        # Action 1: User selects Installable applet
        scrollTo(buttonText)
        @app.DuiButton(:text => buttonText).tap

        # Results:
        # - Applet library disappears, view returns to experience canvas
        verify_not(5) { @app.DuiAppletInventory }
    end

    # Verifies that as long as the progress indicator is visible on the screen,
    # it reflects the correct value.
    def monitorInstallationStatus
        verify(5) { @app.DuiProgressIndicator }
        testDBusService.continueProgress
        begin
            # As long as there is a progress indicator, verify that it's showing the correct value
            while true
                progressIndicator = @app.DuiProgressIndicator(:__timeout => 5)
                verify(5) { progressIndicator.attribute('value').to_i == testDBusService.lastOperationProgress }
            end
        rescue
        end
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Install an applet package that succeeds to install
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Start installation of an installable applet package
    #  * Monitor installation status
    #  * Verify that the applet is launched after the installation ends
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_2_install_applet_successfully
        # Action 1: User selects Installable applet
        installButton = 'Installable'
        startInstallationProcess(installButton)

        # - On experience canvas there's an applet placeholder indicating download and install progress
        monitorInstallationStatus

        # - Applet placeholder is updated to match the new download state
        # FIXME the applet handle doesn't currently listen to the download progress
#         for i in 0..5
#             testDBusService.DownloadProgress(i * 10, 50)
#             verify(1) { @app.DuiAppletHandle.DuiProgressIndicator }
#         end

        # - Applet placeholder is updated to match the new state (from downloading to installing)
        # - Applet placeholder is updated to match the new installation state

        # - Applet contained in the installed package is instantiated and shown where installation placeholder used to be
        verify_not(5) { @app.DuiProgressIndicator }
        verify(5) { @app.DuiContainer(:title => 'Weather Today') }
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Install an applet package that fails to install
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Start installation of an uninstallable applet package
    #  * Monitor installation status
    #  * Verify that the applet ends up in the broken state
    #  * Tap the applet placeholder
    #  * A dialog is shown with "Remove applet" button
    #  * Verify that the applet placeholder can be removed by pressing the button
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_3_handle_error_in_installation
        # Action 1: User selects UnInstallable applet
        installButton = 'Uninstallable'
        startInstallationProcess(installButton)

        # - On experience canvas there's an applet placeholder indicating download and install progress
        monitorInstallationStatus

        # - Applet placeholder is updated to match the new download state
        # FIXME the applet handle doesn't currently listen to the download progress
#         for i in 0..5
#             testDBusService.DownloadProgress(i * 10, 50)
#             verify(1) { @app.DuiAppletHandle.DuiProgressIndicator }
#         end

        # - Applet placeholder is updated to match the new state (from downloading to installing)
        # - Applet placeholder is updated to match the new installation state

        # - Indicate installation failure on applet placeholder
        verify_not(10) { @app.DuiProgressIndicator }
        verify(10) { @app.DuiAppletHandle(:state => 'BROKEN') }

        #   - Clarify how the failure is indicated from Esa and Jorma
        #
        # Action 2: User taps applet placeholder
        @app.DuiAppletHandle.tap

        # - Show dialog that indicates that installation of applet has failed
        # - Show one option "Remove applet"
        verify(5) { @app.DuiMessageBox.DuiButton }

        # Action 3b: User selects "Cancel" on the dialog
        # Tap outside the message box
        @app.DuiMessageBox.tap_object(1, 1)

        # - Dialog is dismissed
        verify_not(5) { @app.DuiMessageBox }
        # - View goes back to experience canvas
        # - Return to state before Action 2

        # Action 2: User taps applet placeholder
        @app.DuiAppletHandle.tap

        # Action 3a: User selects "Remove applet" on the dialog
        @app.DuiMessageBox.DuiButton.tap

        # - Dialog is dismissed
        verify_not(5) { @app.DuiMessageBox }
        # - View goes back to experience canvas
        # - Applet placeholder is removed
        verify_not(5) { @app.DuiAppletHandle }

    end

    
    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Install an applet package that fails during download phase
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Start installation of an downloadable applet package that fails to download completely
    #  * Monitor installation status
    #  * Verify that the applet ends up in the broken state
    #  * Tap the applet placeholder
    #  * A dialog is shown with "Remove applet" button
    #  * Verify that the applet placeholder can be removed by pressing the button
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_4_handle_error_during_download
        # Action 1: User selects UnDownloadable applet
        installButton = 'Not Downloadable'
        startInstallationProcess(installButton)

        # - On experience canvas there's an applet placeholder indicating download and install progress
        monitorInstallationStatus

        # - Applet placeholder is updated to match the new download state
        #   - Clarify this from Esa and Jorma. If progress is indicated with spinning indicator this is not required
        # TODO don't know if download progress is included in the installing status signals

        # - Indicate download failure on applet placeholder
        verify_not(5) { @app.DuiProgressIndicator }
        verify(5) { @app.DuiAppletHandle(:state => 'BROKEN') }

        #   - Clarify how the failure is indicated from Esa and Jorma

        # Action 2: User taps applet placeholder
        @app.DuiAppletHandle.tap

        # - Show dialog that indicates that download of applet has failed
        # - Show one option "Remove applet"
        verify(5) { @app.DuiMessageBox.DuiButton }

        # Action 3b: User selects "Cancel" on the dialog
        # Tap outside the message box
        @app.DuiMessageBox.tap_object(1, 1)

        # - Dialog is dismissed
        verify_not(5) { @app.DuiMessageBox }
        # - View goes back to experience canvas
        # - Return to state before Action 2

        # Action 2: User taps applet placeholder
        @app.DuiAppletHandle.tap

        # Action 3a: User selects "Remove applet" on the dialog
        @app.DuiMessageBox.DuiButton.tap

        # - Dialog is dismissed
        verify_not(5) { @app.DuiMessageBox }
        # - View goes back to experience canvas
        # - Applet placeholder is removed
        verify_not(5) { @app.DuiAppletHandle }
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Install an applet package that fails to start downloading
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Start installation of an undownloadable applet package
    #  * Monitor installation status
    #  * Verify that the applet ends up in the broken state
    #  * Tap the applet placeholder
    #  * A dialog is shown with "Remove applet" button
    #  * Verify that the applet placeholder can be removed by pressing the button
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_5_handle_error_when_download_starts
        # Action 1: User selects MetadataOnly applet
        installButton = 'Metadata Only'
        startInstallationProcess(installButton)

        # - On experience canvas there's an applet placeholder indicating download and install progress
        monitorInstallationStatus

        # - Indicate download failure on applet placeholder
        verify_not(5) { @app.DuiProgressIndicator }
        verify(5) { @app.DuiAppletHandle(:state => 'BROKEN') }

        #   - Clarify how the failure is indicated from Esa and Jorma. Same behavior can be used when package manager cannot access repository (problems with credentials, etc.)

        # Action 2: User taps applet placeholder
        @app.DuiAppletHandle.tap

        # - Show dialog that indicates that download of applet has failed
        # - Show one option "Remove applet"
        verify(5) { @app.DuiMessageBox.DuiButton }

        # Action 3b: User selects "Cancel" on the dialog
        # Tap outside the message box
        @app.DuiMessageBox.tap_object(1, 1)

        # - Dialog is dismissed
        verify_not(5) { @app.DuiMessageBox }
        # - View goes back to experience canvas
        # - Return to state before Action 2

        # Action 2: User taps applet placeholder
        @app.DuiAppletHandle.tap

        sleep 5
        
        # Action 3a: User selects "Remove applet" on the dialog
        @app.DuiMessageBox.DuiButton.tap

        # - Dialog is dismissed
        verify_not(5) { @app.DuiMessageBox }
        # - View goes back to experience canvas
        # - Applet placeholder is removed
        verify_not(5) { @app.DuiAppletHandle }
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Install an applet package that doesn't exist
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Start installation of a non-existing applet package
    #  * Verify that the applet ends up in the broken state
    #  * Tap the applet placeholder
    #  * A dialog is shown with "Remove applet" button
    #  * Verify that the applet placeholder can be removed by pressing the button
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_6_selecting_a_non_existent_applet
        # Action 1: User selects NonExistent applet

        # TODO: this test fails at the moment, because can't send d-bus errors from the fake package manager

#         installButton = 'Non-existent'
#         startInstallationProcess(installButton)

#         # - Indicate download failure on applet placeholder
#         verify_not(5) { @app.DuiProgressIndicator }
#         verify(2) { @app.DuiAppletHandle(:state => 'BROKEN') }

#         # Action 2: User taps applet placeholder
#         @app.DuiAppletHandle.tap

#         # - Show dialog that indicates that download of applet has failed
#         # - Show one option "Remove applet"
#         verify(2) { @app.DuiMessageBox.DuiButton }

#         # Action 3: User selects "Remove applet" on the dialog
#         @app.DuiMessageBox.DuiButton.tap

#         # - Dialog is dismissed
#         verify_not(2) { @app.DuiMessageBox }
#         # - View goes back to experience canvas
#         # - Applet placeholder is removed
#         verify_not(2) { @app.DuiAppletHandle }
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Tests that the applet is instantiated if the installation has finished at the time home is started
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Closes duihome
    #  * Sets up mashup canvas to contain an applet whose installation had succeeded
    #  * Starts duihome
    #* Post-conditions
    #  * duihome started
    #* References
    #  * None
    def test_7_applet_installation_has_succeeded
        # Action 1: User kills home
        @app.close

        # Action 2: Set up mashup canvas
        File.open(ENV['HOME'] + "/.config/duihome/appletcanvas.data", 'w') {|f| f.write("[1]\ndesktopFile=/usr/share/dui/applets/weather.desktop\npackageName=fakeappletpackage-installable.deb\nprivate\layoutIndex=0\ntitle=Weather today") }

        # Action 3: User starts up home
        @app = @sut.run(:name => "duihome", :arguments => "-software")

        # - Verify successful instantiation of the applet
        verify_not(5) { @app.DuiProgressIndicator }
        verify(5) { @app.DuiAppletHandle(:state => 'RUNNING') }
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Tests that the applet placeholder shows installation in progress if the installation is still in progress at the time home is started
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Closes duihome
    #  * Sets up mashup canvas to contain an applet whose installation is still in progress
    #  * Starts duihome
    #* Post-conditions
    #  * duihome started
    #* References
    #  * None
    def test_8_applet_installation_still_in_progress
        # Action 1: User kills home
        @app.close

        # Action 2: Set up mashup canvas
        File.open(ENV['HOME'] + "/.config/duihome/appletcanvas.data", 'w') {|f| f.write("[1]\ndesktopFile=/usr/share/dui/applets/fakeappletpackage-inprogress.desktop\npackageName=fakeappletpackage-inprogress.deb\nprivate\layoutIndex=0\ntitle=Weather today") }

        # Action 3: User starts up home
        @app = @sut.run(:name => "duihome", :arguments => "-software")

        # - Indicate installation in progress on applet placeholder
        verify(5) { @app.DuiProgressIndicator }
    end

    #SubFeature::   SWP#DUI-1602
    #Timeout::      100
    #Type::         Functional
    #Manual::       false
    #Level::        Feature
    #
    #Tests that the applet placeholder shows installation failure if the installation is not in progress at the time home is started
    #
    #* Pre-conditions
    #  * duihome started
    #* Test steps
    #  * Closes duihome
    #  * Sets up mashup canvas to contain an applet whose installation has failed
    #  * Starts duihome
    #* Post-conditions
    #  * None
    #* References
    #  * None
    def test_9_applet_installation_has_failed
        # Action 1: User kills home
        @app.close

        # Action 2: Set up mashup canvas
        File.open(ENV['HOME'] + "/.config/duihome/appletcanvas.data", 'w') {|f| f.write("[1]\ndesktopFile=/usr/share/dui/applets/fakeappletpackage-nonexistent.desktop\npackageName=fakeappletpackage-nonexistent.deb\nprivate\layoutIndex=0\ntitle=Weather today") }

        # Action 3: User starts up home
        @app = @sut.run(:name => "duihome", :arguments => "-software")

        # - Indicate installation failure on applet placeholder
        verify_not(5) { @app.DuiProgressIndicator }
        verify(5) { @app.DuiAppletHandle(:state => 'BROKEN') }

        # Kills duihome
        @app.close
    end

private

    # Opens the applet library
    def accessInstallationSource
        begin
            @app.DuiContainer(:name => 'DuiAppletInventorySourceContainer', :__timeout => 5)
        rescue
            # If source container is not found, open applet library
            @app.DuiButton(:name => 'DuiAppletInventoryButton', :__timeout => 5).tap

            # Wait for the applet library to appear
            @app.DuiContainer(:name => 'DuiAppletInventorySourceContainer')
        end
    end

    # Pans the applet library to make the button with the text passed as an argument
    # to become visible.
    def scrollTo(buttonToScrollTo)
        # Scroll the applet library so that the requested installation source button becomes visible
        until @app.DuiButton(:text => buttonToScrollTo, :__timeout => 5).attribute('visibleOnScreen') == 'true'
            @app.DuiModalSceneWindow(:name => 'DuiAppletInventoryWindow', :__timeout => 5).DuiPannableViewport.gesture(:Up, 0.5, 350)
        end
    end

end
