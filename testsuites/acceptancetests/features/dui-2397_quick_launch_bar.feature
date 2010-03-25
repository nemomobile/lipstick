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
		And I have opened the launcher
		Then Quick Launch Bar is visible

	Scenario: Closing Launcher from Quick Launch Bar
		Given duihome is running
		And I have opened the launcher
		When I tap on the launcher open/close button
		Then launcher is closed

	Scenario: Having a full Quick Launch Bar
		Given duihome is running
		And I have configured application "1" to the slot "1" of Quick Launch Bar
		And I have configured application "2" to the slot "2" of Quick Launch Bar
		And I have configured application "3" to the slot "3" of Quick Launch Bar
		And I have configured application "4" to the slot "4" of Quick Launch Bar
		Then Quick Launch Bar is visible
		And Quick Launch Bar slot "1" contains application "1" icon
		And Quick Launch Bar slot "2" contains application "2" icon
		And Quick Launch Bar slot "3" contains application "3" icon
		And Quick Launch Bar slot "4" contains application "4" icon

	Scenario: Having an empty Quick Launch Bar
		Given duihome is running
		And no application is configured to the slot "1" of Quick Launch Bar
		And no application is configured to the slot "2" of Quick Launch Bar
		And no application is configured to the slot "3" of Quick Launch Bar
		And no application is configured to the slot "4" of Quick Launch Bar
		Then Quick Launch Bar is visible
		And Quick Launch Bar slot "1" contains no icon
		And Quick Launch Bar slot "2" contains no icon
		And Quick Launch Bar slot "3" contains no icon
		And Quick Launch Bar slot "4" contains no icon

	Scenario: Having empty slots in Quick Launch Bar
		Given duihome is running
		And I have configured application "1" to the slot "1" of Quick Launch Bar
		And no application is configured to the slot "2" of Quick Launch Bar
		And I have configured application "2" to the slot "3" of Quick Launch Bar
		And no application is configured to the slot "4" of Quick Launch Bar
		Then Quick Launch Bar is visible
		And Quick Launch Bar slot "1" contains application "1" icon
		And Quick Launch Bar slot "2" contains no icon
		And Quick Launch Bar slot "3" contains application "2" icon
		And Quick Launch Bar slot "4" contains no icon

	Scenario: Uninstalling an application that is present in Quick Launch Bar
		Given duihome is running
		And I have configured application "1" to the slot "1" of Quick Launch Bar
		And I have configured application "2" to the slot "2" of Quick Launch Bar
		And no application is configured to the slot "3" of Quick Launch Bar
		And I have configured application "3" to the slot "4" of Quick Launch Bar
		When I uninstall application "2" from the system
		Then Quick Launch Bar is visible
		And Quick Launch Bar slot "1" contains application "1" icon
		And Quick Launch Bar slot "2" contains no icon
		And Quick Launch Bar slot "3" contains no icon
		And Quick Launch Bar slot "4" contains application "3" icon

	Scenario: Installing an application that was present in Quick Launch Bar
		Given duihome is running
		And I have configured application "1" to the slot "1" of Quick Launch Bar
		And I have configured application "2" to the slot "2" of Quick Launch Bar
		And no application is configured to the slot "3" of Quick Launch Bar
		And I have configured application "3" to the slot "4" of Quick Launch Bar
		When I uninstall application "3" from the system
		And I install application "3" to the system
		Then Quick Launch Bar is visible
		And Quick Launch Bar slot "1" contains application "1" icon
		And Quick Launch Bar slot "2" contains application "2" icon
		And Quick Launch Bar slot "3" contains no icon
		And Quick Launch Bar slot "4" contains no icon

	Scenario: Launching an application from Quick Launch Bar
		Given duihome is running
		And I have configured application "1" to the slot "1" of Quick Launch Bar
		When I tap on the icon on slot "1" of Quick Launch Bar
		Then application "1" is on the foreground
