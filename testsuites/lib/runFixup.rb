require "dui.rb"

class Dui::Runner
    alias :real_runTestApp :runTestApp
    def runTestApp app, args
        # Attempt to run the test application. If that fails, try to lookup
        # the application as it may be already running.
        begin
            real_runTestApp(app, args)
        rescue MobyBase::BehaviourError
            @sut.application(:name => app)
        end
    end
end
