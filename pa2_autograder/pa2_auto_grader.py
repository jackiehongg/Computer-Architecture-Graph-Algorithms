#! /usr/bin/python

import os, sys, glob, time, subprocess, signal
import popen2

subdirectories = ['first', 'second', 'third', 'fourth', 'fifth', 'sixth']
# if a program has single liner input and output, we put all test cases in single file. Otherwise, we have a file for test and associated file with results
formats = {'first':'file', 'second':'file', 'third':'file', 'fourth':'file', 'fifth':'file', 'sixth':'file'}

weight = {'first':1.0, 'second':0.5, 'third':2, 'fourth':1.0, 'fifth':4.0, 'sixth': 4.0}

scores = {'first':0, 'second':0, 'third':0, 'fourth':0, 'fifth':0, 'sixth':0}

test_cases_directory = ""


class ExperimentError(Exception):
    def __init__(self, command, output):
        self.command = command
        limit = 10000
        if(len(output) > limit):
          self.output = output[:limit/2] + "\n\n...TRUNCATED...\n\n" + output[-limit/2:]
        else:
          self.output = output
    def __str__(self):
        return "ExperimentError:" + `self.command`

def run_command(command_string, input_string="", max_lines=0, verbose=False, echo=False, throw_exception=True, return_valgrind_output=False, user_program=False):

#    print "executing:", command_string
    if echo:
        print "executing:", command_string
    obj = popen2.Popen4(command_string)
    output = ""
    valgrind_output = ""
#    print "1"
    obj.tochild.write(input_string)
    obj.tochild.close()
    valgrind_prefix = "==%d==" % obj.pid
    maxSleep = 100
#    print "2"
    if user_program: #program may have an infinite loop
        while maxSleep>0:
            time.sleep(0.25)
            maxSleep-=1
            if obj.poll()!=-1:
                break
        if maxSleep==0 and obj.poll()==-1:
            os.kill(obj.pid, signal.SIGKILL)
            print command_string, " taking longer than expected. Killed."
            return ""
#    print "3"
    line = obj.fromchild.readline()
    while (line):
        if verbose == 1:
            print line,
        if line.startswith(valgrind_prefix):
            valgrind_output += line
        output += line
        line = obj.fromchild.readline()
    exit_status = obj.wait()
 #   print "4"
    if(max_lines != 0):
        lines = output.split("\n");
        output = string.join(lines[-max_lines:], "\n")

    if throw_exception and exit_status != 0:
        raise ExperimentError(command_string, output)

    if return_valgrind_output:
        return valgrind_output
    else:
        return output

def compare_string_file(ref_file, test_string, show_difference=False):
    test_list=test_string.split("\n")
    fd = open(ref_file)
    i=0
    flag=True

    for line in fd:
        if i<len(test_list):
            if line.strip()!=test_list[i].strip():
                print "Observed:", test_list[i].strip();
                print "Expected:", line.strip();
                flag=False
        elif len(line.strip())>0:
            print "Output missing: ", line
            flag=False
        i+=1
    
    fd.close()
    while(i<len(test_list)):
        if len(test_list[i].strip())==0:
            i+=1
            continue
        print "Extra output: ", test_list[i]
        i+=1
        flag=False

    return flag

def compare_string(ref, test):
    ref = ref.strip()
    test = test.strip()

    if(ref==test):
        return True
    if(ref==test.lower()):
        print "%s and %s are in different case. Please print your output in correct case."%(ref, test)
    return False


def make_executable(dirname):
    if os.path.isfile('Makefile') or os.path.isfile('makefile'):
        run_command("make clean", verbose=False)
        run_command("make", verbose=False)
    else:
        print "No Makefile found in", dirname
        print "Please submit a Makefile to receive full grade."
        run_command("gcc -Wall -Werror -fsanitize=address -o %s *.c *.h"%(dirname), verbose=False)


def file_grade(dirname):
    print "Grading", dirname
    prevdir = os.getcwd()
    os.chdir(dirname)
    score = 0

    try:
        make_executable(dirname)
    except:
        print "An exception occured trying to execute %s"%(dirname)
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return
    
    if not os.path.isfile(dirname):
        print "Executable %s missing. Please check the compilation output."%(dirname)
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return

    test_dir = test_cases_directory + dirname  + "/"

    for testfile in sorted(os.listdir(test_dir)):
        if os.path.isdir(testfile) or not testfile.startswith("graph"):
            continue
        resultfile = test_dir + "result"+testfile[5:len(testfile)]
        queryfile = test_dir + "query"+testfile[5:len(testfile)]

#        print "query file is", queryfile

        if not os.path.isfile(queryfile) and not dirname.startswith("fourth"):
            print "Found a graph file %s. but no associated query file %s."%(testfile, queryfile)
            continue
        
        if not os.path.isfile(resultfile):
            print "Found a graph file %s. but no associated result file."%(testfile)
            continue
        
        try:
            if dirname.startswith("fourth"):
                ret = run_command("./%s %s"%(dirname, test_dir + testfile), user_program=True, verbose=False);
            else:
                ret = run_command("./%s %s %s"%(dirname, test_dir + testfile, queryfile), user_program=True, verbose=False);
        except:
            print "An exception occurred while executing %s %s %s"%(dirname, testfile, queryfile)
            os.chdir(prevdir)
            scores[dirname] = score
            print "Score is ", score
            return

        if compare_string_file(resultfile, ret, show_difference=True):
            score = score + weight[dirname]
        else:
            print "The output is not correct for input file %s."%(testfile)
    print "Score is ", score
    scores[dirname] = score
    print ""
    print ""
    os.chdir(prevdir)



def line_to_file_grade(dirname):
    print "Grading", dirname
    prevdir = os.getcwd()
    os.chdir(dirname)
    score = 0

    try:
        make_executable(dirname)
    except:
        print "An exception occured trying to execute %s"%(dirname)
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return
    
    if not os.path.isfile(dirname):
        print "Executable %s missing. Please check the compilation output."%(dirname)
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return

    test_dir = test_cases_directory + dirname  + "/"
    
    test_file = test_cases_directory + dirname + "/graph.txt"
    fd = open(test_file)
    count = 0;

    for line in fd:
        inputline = line
        count = count + 1
        resultfile = test_dir + "result" + str(count) + ".txt"
        try:
            command_str = "./%s %s"%(dirname, inputline.strip())
            ret = run_command(command_str, user_program=True)
            if compare_string_file(resultfile, ret, show_difference=True):
                score = score + weight[dirname]
        except:
            os.chdir(prevdir)
            print "An exception occured trying to execute %s" % (command_str)
            print "Score is ", score
    fd.close()
    print "Score is ", score
    scores[dirname] = score
    print ""
    print ""
    os.chdir(prevdir)
    
    
def single_grade(dirname):
    print "Grading", dirname
    prevdir = os.getcwd()
    os.chdir(dirname)
    #os.getcwd()
    score = 0

    try:
        make_executable(dirname)
    except:
        print "An exception occured trying to execute %s"%(dirname)
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return


    if not os.path.isfile(dirname):
        print "Executable %s missing. Please check the compilation output."%(dirname)
        print "Score is ", score
        print ""
        print ""
        os.chdir(prevdir)
        return

    test_file = test_cases_directory + dirname + "/test.txt"

    if not os.path.isfile(test_file):
        print "Expecting the test cases in test.txt. Not found."
	print "Score is ", score
	print ""
	print ""
	os.chdir(prevdir)
	return
    else:
        print ""
#        print "Using test.txt for grading."
        

    fd = open(test_file)
    state = 0
    for line in fd:
        if state==0:
            inputline = line
            state = 1
        else:
            outputline = line
            state = 0

            try:
	        command_str = "./%s %s"%(dirname, inputline.strip())
#		command_str = "pwd"

#                print "command_str is %s"%(command_str)

                ret = run_command(command_str, user_program=True)
#                ret = run_command(command_str, verbose=True)
#                print "Your program generated %s. The correct answer is %s."%(ret.strip(), outputline.strip())
                if compare_string(outputline, ret):
#                    print "The output is correct for input %s."%(inputline.strip())
    		    score = score + weight[dirname]
            except:
        	os.chdir(prevdir)
                print "An exception occured trying to execute %s"%(command_str)
        	print "Score is ", score
    fd.close()
    print "Score is ", score
    scores[dirname] = score
    print ""
    print ""
    os.chdir(prevdir)
    

def global_grade(dirname):
    #print "dirname is ", dirname
    target = len(subdirectories)
    for subdir in subdirectories:
	#print "looking now for -", os.path.join(subdir), "-"
        if not os.path.isdir(os.path.join(subdir)):
	    #print "will skip ", subdir
            continue
        print subdir, " found!"
        if subdir in formats and formats[subdir]=='line':
            line_to_file_grade(subdir)
        elif subdir in formats and formats[subdir]=='file':
            file_grade(subdir)
        
            
if __name__ == '__main__':
    basepath = "pa2"
    tarmode = False #by default check the directory
    
    test_cases_directory = os.getcwd() + "/testcases/"
    if len(sys.argv)>1:
        if sys.argv[1].strip().endswith("tar"):
            tarmode=True

    if tarmode==False:
        if not os.path.isdir(basepath):
            print "pa2 is not present in this directory."
            sys.exit(1)
        else:
            print "Grading the content of pa2."
            os.chdir(basepath)
            global_grade(basepath)

    else:
        prevdir = os.getcwd()
	print "dir is ", prevdir
        if not os.path.exists("pa2.tar"):
            print "Expecting pa2.tar in current directory. Current directory is %s"%(prevdir)
            print "Please make sure you created pa2.tar in the right directory"
            sys.exit(1)
        if os.path.exists("obj_temp"):
            print "Deleting the directory obj_temp."
            run_command("rm -rf obj_temp", verbose=False)
        run_command("mkdir obj_temp", verbose=False)
        os.chdir("obj_temp")
        run_command("tar -xvf ../pa2.tar")
        if os.path.isdir("pa2"):
            os.chdir("pa2")
            global_grade("pa2")
        else:
            print "There is not directory named pa2 in pa2.tar."
            print "Please check your tar file."
        os.chdir(prevdir)
    Total_score = 0.0
    print 'You scored '
    for key, value in scores.iteritems():
        Total_score = Total_score + value
        print str(value), ' in ', key

    print "Your TOTAL SCORE = ", Total_score, "/75"
    print "Your assignment will be graded for another 75 points with test cases not given to you"
