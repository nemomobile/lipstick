require "dui.rb"

class Dui::Runner
    def teardown
        # The default implementation shuts down the application but we don't want that
        nil
    end
end
