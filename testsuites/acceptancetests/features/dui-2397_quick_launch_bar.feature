Feature: DUI-2397 Quick Launch Bar
	As an end user
	I want to access collection of 0-4 most important applications directly from switcher, anywhere from launcher and from applet space
	so that I have a quick access to the most relevant functionality of the system.
	
	Scenario: Showing Quick Launch Bar in Switcher
		Given duihome is running
		And switcher is opened
		Then Quick Launch Bar is visible

	Scenario: Opening Launcher from Quick Launch Bar
		Given duihome is running
		And launcher is closed
		When I tap on the launcher open/close button
		Then launcher is opened

	Scenario: Showing Quick Launch Bar in Launcher
		Given duihome is running
		And launcher is opened
		Then Quick Launch Bar is visible

	Scenario: Closing Launcher from Quick Launch Bar
		Given duihome is running
		And launcher is opened
		When I tap on the launcher open/close button
		Then launcher is closed

	Scenario: Having a full Quick Launch Bar
		Given duihome is running
		And application "application1" is configured to the "slot1" of Quick Launch Bar
		And application "application2" is configured to the "slot2" of Quick Launch Bar
		And application "application3" is configured to the "slot3" of Quick Launch Bar
		And application "application4" is configured to the "slot4" of Quick Launch Bar
		Then Quick Launch Bar is visible
		And Quick Launch Bar "slot1" contains "application1" icon
		And Quick Launch Bar "slot2" contains "application2" icon
		And Quick Launch Bar "slot3" contains "application3" icon
		And Quick Launch Bar "slot4" contains "application4" icon

	Scenario: Having an empty Quick Launch Bar
		Given duihome is running
		And no application is configured to the "slot1" of Quick Launch Bar
		And no application is configured to the "slot2" of Quick Launch Bar
		And no application is configured to the "slot3" of Quick Launch Bar
		And no application is configured to the "slot4" of Quick Launch Bar
		Then Quick Launch Bar is visible
		And Quick Launch Bar "slot1" contains no icon
		And Quick Launch Bar "slot2" contains no icon
		And Quick Launch Bar "slot3" contains no icon
		And Quick Launch Bar "slot4" contains no icon

	Scenario: Having empty slots in Quick Launch Bar
		Given duihome is running
		And application "application1" is configured to the "slot1" of Quick Launch Bar
		And no application is configured to the "slot2" of Quick Launch Bar
		And application "application2" is configured to the "slot3" of Quick Launch Bar
		And no application is configured to the "slot4" of Quick Launch Bar
		Then Quick Launch Bar is visible
		And Quick Launch Bar "slot1" contains "application1" icon
		And Quick Launch Bar "slot2" contains no icon
		And Quick Launch Bar "slot3" contains "application2" icon
		And Quick Launch Bar "slot4" contains no icon

	Scenario: Uninstalling an application that is present in Quick Launch Bar
		Given duihome is running
		And application "application1" is configured to the "slot1" of Quick Launch Bar
		And application "application2" is configured to the "slot2" of Quick Launch Bar
		And no application is configured to the "slot3" of Quick Launch Bar
		And application "application3" is configured to the "slot4" of Quick Launch Bar
		When I uninstall "application2" from the system
		Then Quick Launch Bar is visible
		And Quick Launch Bar "slot1" contains "application1" icon
		And Quick Launch Bar "slot2" contains no icon
		And Quick Launch Bar "slot3" contains no icon
		And Quick Launch Bar "slot4" contains "application3" icon

	Scenario: Installing an application that was present in Quick Launch Bar
		Given duihome is running
		And application "application1" is configured to the "slot1" of Quick Launch Bar
		And application "application2" is configured to the "slot2" of Quick Launch Bar
		And no application is configured to the "slot3" of Quick Launch Bar
		And application "application3" is configured to the "slot4" of Quick Launch Bar
		When I uninstall "application3" from the system
		And I install "application3" to the system
		Then Quick Launch Bar is visible
		And Quick Launch Bar "slot1" contains "application1" icon
		And Quick Launch Bar "slot2" contains "application2" icon
		And Quick Launch Bar "slot3" contains no icon
		And Quick Launch Bar "slot4" contains no icon

	Scenario: Launching an application from Quick Launch Bar
		Given duihome is running
		And application "application1" is configured to the "slot1" of Quick Launch Bar
		When I tap on the icon on "slot1" of Quick Launch Bar
		Then "application1" is on the foreground

