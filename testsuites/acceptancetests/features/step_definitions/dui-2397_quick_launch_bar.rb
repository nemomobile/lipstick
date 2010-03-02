require 'matti'

Before do
    @sut = MATTI.sut(:Id => 'sut_qt_maemo')
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

When /^I tap on the launcher open\/close button$/ do
    @app.DuiButton(:name => 'ToggleLauncherButton').tap
end

Then /^Quick Launch Bar is visible$/ do
    @app.QuickLaunchBar(:visible => 'true')
end

Then /^launcher is opened$/ do
    @app.DuiModalSceneWindow(:name => 'LauncherWindow')
end
