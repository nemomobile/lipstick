Feature: DUI-2208 Possibility to change the wallpaper of homescreen.
	As an end user
	I want to be able to change the graphics of switcher background
	so that I can personalize the view to my preference.

	Scenario: Changing portrait wallpaper image
		Given duihome is running
		And switcher is opened
		And device is in portrait orientation
		And visible wallpaper is theme default portrait wallpaper
		When I set the portrait wallpaper image to valid_portrait_wallpaper.png
		Then visible wallpaper is valid_portrait_wallpaper.png

	Scenario: Changing landscape wallpaper image
		Given duihome is running
		And switcher is opened
		And device is in landscape orientation
		And visible wallpaper is theme default landscape wallpaper
		When I set the landscape wallpaper image to valid_landscape_wallpaper.png
		Then visible wallpaper is valid_landscape_wallpaper.png

	Scenario: Changing wallpaper image to wrong resolution
		Given duihome is running
		And switcher is opened
		And device is in landscape orientation
		And visible wallpaper is theme default landscape wallpaper
		When I set the landscape wallpaper image to valid_portrait_wallpaper.png
		Then visible wallpaper is theme default landscape wallpaper

	Scenario: Changing wallpaper image to non-existent image
		Given duihome is running
		And switcher is opened
		And device is in portrait orientation
		And visible wallpaper is theme default portrait wallpaper
		When I set the portrait wallpaper image to non-existent_portrait_wallpaper.png
		Then visible wallpaper is theme default portrait wallpaper

	Scenario: Changing wallpaper image to corrupted image
		Given duihome is running
		And switcher is opened
		And device is in landscape orientation
		And visible wallpaper is theme default landscape wallpaper
		When I set the landscape wallpaper image to corrupted_landscape_wallpaper.png
		Then visible wallpaper is theme default landscape wallpaper

	Scenario: Changing landscape wallpaper image in portrait orientation
		Given duihome is running
		And switcher is opened
		And device is in portrait orientation
		And visible wallpaper is theme default portrait wallpaper
		When I set the landscape wallpaper image to valid_landscape_wallpaper.png
		And I rotate the device to landscape orientation
		Then visible wallpaper is valid_landscape_wallpaper.png

