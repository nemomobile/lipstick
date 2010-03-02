Feature: DUI-1949 Launcher layout and navigation
	As an end user
	I want to access applications from the launcher
	so that I can execute applications on my system

	Scenario: Opening launcher when there are no applications installed
		Given duihome is running
		And there are no applications installed on the system
		When I open launcher
		Then launcher is opened
		And current page of launcher is empty

	Scenario: Showing application icons on the launcher
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And application "application2" is configured to the "slot2" of "page1" of launcher
		When I open launcher
		Then launcher is opened
		And "page1" of launcher is visible
		And "slot1" of current page of launcher contains "application1" icon
		And "slot2" of current page of launcher contains "application2" icon

	Scenario: Flipping between launcher pages
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And application "application2" is configured to the "slot2" of "page1" of launcher
		And application "application3" is configured to the "slot1" of "page2" of launcher
		And launcher is opened
		And "page1" of launcher is visible
		And "slot1" of current page of launcher contains "application1" icon
		And "slot2" of current page of launcher contains "application2" icon
		When I flick to next launcher page
		Then "page2" of launcher is visible
		And "slot1" of current page of launcher contains "application3" icon

	Scenario: Launching an application from launcher
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And launcher is opened
		And "page1" of launcher is visible
		And "slot1" of current page of launcher contains "application1" icon
		When I tap on the icon on "slot1" of current page of launcher
		Then "application1" is on the foreground

	Scenario: Bringing an application on foreground from launcher
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And launcher is opened
		And "page1" of launcher is visible
		And "slot1" of current page of launcher contains "application1" icon
		And "application1" is on the background
		When I tap on the icon on "slot1" of current page of launcher
		Then "application1" is on the foreground

	Scenario: Adding launcher icon to the end of launcher page when application is installed
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And application "application3" is configured to the "slot1" of "page2" of launcher
		And launcher is opened
		And "page2" of launcher is visible
		And "slot1" of current page of launcher contains "application3" icon
		When I install "application4" to the system
		Then "slot2" of current page of launcher contains "application4" icon
		And application "application4" is configured to the "slot2" of "page2" of launcher

	Scenario: Creating new launcher page when new application is installed
		Given duihome is running
		And "page1" of launcher is full
		And launcher is opened
		And "page1" of launcher is visible
		When I install "NewApplication" to the system
		Then "page1" of launcher is visible
		And application "NewApplication" is configured to the "slot1" of "page2" of launcher

	Scenario: Removing a launcher icon when application is uninstalled
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And application "application2" is configured to the "slot2" of "page1" of launcher
		And launcher is opened
		And "page1" of launcher is visible
		And "slot1" of current page of launcher contains "application1" icon
		And "slot2" of current page of launcher contains "application2" icon
		When I uninstall "application1" from the system
		Then "page1" of launcher is visible
		And "slot1" of current page of launcher contains "application2" icon

	Scenario: Removing a launcher page when all application on the page are uninstalled
		Given duihome is running
		And application "application1" is configured to the "slot1" of "page1" of launcher
		And application "application2" is configured to the "slot1" of "page2" of launcher
		And application "application3" is configured to the "slot1" of "page3" of launcher
		And launcher is opened
		And "page2" of launcher is visible
		And "slot1" of current page of launcher contains "application2" icon
		When I uninstall "application2" from the system
		Then "page3" of launcher is visible
		And "slot1" of current page of launcher contains "application3" icon

