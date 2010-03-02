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

Then /^Quick Launch Bar is visible$/ do
    @app.QuickLaunchBar(:visible => 'true')
end
