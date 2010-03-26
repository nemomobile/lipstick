require 'matti'
include MattiVerify

def close_duihome
    home = @sut.application(:name => 'duihome')
    if home != nil
        pid = home.attribute('processId')
        home.close
        if File.exists?('/proc' + pid + '/status')
            Process.kill(:SIGKILL, pid.to_i)
        end
    end
end

def launch_duihome
    begin
        @app = @sut.application(:name => 'duihome')
    rescue
        @app = @sut.run(:name => 'duihome')
    end
end


# Gets the home path of an application object
def getAppHome(app)
    File.expand_path("~" + getProcessOwnerName(app.attribute("processId")))
end

# Gets the user name of the owner of the process specified by pid.
# If the process doesn't exist, returns an empty string.
def getProcessOwnerName(pid)
    cmd = "ps -e -o user,pid | grep #{pid} | grep -v grep | awk '{print $1}'"
    owner = ""
    IO::popen(cmd) { |io| owner = io.gets }
    return owner.strip()
end

alias orig_verify verify
alias orig_verify_not verify_not

def verify(timeout = 30, message = nil, &block)
    orig_verify(timeout, message, &block)
end

def verify_not(timeout = 30, message = nil, &block)
    orig_verify_not(timeout, message, &block)
end

