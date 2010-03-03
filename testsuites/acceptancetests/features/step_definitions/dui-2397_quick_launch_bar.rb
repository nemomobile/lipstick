require 'matti'

Before do
    @sut = MATTI.sut(:Id => 'sut_qt_maemo')
end

After do
    # Close the launcher if it's visible
    begin
        @app.DuiModalSceneWindow(:name => 'LauncherWindow', :__timeout => 1)
        @app.DuiButton(:name => 'ToggleLauncherButton').tap
    rescue
    end
end

Given /^duihome is running$/ do
    @app = @sut.application(:name => 'duihome')
end

Given /^switcher is opened$/ do
    verify_not(3) { @app.DuiModalSceneWindow(:name => 'LauncherWindow') }
end

Given /^launcher is closed$/ do
    verify_not(3) { @app.DuiModalSceneWindow(:name => 'LauncherWindow') }
end

Given /^I have opened the launcher$/ do
    # Open the launcher if it's not visible
    begin
        @app.DuiModalSceneWindow(:name => 'LauncherWindow', :__timeout => 1)
    rescue
        @app.DuiButton(:name => 'ToggleLauncherButton').tap
    end
end

When /^I tap on the launcher open\/close button$/ do
    @app.DuiButton(:name => 'ToggleLauncherButton').tap
end

Then /^Quick Launch Bar is visible$/ do
    @app.QuickLaunchBar(:visible => 'true')
end

Then /^launcher is opened$/ do
    @app.DuiModalSceneWindow(:name => 'LauncherWindow')
end
