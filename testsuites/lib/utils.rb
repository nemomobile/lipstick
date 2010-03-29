require 'test/unit'
require 'open3'

# The location of the helper scripts

$duihome_helpers = '/usr/share/duifw-home-tests/helpers'

# Create a file using sudo and a helper script.

def create_file name, content
    cmd = "/usr/bin/sudo #{$duihome_heplers}/create-file.sh #{name}"
    execute cmd, content
end

# Remove a file using sudo and a helper script.

def remove_file name
    cmd = "/usr/bin/sudo #{$duihome_helpers}/remove-file.sh #{name}"
    execute cmd
end

# Execute a command "cmd" with possible input "input", return the text
# output by the command to stdout. In case of error, raise a
# VerificationError (unless check=false).

def execute cmd, input=nil, check=true

    cmdin, cmdout, cmderr = Open3::popen3(cmd)
    cmdin.puts input unless input.nil?
    cmdin.close

    out = cmdout.readlines.join
    cmdout.close

    err = cmderr.readlines.join
    cmderr.close

    verify_equal("", 0, "Errors executing command") { err } if check

    out
end

