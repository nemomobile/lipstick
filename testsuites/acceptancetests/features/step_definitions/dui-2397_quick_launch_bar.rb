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

Given /^I have configured application "([^\"]*)" to the slot "([^\"]*)" of Quick Launch Bar$/ do |application, slot|
    setSlotConfiguration(slot, application)
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

Then /^Quick Launch Bar slot "([^\"]*)" contains application "([^\"]*)" icon$/ do |slot, application|
    ensureConfigurationUpdated()

    index = slot.to_i - 1
    qlbObject = @app.QuickLaunchBar.child(:name => 'QuickLaunchBarButton', :__index => index)
    verify_equal('LauncherButton') { qlbObject.type }
    verify_equal(applicationDesktopEntryName(application)) { qlbObject.attribute('desktopEntry') }
end


$configuration = {}
$configurationDirty = false

def ensureConfigurationUpdated()
    if $configurationDirty
        output = ''

        $configuration.each_pair do |s, a|
            output += "[#{s}]\n"
            output += "desktopFile=/tmp/#{a}.desktop\n"
        end

        target = File.expand_path('~/.config/duihome/quicklaunchbar.data')
        File.open(target, 'w') { |f|
            f.write(output)
        }

        $configurationDirty = false

        # Give some time for the application to react on the change
        sleep 2
    end
end

def setSlotConfiguration(slot, application)
    $configuration[slot] = application

    # Copy the widgetsgallery desktop entry to a temporary file
    File.copy('/usr/share/applications/widgetsgallery.desktop', '/tmp/' + application + '.desktop')

    $configurationDirty = true
end
