# Gets the user name of the owner of the process specified by pid.
# If the process doesn't exist, returns an empty string.
def getProcessOwnerName(pid)
    cmd = "ps -o user,pid | grep #{pid} | grep -v grep | awk '{print $1}'"
    owner = ""
    IO::popen(cmd) { |io| owner = io.gets }
    return owner.strip()
end
