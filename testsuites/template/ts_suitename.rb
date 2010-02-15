#!/usr/bin/ruby 

# Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# Contact: Nokia Corporation (directui@nokia.com)
#
# This file is part of duihome.
#
# If you have questions regarding the use of this file, please contact
# Nokia at directui@nokia.com.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1 as published by the Free Software Foundation
# and appearing in the file LICENSE.LGPL included in the packaging
# of this file.

$_MATTI_DISABLE_STATS_REPORTING = true
PATH=File.dirname(__FILE__)
CASES=File.join(PATH,"cases")
$LOAD_PATH << File.join(PATH,"..","lib")
require "dui.rb"

# Loads all the testcase files
# TODO: Move to lib/dui.rb ?
Dir.entries(CASES).each do |file|
  require File.join(CASES,file)  if file =~ /^tc_.*\.rb$/
end


suite = Dui::Runner.new
# NOTICE: Dont pass -hardware or -software anymore unless you explicitly
# require either of them, hw acceleration will be automatically probed
# at runtime via dui.rb library
suite.start('ta_suitename','')

