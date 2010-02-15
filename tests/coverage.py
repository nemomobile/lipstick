#!/usr/bin/python2.5
import sys,commands,re,os


def printxml(list,percentage):

    output = open('./coverage.log.xml','w')

    tests = len(list)
    fails = 0 
   
    
    print >>output, '<?xml version="1.0"?>'
    
    for row in list:
        (file,coveredp,coveredl,uncovered,excluded,reason) = row
        if coveredp < percentage:
            fails = fails + 1

    print >>output, '<testsuite name="CodeCoverage" tests="%d" failures="%d" errors="0" time="0.0">' % (tests,fails),

    for row in list:
        (file,coveredp,coveredl,uncovered,excluded,reason) = row
        print >>output 
        print >>output, '<testcase classname="line coverage" name="line coverage %s" time="0.0" ' % ( file ),
        if coveredp < percentage:
            print >>output, '>'
            if reason != None:
                msg = '0%% Coverage, Reason: %s' % (reason)
            else:
                msg  = 'Coverage: %s%% out of %s , (%d covered lines, %d uncovered, %d excluded)' %  ( str(coveredp), str(percentage),coveredl,uncovered,excluded )
            
            print >>output, ' <failure type="Standard" message="%s"/>' % ( msg )
            print >>output, '</testcase>',

        else:
            print >>output, '/>',

    print >>output,""
    print >>output, '</testsuite>'

    return fails 


def main(argv):
    if len(sys.argv) != 4:
        print "I need 3 arguments, " + str(len(sys.argv) - 1) + " given. "
        usage()

    # parse through arguments
    fileList = sys.argv[1].split(",")

    objectDirectory = sys.argv[2]


    results = []

    percentage = sys.argv[3]
    (t,y,exc) = (0,0,0)
    curdir = commands.getoutput("pwd")+"/"
    # run gcov for file1,file2,...
    results = []
    for singleFile in fileList:
        if singleFile != "":
            # singleFile = "../" + singleFile;
            singleFileWithoutPath = os.path.basename(singleFile)
            # print("cd .obj ; gcov --object-directory . " + singleFile );
            print ("gcov --object-directory .obj " + singleFile );
            commands.getoutput("gcov --object-directory . " + singleFile );
            #commands.getoutput("cd "+sd+";gcov --object-directory " +curdir+ objectDirectory +" "+ singleFile)
            try:
                #h= open(sd+i+".gcov","r") 
                h= open(singleFileWithoutPath + ".gcov","r");
            except:
                msg = "Unable to open file " + singleFileWithoutPath + ".gcov for reading"
                results.append( [singleFileWithoutPath,0,0,0,0,msg] )
            else:
                (e,r,o) = parse(h)
    
                name = singleFileWithoutPath
                coveredp = round(float(r)/(e+r)*100,2)
                coveredl = r
                uncovered =  e
                excluded = o
            
                results.append( [singleFileWithoutPath,coveredp,coveredl,uncovered,excluded,None] )
                print singleFileWithoutPath +  " covered "+str(round(float(r)/(e+r)*100,2))+" % ("+str(e)+" uncovered lines, "+str(r)+" covered lines, " + str(o) +" excluded lines, "+str(e+r+o)+" in total)" # print per-file coverage
        #(t,y,exc) = (t+e, r+y, exc+o) # sum up line counts

    ret = printxml(results,float(percentage))


    #print "Average coverage: " + str(round(float(y)/(t+y)*100,2))+" %" 
    #print "Total line coverage: "+str(t)+" uncovered lines, "+str(y)+" covered lines, " + str(exc) +" excluded lines, "+str(t+y+exc)+" in total)" # print per-file coverage
    # ret = (float(percentage)/100*(y+t) > y)
    if ret>0:
        print "error: code coverage below set limit"
    return 0

def usage():
    print "Usage: coverage.py file1,file2,... sourcedirect objectdirectory percentage"
    print
    print "Return values:"
    print " 0 check successful, coverage sufficient"
    print " 1 check successful, coverage insufficient"
    print " 2 check failed"
    sys.exit(2)

def parse (f):
    a = 0
    b = 0
    c = 0
    for i in f: # parse through the file
        s = i.split(":")[0];
        if s[-1]=="#":
            a += 1
        elif s[-1] != "-":
            b += 1
        else:
            c += 1    
    return (a,b,c); # return (uncovered lines, covered lines count)

sys.exit(main(sys.argv))
