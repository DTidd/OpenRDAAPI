#!/usr/bin/python

# Import globally defined options
#Import([ 'env', 'idir_lib' ])
import os
import platform
#import pprint
import datetime
#import logging
#import re
import sys
#import glob
import fnmatch

#logging.info("delobj: ")
#logging.info("delobj: %s", GetOption('delobj'))

if GetOption('delobj'):
	print 'delobj=true'
else:
	print 'delobj=false'

#if GetOption('use_exdir'):
#	print 'use_exdir=true'
#else:
#	print 'use_exdir=false'
#	print "USE_EXDIR: "+str(GetOption('use_exdir'))


#print "Default SCons Debug Settings: "+str(GetOption('warn'))
SetOption('warn','no-duplicate-environment')
SetOption('warn','no-python-version')
#print "Current SCons Debug Settings: "+str(GetOption('warn'))


#if GetOption('GUI_TYPE') == 'WT':
#	print 'GUI_TYPE=WT\n'
#else:
#	print 'GUI_TYPE!=WT\n'


Import('localc_dir')
Import('build_dir')
Import('LOCAL_CPP')

if not os.path.isdir(GetOption('MYSQL_PATH_VALUE')):
	print 'Invalid Value for MYSQL PATH, [%s] does not exist.' % GetOption('MYSQL_PATH_VALUE')
	print 'Defaulting to MYSQL PATH [%s] instead.' % '/opt/mysql-5.0.37-rda'
else:
	MYSQL_PATH=GetOption('MYSQL_PATH_VALUE')


#print "build_dir="+build_dir


# Begin Custom Function Defines

def print_cmd_line(s, target, source, env):
	sys.stdout.write("Building %s -> %s...\n" %
		(' and '.join([str(x) for x in source]),
		' and '.join([str(x) for x in target])))
	#env=Environment(PRINT_CMD_LINE_FUNC=print_cmd_line)
	#env.Program('foo', 'foo.c')


def purge_target_oparts(targetname, str1, list1):
		
		if targetname in COMMAND_LINE_TARGETS:

			list2=list(((str1.replace('.cpp','.o').replace('.c','.o')+' ')+str(list1).replace('.cpp','.o').replace('.c','.o').replace('[','').replace(']','').replace("'",'').replace(',','')).split()) 

			for curr_str in list2:
				try:
					if os.path.exists(build_dir+'/'+curr_str):
						print 'Removing '+build_dir+'/'+curr_str+'...'
						os.remove(build_dir+'/'+curr_str)
				except Exception, e:
					print 'Error removing: ['+build_dir+'/'+curr_str+']'


def get_addlibs_buildpgms(self,str1):

	my_addlib_line_count=-1

	for line in open(localc_dir+'/'+str1):
		++my_addlib_line_count
		#self.AppendUnique(LIBS = ['pgmlib'+str1.replace('/','').replace('.c','')] ) 
		if "ADDLIB" in line:
			my_addlib_line_count=0
			line=line.replace(' ','')
			line=line.replace('/*ADDLIB','')
			line=line.replace('*/\n',"")
				
			if line == 'WT':

				if GetOption('debug_build') == 2:
					#self.AppendUnique(LIBS = ['wtd'] ) 
					self.AppendUnique(LIBS = ['wt'] ) 
					#self.AppendUnique(LIBS = ['wtext'] ) 
					if GetOption('use_fastcgi') == 1:
						#self.AppendUnique(LIBS = ['wtfcgid'] ) 
						self.AppendUnique(LIBS = ['wtfcgi'] ) 
					else:
						#print 'Using WT HTTP...'
						#self.AppendUnique(LIBS = ['wthttpd'] ) 
						self.AppendUnique(LIBS = ['wthttp'] ) 
				else:
					self.AppendUnique(LIBS = ['wt'] ) 
					#self.AppendUnique(LIBS = ['wtext'] ) 
					if GetOption('use_fastcgi') == 1:
						self.AppendUnique(LIBS = ['wtfcgi'] ) 
					else:
						#print 'Using WT HTTP...'
						self.AppendUnique(LIBS = ['wthttp'] ) 

				self.AppendUnique(LIBS = ['boost_system'] ) 
				self.AppendUnique(LIBS = ['boost_program_options'] ) 
				self.AppendUnique(LIBS = ['boost_signals'] ) 
				self.AppendUnique(LIBS = ['boost_thread'] ) 
				self.AppendUnique(LIBS = ['boost_regex'] ) 
				self.AppendUnique(LIBS = ['boost_date_time'] ) 
				self.AppendUnique(LIBS = ['boost_filesystem'] ) 

			else:
				self.AppendUnique(LIBS = [line] ) 

			#if line == 'add':
			#elif line == 'chglib':
			#elif line == 'cscn':
			#elif line == 'ctpf':
			#elif line == 'curl':
				#self.AppendUnique(LIBS = ['curl'] ) 
			#elif line == 'cver':
			if line == 'gui':
				#print "ADDLIB: gui"
				if GetOption('GUI_TYPE') == 'WT':
	       #g++ -Wno-deprecated -I/usr/include -I/usr/local/include -I/usr/include/c++ -I/opt/boost_1_45_0 -I. -L/lib -L/usr/lib -L/usr/local/lib -L/opt/boost_1_45_0/stage/lib -lwt -lwtext -lwthttp -lboost_system -lboost_program_options -lboost_signals -lboost_thread -lboost_regex -lboost_date_time -lboost_filesystem -o wt_test.lnx guibridgeapi.o wt_test.o 
					#self.AppendUnique(LIBS = ['guibridge'] ) 
					self.AppendUnique(LIBS = ['wt'] ) 
					#self.AppendUnique(LIBS = ['wtext'] ) 
					if GetOption('use_fastcgi') == 1:
						self.AppendUnique(LIBS = ['wtfcgi'] ) 
					else:
						#print 'Using WT HTTP...'
						self.AppendUnique(LIBS = ['wthttp'] ) 

					self.AppendUnique(LIBS = ['boost_system'] ) 
					self.AppendUnique(LIBS = ['boost_program_options'] ) 
					self.AppendUnique(LIBS = ['boost_signals'] ) 
					self.AppendUnique(LIBS = ['boost_thread'] ) 
					self.AppendUnique(LIBS = ['boost_regex'] ) 
					self.AppendUnique(LIBS = ['boost_date_time'] ) 
					self.AppendUnique(LIBS = ['boost_filesystem'] ) 
				else:
					self.AppendUnique(LIBS = ['qtc'] ) 
					if platform.system() == 'Linux':
						self.AppendUnique(LIBS = ['rdaqt-mt'] ) 
						self.AppendUnique(LIBS = ['rdaqui'] ) 
						self.AppendUnique(LIBS = ['Xpm'] ) 
					if platform.system() == 'Windows':
						self.AppendUnique(LIBS = ['qt-mt3'] ) 
						self.AppendUnique(LIBS = ['qui'] ) 

#			elif line == 'guibridge':
#				if GetOption('GUI_TYPE') == 'WT':
#					self.AppendUnique(LIBS = ['guibridge'] ) 
#					self.AppendUnique(LIBS = ['wt'] ) 
#					self.AppendUnique(LIBS = ['wtext'] ) 
#					if GetOption('use_fastcgi') == 1:
#						self.AppendUnique(LIBS = ['wtfcgi'] ) 
#					else:
#						print 'Using WT HTTP...'
#						self.AppendUnique(LIBS = ['wthttp'] ) 


					self.AppendUnique(LIBS = ['boost_system'] ) 
					self.AppendUnique(LIBS = ['boost_program_options'] ) 
					self.AppendUnique(LIBS = ['boost_signals'] ) 
					self.AppendUnique(LIBS = ['boost_thread'] ) 
					self.AppendUnique(LIBS = ['boost_regex'] ) 
					self.AppendUnique(LIBS = ['boost_date_time'] ) 
					self.AppendUnique(LIBS = ['boost_filesystem'] ) 

			elif line == 'misc':
				self.AppendUnique(LIBS = ['cyassl'] ) 
				if platform.system() == 'Windows':
					self.AppendUnique(LIBS = ['ctaocrypt'] ) 
			#elif line == 'mix':
			#elif line == 'mklib':
			#elif line == 'mkmisc':
			#elif line == 'mkvir':
			elif line == 'nrd':
				#if LOCAL_CPP:
				#	self.AppendUnique(LIBS = ['mysqlcppconn-static'] ) 

				if GetOption('enable_mysql_server') == 1:
					self.AppendUnique(LIBS = ['mysqlclient'] ) 

				if GetOption('enable_myisam_engine') == 1:
					self.AppendUnique(LIBS = ['myisam'] ) 
					self.AppendUnique(LIBS = ['mysys'] ) 
					self.AppendUnique(LIBS = ['dbug'] ) 
					if platform.system() == 'Linux':
						self.AppendUnique(LIBS = ['mystrings'] ) 
					if platform.system() == 'Windows':
						self.AppendUnique(LIBS = ['strings'] ) 
						self.AppendUnique(LIBS = ['yassl'] ) 
						self.AppendUnique(LIBS = ['taocrypt'] ) 
						self.AppendUnique(LIBS = ['wsock32'] ) 
			#elif line == 'olh':
			#elif line == 'rpt':
			#elif line == 'sec':
			#elif line == 'trans':

		else:
			if my_addlib_line_count != -1:
				++my_addlib_line_count
				if my_addlib_line_count == 10:
					break	
				
	#if SCONS_HACK_PROG.count(str1):
	#	print "Adding hacklib to program: ["+str1+"]"
	if platform.system() == 'Linux':
		self.AppendUnique(LIBS = ['m']) 
		self.AppendUnique(LIBS = ['c'] ) 
		self.AppendUnique(LIBS = ['pthread'] ) 
		self.AppendUnique(LIBS = ['uuid'] ) 

	if platform.system() == 'Windows':
		self.AppendUnique(LIBS = ['kernel32']) 
		self.AppendUnique(LIBS = ['user32']) 
		self.AppendUnique(LIBS = ['gdi32']) 
		self.AppendUnique(LIBS = ['winspool']) 
		self.AppendUnique(LIBS = ['comdlg32']) 
		self.AppendUnique(LIBS = ['advapi32']) 
		self.AppendUnique(LIBS = ['shell32']) 
		self.AppendUnique(LIBS = ['ole32']) 
		self.AppendUnique(LIBS = ['oleaut32']) 
		self.AppendUnique(LIBS = ['uuid']) 
		self.AppendUnique(LIBS = ['rpcrt4']) 
		self.AppendUnique(LIBS = ['odbc32']) 
		self.AppendUnique(LIBS = ['odbccp32']) 




AddMethod(Environment, get_addlibs_buildpgms)


def get_oparts_builddir(str1):
	my_opart_line_count=-1
	list=[ ]
	x=0
	if str1.count('/') == 1 & str1.count('.'):
		str2 = str1.replace('/',' ')
		str2 = str2.replace('.',' ')
		mstr = str2.split()
		mstr[0] = mstr[0]+'/'
		#mstr[2] = '.'+mstr[2]
	

	for line in open(localc_dir+'/'+str1):
		if "CPP_OPART" in line:
			my_opart_line_count=0
			line=line.replace(' ','')
			line=line.replace('/*CPP_OPART','')
			line=line.replace('*/\n','.cpp')


			if platform.system() == 'Windows':
				list.append([localc_dir+'\\'+mstr[0]+line])                         
			else:
				list.append([mstr[0]+line])                         

			#print 'Remove File? '+line.replace('cpp','o')
			
		elif "OPART" in line:
			my_opart_line_count=0
			line=line.replace(' ','')
			line=line.replace('/*OPART','')
			line=line.replace('*/\n','.c')
			
			if platform.system() == 'Windows':
				list.append([localc_dir+'\\'+mstr[0]+line])                         
			else:
				list.append([mstr[0]+line])                         

			#print 'Remove File? '+line.replace('c','o')
		else:
			if my_opart_line_count != -1:
				++my_opart_line_count
				if my_opart_line_count == 10:
					break

		#if str2 == "domenu":
		#	print "str1: "+str1+" str2: "+str2+" str3: "+str3
		#	print "OPART: "+line
	
	x=0

	# Do a replace on the strings to change '.c' to '.o' and 
	# then an os.unlink for them if -a sets a scons variable...
	# to simulate mk -ac
	return [list]


def PrintENV(self):
	#dict = base_env.Dictionary()
	#dict = rdalibs_misc_env.Dictionary()
	dict = self.Dictionary()
	keys = dict.keys()
	keys.sort()
	for key in keys:
		#print "construction variable = '%s', value = '%s'" % (key, dict[key])
		print "var = '%s', value = '%s'" % (key, dict[key])
	#	print "var = '%s'" % (key)
                
AddMethod(Environment, PrintENV)


def buildpgms2(self,str1):
	
	if str1.count('/') == 1 & str1.count('.'):
		str2 = str1.replace('/',' ')
		str2 = str2.replace('.',' ')
		mstr = str2.split()
		#mstr[0] = mstr[0]+'/'
		#mstr[2] = '.'+mstr[2]

	myenv = self.Clone()
	mysrc=get_oparts_builddir(str1) 
	#print "pgm ["+str1+"]  oparts: "+str(mysrc)+"  "+build_dir
	
	if "sftp" in str1 or "ssh" in str1:
		if os.path.exists('/usr/local/lib/libssh-static.a') or os.path.exists('/opt/libssh/lib/libssh-static.a'):
       	 		myenv.Append(LIBS = [ 'libssh-static' ])
		else:
       	 		myenv.Append(LIBS = [ 'libssh' ])
       	 	myenv.Append(LIBS = [ 'libssl' ])
       	 	myenv.Append(LIBS = [ 'libcrypto' ])
       	 	#myenv.Append(LIBS = [ 'z' ])
        	#myenv.Append(LIBS = [ 'dl' ])
        	myenv.Append(LIBS = [ 'rt' ])
		#myenv.PrintENV() 

	
	if GetOption('delobj') == 1:
		purge_target_oparts(mstr[1],str1,mysrc)

#		if mstr[1] in COMMAND_LINE_TARGETS:
#			myobjs=list(((str1.replace('.cpp','.o').replace('.c','.o')+' ')+str(mysrc).replace('.cpp','.o').replace('.c','.o').replace('[','').replace(']','').replace("'",'').replace(',','')).split()) 
#
#			for curr_str in myobjs:
#				try:
#					if os.path.exists(build_dir+'/'+curr_str):
#						print 'Removing '+build_dir+'/'+curr_str+'...'
#						os.remove(build_dir+'/'+curr_str)
#				except Exception, e:
#					print 'Error removing: ['+build_dir+'/'+curr_str+']'


				

#			while True:
#				try:
#					curr_str=list.pop(myobjs)
#					try:
#						if os.path.exists(build_dir+'/'+curr_str):
#							print 'Removing '+build_dir+'/'+curr_str+'...'
#							os.remove(build_dir+'/'+curr_str)
#					except Exception, e:
#						print 'Error removing: ['+build_dir+'/'+curr_str+']'
#
#				except Exception, e:
#					break
	myenv.get_addlibs_buildpgms(str1) 
	#myenv.PrintENV() 

	#myenv = self.Clone()

	#'CCCOM', value = '$CC -o $TARGET -c $CFLAGS $CCFLAGS $_CCCOMCOM $SOURCES'

	#if platform.system() == 'Linux':
	#        myenv['CCCOM'] = [ '$CC -o $TARGET -c -D__NAM__=\\"'+mstr[1]+'.lnx\\"  $CFLAGS $CCFLAGS $_CCCOMCOM $SOURCES ' ]

	#        myenv.Append(CPPDEFINES = [ '__NAM__=\\"'+mstr[1]+'.lnx\\"' ])
	#	if GetOption('GUI_TYPE') == 'WT':
	#        	myenv.Append(CPPDEFINES = [ 'CPPMAIN' ])

	#if platform.system() == 'Windows':
        #	myenv.Append(CPPDEFINES = [ '__NAM__=\\"'+mstr[1]+'.exe\\"' ])

	return myenv.Program(target=mstr[1], source=[ str1, mysrc ])

	#if platform.system() == 'Linux':
	#        myenv.Append(CPPDEFINES = [ '__NAM__=\\"'+mstr[1]+'.lnx\\"' ])
	#	if GetOption('GUI_TYPE') == 'WT':
	#        	myenv.Append(CPPDEFINES = [ 'CPPMAIN' ])

	#if platform.system() == 'Windows':
        #	myenv.Append(CPPDEFINES = [ '__NAM__=\\"'+mstr[1]+'.exe\\"' ])


	#	print"i="+i
	#	if i == str1:
	#		print "String is in hack lib list "+str1

	#mysrc=get_oparts_builddir(str1) 

	#myenv.StaticLibrary('pgmlib'+str1.replace('/','').replace('.c',''), mysrc )

	#myenv.get_addlibs_buildpgms(str1) 


	#if SCONS_HACK_PROG.count(str1):
		#print "Adding hacklib to program: ["+str1+"]"
		#self.AppendUnique(LIBS = ['sconshacklib'] ) 
		#print myenv['LIBS']
	#return myenv.Program(target=mstr[1], source=[ str1, mysrc ])

AddMethod(Environment, buildpgms2)

def buildpgms3(self,str1,str2):
	
	if os.access(localc_dir+'/'+str1, os.R_OK)!=1:
		print 'Can not find file: "'+str1+'"'
		return
	build_result=rda_apps_env.buildpgms2(str1)

	if platform.system() == 'Windows':
	# #2 for library #1 for exe
	        rda_apps_env.AddPostAction(build_result, 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1')
	#        env.AddPostAction(build_result, 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2')

	#if platform.system() == 'Linux':
	#        rda_apps_env.AddPostAction(build_result, 'echo "Post Build Action For Target: ${TARGET}"')

	mystr=str1.replace('/',' ')
	if ".cpp" in str1:
		mystr=mystr.replace('.cpp','')

	elif ".c" in str1:
		mystr=mystr.replace('.c','')

	mylist=mystr.split()


	#print 'str1: '+str1+'  mystr: '+mystr+'  mylist[0]: '+mylist[0]+'  mylist[1]: '+mylist[1]
	Alias(mylist[1],build_result) 
	for x in str2.split(','):
		try:
			MODS[x].append(build_result)
			#print x+' '+str(type(MODS[x]))
		except Exception, e:
			MODS[x]=list()
			MODS[x].append(build_result)
			#print 'Exception: '+str(type(mods[x]))
		
	return 


AddMethod(Environment, buildpgms3)

# Type: 'scons program' to build the production program,
# 'scons debug' to build the debug version.
Help("""

  Command line options:

     -c			=	This tells scons to delete all objects for the specified targets
     				AND for the dependencies of the specified targets. 

     -j#		=	This sets how many concurrent processors are used when building.

     --strip		=	This uses the strip command on linux binaries being built.
     --no-strip		=	This disables the strip command.  Default for debug builds.

     --use-exdir	=	Moves binaries to $EXDIR after being built.  This is the default.
     --no-exdir		=	Leaves binaries in the /build/$BUILDENV directory.  This is the default for Wt.

     --use-qadir	=	Moves binaries to /rdadev/QA/$BUILDENV directory after being built.  

     --gui		=	Options are QT or WT.  Default is QT. 


  Here are some valid example build targets:
  misc nrd gui trans olh sec mix rpt add mklib mkvir mkmisc mbplus chglib cver
  cscn mkgag tools viewpdf vmimesendmail makelibs 3rdparty 
  
  Example1: \"scons misc nrd gui trans olh sec mix rpt\" 
  Example2: \"scons makelibs\" 
  Example3: \"scons viewpdf vmime-sendmail\" 
  Example4: \"scons 3rdparty\" 
  Example5: \"scons domenu\" 
  Example6: \"scons ALLPGMS\" 
  Example7: \"scons --gui=WT guibridge\" 
  Example8: \"scons --gui=WT domenu\" 


  To clean the objects from the workspace, add the \"-c\" option to the command.



""")


# End Custom Function Defines


#SetOption('num_jobs', num_cpu)
#print "Using -h", GetOption('help')
if len(COMMAND_LINE_TARGETS)==0:
	SetOption('help', True)

#print "BUILD_TARGETS is", map(str, BUILD_TARGETS)




#Import('baseEnv')

base_env = Environment()
base_env.SConsignFile(build_dir+'/.sconsign')
print base_env.Dump('CCFLAGS')
#print base_env.Dump('TARGETS')

# Use alternate print function instead of displaying actual build commands...
# base_env = Environment(PRINT_CMD_LINE_FUNC=print_cmd_line)


#env = Environment(ENV = {'INCLUDE' : os.environ['INCLUDE']})
conf = base_env.Configure()

#print platform.system()

if platform.system() == 'Linux':

	if GetOption('useclang'):
		conf.env.Replace(CC = [ '/opt/clang/bin/clang' ])
		conf.env.Replace(CXX = [ '/opt/clang/bin/clang++' ])
        	#conf.env.Append(CFLAGS = [ '-std=gnu89' ])
        	conf.env.Append(CFLAGS = [ '-std=gnu99', '-fcatch-undefined-behavior' ])
        	conf.env.Append(CXXFLAGS = [ '-Wold-style-cast', '-fcatch-undefined-behavior', '-Wpointer-arith', '-Wno-error=pointer-arith', '-Weverything' ])
	else:
        #	conf.env.Append(CFLAGS = [ '-std=c99' ])
		if os.path.exists('/usr/bin/gcc-4.7'):
			conf.env.Replace(CC = [ '/usr/bin/gcc-4.7' ])
		if os.path.exists('/usr/bin/g++-4.7'):
			conf.env.Replace(CXX = [ '/usr/bin/g++-4.7' ])

		#conf.env.Replace(LINK = [ '/usr/bin/ld.gold.real' ])

#	if GetOption('GUI_TYPE') == 'WT':
#		if os.path.exists('/opt/boost-1_54/lib'):
#        		conf.env.Append(LIBPATH = [ '/opt/boost-1_54/lib' ])
#		if os.path.exists('/opt/boost-1_54/include'):
#        		conf.env.Append(CPPPATH = [ '/opt/boost-1_54/include' ])


        #conf.env.Append(CPPDEFINES = 'POSIX')
        conf.env.Append(CPPPATH = [ '/usr/include', '/usr/local/include' ])
	if GetOption('GUI_TYPE') != 'WT':
        	conf.env.Append(CPPPATH = [ '/usr/X11R6/include' ])
        conf.env.Append(LIBPATH = [ '/lib', '/usr/lib', '/usr/local/lib' ])
#        conf.env.Append(LINKFLAGS = [ '-Xlinker', '--demangle' ])
        conf.env.Append(LINKFLAGS = [ '-Xlinker', '--no-demangle' ])
	
        #conf.env.Append(CXXFLAGS = [ '-Wno-nonnull' ])
        #conf.env.Append(CXXFLAGS = [ '-Wno-pmf-conversions' ])

	if GetOption('enable-warnings') == 1:
		print 'Enabling Display of GCC/G++ Warnings...'
	else:
		print 'Disabling Display of GCC/G++ Warnings...'
	        conf.env.Append(CFLAGS = [ '-w' ])
	        conf.env.Append(CXXFLAGS = [ '-w' ])

if platform.system() == 'Windows':
#	base_env['LINKCOM'] = [base_env['LINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1']
#        base_env['SHLINKCOM'] = [base_env['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']
        conf.env.Append(CPPDEFINES = [ 'WIN32' ])
        conf.env.Append(CPPDEFINES = [ '_WIN32' ])
        conf.env.Append(CPPDEFINES = [ 'MSWIN32' ])
        conf.env.Append(CPPDEFINES = [ '_CRT_SECURE_NO_DEPRECATE' ])
        conf.env.Append(CPPDEFINES = [ '_CRT_NONSTDC_NO_DEPRECATE' ])

#        conf.env.Append(LINKFLAGS = [ '/MANIFEST' ])
#        conf.env.Append(LINKFLAGS = [ '/STACK:10000000' ])
#        conf.env.Append(LINKFLAGS = [ '/machine:I386' ])
        #conf.env.Append(LINKFLAGS = [ '/subsystem:windows' ])
        #conf.env.Append(LINKFLAGS = [ '/subsystem:console' ])
        conf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:libcmt.lib' ])
        conf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:libcmtd.lib' ])
        conf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:msvcrtd.lib' ])

        conf.env.Append(CPPPATH = [ 'C:\\MSVC8\\VC\\INCLUDE' ])
        conf.env.Append(CPPPATH = [ 'C:\\WIN2003SDK\\Include' ])

        conf.env.Append(LIBPATH = [ 'C:\\MSVC8\\VC\\Lib' ])
        conf.env.Append(LIBPATH = [ 'C:\\WIN2003SDK\\Lib' ])


#print os.name

#if conf.CheckCHeader('zlib.h'):
#        print 'Found zlib.h, exiting!'
        #conf.conf.env.Append('-DHAS_ZLIB')

#if conf.CheckCHeader('jpeglib.h'):
#        print 'Found libjpeg.h, exiting!'
#        conf.conf.env.Append('-DHAS_FOO_H')

#if conf.CheckCHeader('ps_list.h'):
#        print 'Found ps_list.h, exiting!'
#        conf.conf.env.Append('-DHAS_FOO_H')

#if not conf.CheckLibWithHeader('libjpeg', 'jpeglib.h', 'c'):
#        print 'Did not find libjpeg.a or jpeg.lib, exiting!'
        #Exit(1)

#if not conf.CheckLib('somelib'):
#	print 'The library "somelib" was not found on this system.'
	#add_file_to_emitter(env, 'PROGEMITTER', File('somelibreplacement.c'))
conf.Finish()

rdalibs_env = base_env.Clone()
conf = rdalibs_env.Configure()
if platform.system() == 'Linux':
	       #g++ -Wno-deprecated -I/usr/include -I/usr/local/include -I/usr/include/c++ -I/opt/boost_1_45_0 -I. -L/lib -L/usr/lib -L/usr/local/lib -L/opt/boost_1_45_0/stage/lib -lwt -lwtext -lwthttp -lboost_system -lboost_program_options -lboost_signals -lboost_thread -lboost_regex -lboost_date_time -lboost_filesystem -o wt_test.lnx guibridgeapi.o wt_test.o 

	#if LOCAL_CPP:
	#	conf.env.AppendUnique(CPPPATH = [ '/opt/mysql-connector-c++/include' ])
	#	conf.env.Append(LIBPATH = [ '/opt/mysql-connector-c++/lib' ])
	
	
	if GetOption('use_rdadiag') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_RDA_DIAGNOSTICS' ])


	if GetOption('GUI_TYPE') == 'WT':

        	conf.env.Append(CPPDEFINES = [ 'CPPMAIN' ])
		
		if GetOption('use_fastcgi') == 1:
        		conf.env.Append(CPPDEFINES = [ 'WT_FASTCGI' ])
		#if os.path.exists(path):
		#os.path.isfile(path)
		#os.path.isdir(path)

		if os.path.exists('/usr/include/Wt'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/include' ])
		elif os.path.exists('/usr/local/include/Wt'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/local/include' ])

		if os.path.exists('/usr/include/c++'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/include/c++' ])

		if os.path.exists('/usr/include/boost'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/include' ])
		elif os.path.exists('/opt/boost_1_45_0'):
        		conf.env.AppendUnique(CPPPATH = [ '/opt/boost_1_45_0' ])


		#if os.path.exists(''):
        	#	conf.env.Append(CPPPATH = [ '' ])

			
        	conf.env.Prepend(CPPPATH = [ '/usr/local/cyassl/include' ])
       		conf.env.Prepend(CPPPATH = [ MYSQL_PATH+'/include' ])
        	conf.env.Prepend(CPPPATH = [ localc_dir+'/mb2i_wt' ])
        	conf.env.Prepend(LIBPATH = [ localc_dir+'/mb2l_wt' ])
	else:
        	conf.env.Prepend(CPPPATH = [ '/usr/local/cyassl/include' ])
       		conf.env.Prepend(CPPPATH = [ MYSQL_PATH+'/include' ])
        	conf.env.Prepend(CPPPATH = [ localc_dir+'/mb2i_qt' ])
        	conf.env.Prepend(LIBPATH = [ localc_dir+'/mb2l_qt' ])

        conf.env.Append(CPPDEFINES = [ 'linux' ])
        conf.env.Append(CPPDEFINES = [ 'LINUX2_2' ])
	if GetOption('enable_mysql_server') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYSQL' ])
	if GetOption('enable_myisam_engine') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYISAM' ])
        conf.env.Append(CPPDEFINES = [ 'SKIP_YWINDOWS' ])

	if os.environ.has_key('BUILDENV'):
		if os.environ.get('BUILDENV') == "ubuntu1204":
        		conf.env.Append(CPPDEFINES = [ 'UBUNTU_OS' ])
        		#conf.env.Append(CFLAGS = [ '-fno-stack-protector', '-D_FORTIFY_SOURCE=0' ])
			#conf.env.Append(LINKFLAGS = [ '-Wl,-z,norelro', '-z', 'execstack' ])

	print "Using standard paths for base library INCLUDES"
        conf.env.Append(CXXFLAGS = [ '-std=gnu++11' ])
	if GetOption('debug_build') == 1:
	       	conf.env.Append(CFLAGS = [ '-O0', '-ggdb3' ])
       		conf.env.Append(CXXFLAGS = [ '-O0', '-ggdb3' ])
	else:
	       	conf.env.Append(CFLAGS = [ '-O1' ])

       	conf.env.Append(CFLAGS = [ '-m64','-fpermissive', '-std=c99', '-c', '-pipe', '-Wunused', '-Wno-main', '-Wno-write-strings' ])
	conf.env.Append(CXXFLAGS = [ '-m64', '-Wno-write-strings' ,'-Wold-style-cast' , '-fpermissive' ])

       	conf.env.Append(CPPDEFINES = [ 'RDA_64BITS' ])


	if GetOption('GUI_TYPE') != 'WT':
        	conf.env.Append(CPPPATH = [ '/usr/local/rdaqt/include' ])
        	conf.env.Append(CPPPATH = [ '/usr/local/rdaqt/include/qtc' ])

        conf.env.Append(CPPDEFINES = [ 'SYSV' ])
        conf.env.Append(CPPDEFINES = [ '_POSIX_C_SOURCE=199309L' ])
        conf.env.Append(CPPDEFINES = [ '_POSIX_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ '_XOPEN_SOURCE=500L' ])
        conf.env.Append(CPPDEFINES = [ 'BSD_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ '_SVID_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ 'NeedFunctionPrototypes' ])
        conf.env.Append(CPPDEFINES = [ 'X_WCHAR' ])
        conf.env.Append(CPPDEFINES = [ 'FUNCPROTO=15' ])
        conf.env.Append(CPPDEFINES = [ 'NARROWPROTO' ])
        conf.env.Append(CPPDEFINES = [ 'MAP_TO_USE_RAID' ])
        conf.env.Append(CPPDEFINES = [ 'DBUG_OFF' ])

        #conf.env.Append(EXDIR = [ '/tmp/release/lnx_mysql' ])
        #conf.env.Append(LIBS = [ 'uuid' ])
        #base_env['PROGSUFFIX'] = [base_env['PROGSUFFIX'], '.lnx']
	#base_env['LINKCOM'] = [base_env['LINKCOM'], 'strip ${TARGET}', 'cp -v ${TARGET} ${EXDIR}' ]
	#base_env['LINKCOM'] = [base_env['LINKCOM'], 'strip ${TARGET}']

if platform.system() == 'Windows':
	print "rdalibs_env system() is Windows"
        conf.env.Append(CFLAGS = [ '/MD', '/w', '/EHsc', '/O1' ])
        conf.env.Append(CPPDEFINES = [ '__OSTYPE__="MSWINDOWS_NT"' ])
        conf.env.Append(CPPDEFINES = [ '_POSIX_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ 'POSIX' ])
        conf.env.Append(CPPDEFINES = [ 'NDEBUG' ])
        conf.env.Append(CPPDEFINES = [ '_WINDOWS' ])
        conf.env.Append(CPPDEFINES = [ 'SKIP_YWINDOWS' ])
	if GetOption('enable_mysql_server') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYSQL' ])

	if GetOption('enable_myisam_engine') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYISAM' ])

	conf.env.Append(CPPPATH = [ 'C:\\src\\mysql-5.0.37\\myisam' ])
	conf.env.Append(CPPPATH = [ 'C:\\src\\mysql-5.0.37\\include' ])
	conf.env.Append(CPPPATH = [ 'C:\\src\\mysql-5.0.37' ])
	conf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\cyassl-1.4.0\\include' ])



	if GetOption('GUI_TYPE') == 'WT':
		print "rdalibs_env system() is Windows and wt env"
        	conf.env.Append(CPPPATH = [ 'C:\\localc\\mb2i_wt' ])
        	conf.env.Append(LIBPATH = [ 'C:\\localc\\mb2l_wt' ])
	else:
		print "rdalibs_env system() is Windows but not wt env"
		conf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qtc-qt3\\clib' ])
		conf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qtc-qt3\\clib\\qtc' ])
        	conf.env.Append(CPPPATH = [ 'mb2i_qt' ])
        	conf.env.Append(LIBPATH = [ 'mb2l_qt' ])

	print "Current Include path is: "+str(rdalibs_env['CPPPATH'])
        conf.env.Append(LIBS = [ 'zlib' ])
        conf.env.Append(LIBS = [ 'libpng' ])
        conf.env.Append(LIBS = [ 'libjpeg' ])
        conf.env.Append(LIBS = [ 'msvcrt' ])
        conf.env.Append(LIBS = [ 'rpcrt4' ])
        conf.env.Append(LIBS = [ 'shell32' ])

conf.Finish()
print "\nCurrent RDA Libs Include path is: "+str(rdalibs_env['CPPPATH'])+'\n'
print "\nCurrent RDA Libs Library path is: "+str(rdalibs_env['LIBPATH'])+'\n'



#misc nrd gui trans olh sec mix rpt add mklib mkvir mkmisc mbplus chglib cver cscn mkgag tool 
#BEGIN_LIBS


rdalibs_misc_env = rdalibs_env.Clone()
conf = rdalibs_misc_env.Configure()
conf.Finish()

LIB_OPARTS=list(get_oparts_builddir('misc/misc.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('misc','misc/misc.cpp libmisc.a',LIB_OPARTS)
buildcmd_rdalibs_misc = rdalibs_misc_env.StaticLibrary('misc',LIB_OPARTS)
#buildcmd_rdalibs_misc = rdalibs_misc_env.SharedLibrary('misc',LIB_OPARTS)
Delete(LIB_OPARTS)


#if GetOption('delobj'):
#	print "skip cleaning misc..."
#	rdalibs_misc_env.Precious(buildcmd_rdalibs_misc)
#	rdalibs_misc_env.NoClean(buildcmd_rdalibs_misc)
#	NoClean(buildcmd_rdalibs_misc)
#	Precious(buildcmd_rdalibs_misc)





#misc nrd gui trans olh sec mix rpt add mklib mkvir mkmisc mbplus chglib cver cscn mkgag tool 

rdalibs_nrd_env = rdalibs_env.Clone()
conf = rdalibs_nrd_env.Configure()
conf.Finish()

LIB_OPARTS=list(get_oparts_builddir('nrd/nrd.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('nrd','nrd/nrd.cpp libnrd.a',LIB_OPARTS)

if os.access(localc_dir+'/nrd/nrd.cpp', os.R_OK):
	buildcmd_rdalibs_nrd = rdalibs_nrd_env.StaticLibrary('nrd',LIB_OPARTS)
	#buildcmd_rdalibs_nrd = rdalibs_nrd_env.SharedLibrary('nrd',LIB_OPARTS)
else:
	buildcmd_rdalibs_nrd = None
	print 'Could Not Find: [nrd/nrd.cpp].  Skipping NRD Library...'

Delete(LIB_OPARTS)




rdalibs_gui_env = rdalibs_env.Clone()
conf = rdalibs_gui_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('gui_wt/gui.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('gui','gui_wt/gui.cpp libgui.a',LIB_OPARTS)

if GetOption('GUI_TYPE') == 'WT':
	buildcmd_rdalibs_gui = rdalibs_gui_env.StaticLibrary('gui',LIB_OPARTS)
	#buildcmd_rdalibs_gui = rdalibs_gui_env.SharedLibrary('gui',LIB_OPARTS)

#elif GetOption('GUI_TYPE') == 'QT':
#	buildcmd_rdalibs_gui = rdalibs_gui_env.StaticLibrary('gui', [ get_oparts_builddir('gui_qt/gui.cpp') ] )

else:
	buildcmd_rdalibs_gui = None
	print 'Could Not Find: '+str(GetOption('GUI_TYPE'))+'  [gui.cpp'+'].  Skipping GUI Library...'

Delete(LIB_OPARTS)





rdalibs_trans_env = rdalibs_env.Clone()
conf = rdalibs_trans_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('trans/trans.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('trans','trans/trans.cpp libtrans.a',LIB_OPARTS)

buildcmd_rdalibs_trans = rdalibs_trans_env.StaticLibrary('trans',LIB_OPARTS)
#buildcmd_rdalibs_trans = rdalibs_trans_env.SharedLibrary('trans',LIB_OPARTS)
Delete(LIB_OPARTS)







rdalibs_olh_env = rdalibs_env.Clone()
conf = rdalibs_olh_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('olh/olh.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('olh','olh/olh.cpp libolh.a',LIB_OPARTS)

buildcmd_rdalibs_olh = rdalibs_olh_env.StaticLibrary('olh',LIB_OPARTS)
#buildcmd_rdalibs_olh = rdalibs_olh_env.SharedLibrary('olh',LIB_OPARTS)
Delete(LIB_OPARTS)








rdalibs_sec_env = rdalibs_env.Clone()
conf = rdalibs_sec_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('sec/sec.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('sec','sec/sec.cpp lib.a',LIB_OPARTS)

buildcmd_rdalibs_sec = rdalibs_sec_env.StaticLibrary('sec', [ get_oparts_builddir('sec/sec.cpp') ] )
#buildcmd_rdalibs_sec = rdalibs_sec_env.SharedLibrary('sec', [ get_oparts_builddir('sec/sec.cpp') ] )
Delete(LIB_OPARTS)






rdalibs_mix_env = rdalibs_env.Clone()
conf = rdalibs_mix_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('mix/mix.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('mix','mix/mix.cpp libmix.a',LIB_OPARTS)

buildcmd_rdalibs_mix = rdalibs_mix_env.StaticLibrary('mix',LIB_OPARTS)
#buildcmd_rdalibs_mix = rdalibs_mix_env.SharedLibrary('mix',LIB_OPARTS)
Delete(LIB_OPARTS)







rdalibs_rpt_env = rdalibs_env.Clone()
conf = rdalibs_rpt_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('rptgen/rpt.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('rpt','rptgen/rpt.cpp lib.a',LIB_OPARTS)

buildcmd_rdalibs_rpt = rdalibs_rpt_env.StaticLibrary('rpt',LIB_OPARTS)
#buildcmd_rdalibs_rpt = rdalibs_rpt_env.SharedLibrary('rpt',LIB_OPARTS)
Delete(LIB_OPARTS)






rdalibs_add_env = rdalibs_env.Clone()
conf = rdalibs_add_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('add/add.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('add','add/add.cpp libadd.a',LIB_OPARTS)

buildcmd_rdalibs_add = rdalibs_add_env.StaticLibrary('add',LIB_OPARTS)
#buildcmd_rdalibs_add = rdalibs_add_env.SharedLibrary('add',LIB_OPARTS)
Delete(LIB_OPARTS)





rdalibs_mklib_env = rdalibs_env.Clone()
conf = rdalibs_mklib_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/mklib.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('mklib','dumlib/mklib.cpp libmklib.a',LIB_OPARTS)
Delete(LIB_OPARTS)

buildcmd_rdalibs_mklib = rdalibs_mklib_env.StaticLibrary('mklib',LIB_OPARTS)
#buildcmd_rdalibs_mklib = rdalibs_mklib_env.SharedLibrary('mklib',LIB_OPARTS)







rdalibs_mkvir_env = rdalibs_env.Clone()
conf = rdalibs_mkvir_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/mkvir.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('mkvir','dumlib/mkvir.cpp libmkvir.a',LIB_OPARTS)

buildcmd_rdalibs_mkvir = rdalibs_mkvir_env.StaticLibrary('mkvir',LIB_OPARTS)
#buildcmd_rdalibs_mkvir = rdalibs_mkvir_env.SharedLibrary('mkvir',LIB_OPARTS)
Delete(LIB_OPARTS)






rdalibs_mkmisc_env = rdalibs_env.Clone()
conf = rdalibs_mkmisc_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/mkmisc.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('mkmisc','dumlib/mkmisc.cpp libmkmisc.a',LIB_OPARTS)

buildcmd_rdalibs_mkmisc = rdalibs_mkmisc_env.StaticLibrary('mkmisc',LIB_OPARTS)
#buildcmd_rdalibs_mkmisc = rdalibs_mkmisc_env.SharedLibrary('mkmisc',LIB_OPARTS)
Delete(LIB_OPARTS)





rdalibs_mbplus_env = rdalibs_env.Clone()
conf = rdalibs_mbplus_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/mbplus.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('mbplus','dumlib/mbplus.cpp libmbplus.a',LIB_OPARTS)

buildcmd_rdalibs_mbplus = rdalibs_mbplus_env.StaticLibrary('mbplus',LIB_OPARTS)
#buildcmd_rdalibs_mbplus = rdalibs_mbplus_env.SharedLibrary('mbplus',LIB_OPARTS)
Delete(LIB_OPARTS)






rdalibs_chglib_env = rdalibs_env.Clone()
conf = rdalibs_chglib_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/chglib.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('chglib','dumlib/chglib.cpp libchglib.a',LIB_OPARTS)

buildcmd_rdalibs_chglib = rdalibs_chglib_env.StaticLibrary('chglib',LIB_OPARTS)
#buildcmd_rdalibs_chglib = rdalibs_chglib_env.SharedLibrary('chglib',LIB_OPARTS)
Delete(LIB_OPARTS)







rdalibs_cver_env = rdalibs_env.Clone()
conf = rdalibs_cver_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/cver.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('cver','dumlib/cver.cpp libcver.a',LIB_OPARTS)

buildcmd_rdalibs_cver = rdalibs_cver_env.StaticLibrary('cver',LIB_OPARTS)
#buildcmd_rdalibs_cver = rdalibs_cver_env.SharedLibrary('cver',LIB_OPARTS)
Delete(LIB_OPARTS)






rdalibs_cscn_env = rdalibs_env.Clone()
conf = rdalibs_cscn_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/cscn.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('cscn','dumlib/cscn.cpp libcscn.a',LIB_OPARTS)

buildcmd_rdalibs_cscn = rdalibs_cscn_env.StaticLibrary('cscn',LIB_OPARTS)
#buildcmd_rdalibs_cscn = rdalibs_cscn_env.SharedLibrary('cscn',LIB_OPARTS)
Delete(LIB_OPARTS)







rdalibs_mkgag_env = rdalibs_env.Clone()
conf = rdalibs_mkgag_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('dumlib/mkgag.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('mkgag','dumlib/mkgag.cpp libmkgag.a',LIB_OPARTS)

buildcmd_rdalibs_mkgag = rdalibs_mkgag_env.StaticLibrary('mkgag',LIB_OPARTS)
#buildcmd_rdalibs_mkgag = rdalibs_mkgag_env.SharedLibrary('mkgag',LIB_OPARTS)
Delete(LIB_OPARTS)






rdalibs_tool_env = rdalibs_env.Clone()
conf = rdalibs_tool_env.Configure()
conf.Finish()
LIB_OPARTS=list(get_oparts_builddir('tools/tool.cpp'))
if GetOption('delobj') == 1:
	purge_target_oparts('tool','tools/tool.cpp libtool.a',LIB_OPARTS)

buildcmd_rdalibs_tool = rdalibs_tool_env.StaticLibrary('tool',LIB_OPARTS)
#buildcmd_rdalibs_tool = rdalibs_tool_env.SharedLibrary('tool',LIB_OPARTS)
Delete(LIB_OPARTS)


#END_LIBS


#g++ -O3 -static -I/opt/boost-1_55/include -L/opt/boost-1_55/lib  -o test.lnx test.cpp -lboost_date_time -lboost_log -lboost_system  -lboost_thread -lpodofo -lpthread -lfontconfig -lfreetype -lexpat -lcrypto++ -lcrypto -lidn  -ltiff -ljpeg -lz -luuid -lrt -ldl

viewpdfpodofoenv = base_env.Clone()
viewpdfpodofoconf = viewpdfpodofoenv.Configure()

#viewpdfpodofoconf.env.Append(CPPDEFINES = [ '__VER__=\\"'+datetime.datetime.now().strftime("%m/%d/%Y")+'\\"' ])

if GetOption('use_rdadiag') == 1:
       	viewpdfpodofoconf.env.Append(CPPDEFINES = [ 'USE_RDA_DIAGNOSTICS' ])

if GetOption('debug_build') == 1:
 	viewpdfpodofoconf.env.Append(CFLAGS = [ '-ggdb3' ])
        viewpdfpodofoconf.env.Append(CXXFLAGS = [ '-ggdb3' ])
	


if platform.system() == 'Linux':
 	viewpdfpodofoconf.env.Append(CFLAGS = [ '-O3', '-static' ])
        viewpdfpodofoconf.env.Append(CXXFLAGS = [ '-O3', '-static' ])
        viewpdfpodofoconf.env.Append(LINKFLAGS = [ '-static' ])

        viewpdfpodofoconf.env.Prepend(CPPPATH = [ '/opt/boost-1_55/include' ])
        viewpdfpodofoconf.env.Prepend(LIBPATH = [ '/opt/boost-1_55/lib' ])

        viewpdfpodofoconf.env.Append(LIBS = [ 'boost_date_time' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'boost_log' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'boost_system' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'boost_thread' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'podofo' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'pthread' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'fontconfig' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'freetype' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'expat' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'crypto++' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'crypto' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'idn' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'tiff' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'jpeg' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'png' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'z' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'uuid' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'rt' ])
        viewpdfpodofoconf.env.Append(LIBS = [ 'dl' ])

        viewpdfpodofoenv['PROGSUFFIX'] = [viewpdfpodofoenv['PROGSUFFIX'], '.lnx']

# This needs to not strip when debug is true
	print GetOption('use_strip')
	print GetOption('debug_build')
	if GetOption('use_strip') and GetOption('debug_build') == 0:
		viewpdfpodofoenv.Append(LINKCOM = [ 'strip ${TARGET}' ])
#viewpdfpodofoenv.AddPostAction(buildcmd_viewpdfpodofo, 'strip -v $TARGET')

	if GetOption('use_qadir'):
		print "using qadir section..."
		if os.environ.has_key('BUILDENV'):
			if GetOption('GUI_TYPE') == 'WT':
				if not os.path.isdir('/rdadev/QA/' + os.environ.get('BUILDENV') + '-wt'):
				        os.makedirs('/rdadev/QA/' + os.environ.get('BUILDENV') + '-wt')
				viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -fv ${TARGET} /rdadev/QA/' + os.environ.get('BUILDENV') + '-wt/'  ])
			else:
				if not os.path.isdir('/rdadev/QA/' + os.environ.get('BUILDENV')):
				        os.makedirs('/rdadev/QA/' + os.environ.get('BUILDENV'))
				viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -fv ${TARGET} /rdadev/QA/' + os.environ.get('BUILDENV') + '/'  ])
	elif os.environ.has_key('EXDIR'):
		print "using exdir section..."
		#if GetOption('use_exdir'):
		if os.environ.get('BUILDENV') == "ubuntu1204":
			if GetOption('GUI_TYPE') != 'WT':
				viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '/' ])
			else:
				viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '-wt/' ])
		else:
			viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '/' ])
			
	else:
		print "using else section..."
		if GetOption('use_fastcgi') == 1:
			print 'Using FASTCGI DIR'
			viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms-fastcgi/' ])
		else:
			print 'Using WTHTTP DIR'
			if os.environ.get('BUILDENV') == "ubuntu1204":
				viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms-wt/' ])
			else:
				viewpdfpodofoconf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms/' ])


viewpdfpodofoconf.Finish()
buildcmd_viewpdfpodofo = viewpdfpodofoenv.Program(target='viewpdf-podofo', source=['viewpdf-podofo/viewpdf-podofo.cpp'])  
buildcmd_vtest = viewpdfpodofoenv.Program(target='vtest', source=[localc_dir+'/utl/vtest.cpp'])  




#env = Environment(ENV = {'INCLUDE' : os.environ['INCLUDE']})
viewpdfenv = base_env.Clone()
viewpdfconf = viewpdfenv.Configure()

if GetOption('use_rdadiag') == 1:
       	viewpdfconf.env.Append(CPPDEFINES = [ 'USE_RDA_DIAGNOSTICS' ])

if GetOption('debug_build') == 1:
       	viewpdfconf.env.Append(CFLAGS = [ '-ggdb3' ])


if platform.system() == 'Linux':
        viewpdfconf.env.Append(CPPDEFINES = 'POSIX')
	
	if os.path.exists('/usr/include/ghostscript'):
		viewpdfconf.env.Append(CPPPATH = [ '/usr/include/include/efi' ])
		viewpdfconf.env.Append(CPPPATH = [ '/usr/include/ghostscript' ])
	#	viewpdfconf.env.Append(LIBPATH = [ '/opt/gpl-gs-8.70/lib' ])
	else:
		viewpdfconf.env.Append(CPPPATH = [ '/opt/gpl-gs-8.70/include/efi' ])
		viewpdfconf.env.Append(CPPPATH = [ '/opt/gpl-gs-8.70/include/ghostscript' ])
		viewpdfconf.env.Append(LIBPATH = [ '/opt/gpl-gs-8.70/lib' ])

        viewpdfconf.env.Append(EXDIR = [ '/tmp/release/lnx_mysql' ])
        viewpdfconf.env.Append(LIBS = [ 'uuid' ])
        viewpdfconf.env.Append(LIBS = [ 'ps-static' ])
        viewpdfconf.env.Append(LIBS = [ 'ungif' ])
        viewpdfconf.env.Append(LIBS = [ 'tiff-static' ])
	if os.path.exists('/usr/lib/libgs.a'):
        	viewpdfconf.env.Append(LIBS = [ 'gs' ])
	else:
        	viewpdfconf.env.Append(LIBS = [ 'gpl-gs' ])
        viewpdfenv['PROGSUFFIX'] = [viewpdfenv['PROGSUFFIX'], '.lnx']

	# This needs to not strip when debug is true

	if GetOption('use_qadir'):
		print "using qadir section..."
		if os.environ.has_key('BUILDENV'):
			if GetOption('GUI_TYPE') == 'WT':
				if not os.path.isdir('/rdadev/QA/' + os.environ.get('BUILDENV') + '-wt'):
				        os.makedirs('/rdadev/QA/' + os.environ.get('BUILDENV') + '-wt')
				viewpdfenv.Append(LINKCOM = [ 'mv -fv ${TARGET} /rdadev/QA/' + os.environ.get('BUILDENV') + '-wt/'  ])
			else:
				if not os.path.isdir('/rdadev/QA/' + os.environ.get('BUILDENV')):
				        os.makedirs('/rdadev/QA/' + os.environ.get('BUILDENV'))
				viewpdfenv.Append(LINKCOM = [ 'mv -fv ${TARGET} /rdadev/QA/' + os.environ.get('BUILDENV') + '/'  ])
	elif os.environ.has_key('EXDIR'):
		print "using exdir section..."
		#if GetOption('use_exdir'):
		if os.environ.get('BUILDENV') == "ubuntu1204":
			if GetOption('GUI_TYPE') != 'WT':
				viewpdfenv.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '/' ])
			else:
				viewpdfenv.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '-wt/' ])
		else:
			viewpdfenv.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '/' ])
			
	else:
		print "using else section..."
		if GetOption('use_fastcgi') == 1:
			print 'Using FASTCGI DIR'
			viewpdfenv.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms-fastcgi/' ])
		else:
			print 'Using WTHTTP DIR'
			if os.environ.get('BUILDENV') == "ubuntu1204":
				viewpdfenv.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms-wt/' ])
			else:
				viewpdfenv.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms/' ])
			

	#viewpdfenv['LINKCOM'] = [viewpdfenv['LINKCOM'], 'strip ${TARGET}', 'cp -v ${TARGET} '+localc_dir+'/testqt/' ]

	#viewpdfenv['LINKCOM'] = [viewpdfenv['LINKCOM'], 'strip ${TARGET}', 'cp -v ${TARGET} ${EXDIR}' ]
	#viewpdfenv['LINKCOM'] = [viewpdfenv['LINKCOM'], 'strip ${TARGET}']


if platform.system() == 'Windows':
#	viewpdfenv['LINKCOM'] = [viewpdfenv['LINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1']
#        viewpdfenv['SHLINKCOM'] = [viewpdfenv['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']
        viewpdfconf.env.Append(CPPDEFINES = [ 'WIN32' ])
        viewpdfconf.env.Append(CPPDEFINES = [ '_WIN32' ])
        viewpdfconf.env.Append(CPPDEFINES = [ 'MSWIN32' ])
        viewpdfconf.env.Append(CPPDEFINES = [ '_CRT_SECURE_NO_DEPRECATE' ])
#        viewpdfconf.env.Append(CPPDEFINES = [ 'pslib_EXPORTS' ])
#        viewpdfconf.env.Append(CPPDEFINES = [ 'PSLIB_EXPORTS' ])

#        viewpdfconf.env.Append(LINKFLAGS = [ '/MANIFEST' ])
#        viewpdfconf.env.Append(LINKFLAGS = [ '/STACK:10000000' ])
#        viewpdfconf.env.Append(LINKFLAGS = [ '/machine:I386' ])
        #viewpdfconf.env.Append(LINKFLAGS = [ '/subsystem:windows' ])
        #viewpdfconf.env.Append(LINKFLAGS = [ '/subsystem:console' ])
        viewpdfconf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:libcmt.lib' ])
        viewpdfconf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:libcmtd.lib' ])
        viewpdfconf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:msvcrtd.lib' ])

        viewpdfconf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\pslib-0.4.1\\include' ])
        viewpdfconf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\ghostscript-8.70\\psi' ])
        viewpdfconf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\zlib' ])
        viewpdfconf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libjpeg' ])
        viewpdfconf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libpng' ])

        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\zlib' ])
        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libjpeg' ])
        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libpng' ])
        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\zlib' ])
        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\tiff-3.8.2\libtiff' ])
        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\pslib-0.4.1' ])
        viewpdfconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\ghostscript-8.70\\bin' ])
        viewpdfconf.env.Append(LIBS = [ 'zlib' ])
        viewpdfconf.env.Append(LIBS = [ 'libpng' ])
        viewpdfconf.env.Append(LIBS = [ 'libjpeg' ])
        viewpdfconf.env.Append(LIBS = [ 'libtiff' ])
        #viewpdfconf.env.Append(LIBS = [ 'pslib' ])
        viewpdfconf.env.Append(LIBS = [ 'pslib-static' ])
        viewpdfconf.env.Append(LIBS = [ 'msvcrt' ])
        viewpdfconf.env.Append(LIBS = [ 'gsdll32' ])
        viewpdfconf.env.Append(LIBS = [ 'rpcrt4' ])
        viewpdfconf.env.Append(LIBS = [ 'shell32' ])

# None indented adds to all platform's Environment.
viewpdfconf.env.Append(CPPDEFINES = [ '__VER__=\\"'+datetime.datetime.now().strftime("%m/%d/%Y")+'\\"' ])
viewpdfconf.env.Append(CPPDEFINES = [ 'HAVE_LIBPNG' ])
viewpdfconf.env.Append(CPPDEFINES = [ 'HAVE_LIBJPEG' ])
#viewpdfconf.env.Append(CPPDEFINES = [ 'HAVE_LIBGIF' ])
#viewpdfconf.env.Append(CPPDEFINES = [ 'HAVE_LIBTIFF' ])

viewpdfconf.Finish()
buildcmd_viewpdf = viewpdfenv.Program(target='viewpdf', source=[localc_dir+'/utl/viewpdf.cpp'])  
buildcmd_testpdf = viewpdfenv.Program(target='testpdf', source=[localc_dir+'/utl/testpdf.cpp'])  

#viewpdfenv.Program(target='viewpdf', source=[localc_dir+'utl/viewpdf.cpp'])







vmimesendenv = base_env.Clone()
vmimesendconf = vmimesendenv.Configure()


if GetOption('use_rdadiag') == 1:
       	vmimesendconf.env.Append(CPPDEFINES = [ 'USE_RDA_DIAGNOSTICS' ])

if platform.system() == 'Linux':
        vmimesendconf.env.Append(LIBS = [ 'vmime' ])


if platform.system() == 'Windows':
	#vmimesendconf.env.Append(LINKCOM = [ 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1'])
	#vmimesendconf.env.Append(SHLINKCOM = [ 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2'])

        vmimesendconf.env.Append(CPPDEFINES = [ 'WIN32' ])
        vmimesendconf.env.Append(CPPDEFINES = [ 'WIN32' ])
        vmimesendconf.env.Append(CPPDEFINES = [ '_WIN32' ])
        vmimesendconf.env.Append(CPPDEFINES = [ 'MSWIN32' ])
        vmimesendconf.env.Append(CPPDEFINES = [ '_CRT_SECURE_NO_DEPRECATE' ])
	vmimesendconf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:libcmt.lib' ])
        vmimesendconf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:libcmtd.lib' ])
        vmimesendconf.env.Append(LINKFLAGS = [ '/NODEFAULTLIB:msvcrtd.lib' ])
        vmimesendconf.env.Append(CPPFLAGS = [ '/EHsc' ])

        vmimesendconf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\vmime-svn' ])
	vmimesendconf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\vmime-svn' ])

        vmimesendconf.env.Append(LIBS = [ 'vmime' ])
        vmimesendconf.env.Append(LIBS = [ 'msvcrt' ])
        vmimesendconf.env.Append(LIBS = [ 'rpcrt4' ])
        vmimesendconf.env.Append(LIBS = [ 'shell32' ])

#if conf.CheckCHeader('vmime.h'):
#if not vmimesendconf.CheckLib('vmime'):
#	print "Can Not Find VMIME Library.  Can Not Build vmime-sendmail."
vmimesendconf.Finish()
#vmimesendenv.Program(target='vmime-sendmail', source=[localc_dir+'/utl/vmime-sendmail.cpp', localc_dir+'/utl/vmime-sendmail2.c' ])
buildcmd_vmimesendmail = vmimesendenv.Program(target='vmime-sendmail', source=[localc_dir+'/utl/vmime-sendmail.cpp', localc_dir+'/utl/vmime-sendmail2.c' ]) 





Alias('misc', buildcmd_rdalibs_misc )
Alias('nrd', buildcmd_rdalibs_nrd )
Alias('gui', buildcmd_rdalibs_gui )
#if GetOption('GUI_TYPE') == 'WT':
#	Alias('guibridge', buildcmd_rdalibs_guibridge )
Alias('trans', buildcmd_rdalibs_trans )
Alias('olh', buildcmd_rdalibs_olh )
Alias('sec', buildcmd_rdalibs_sec )
Alias('mix', buildcmd_rdalibs_mix )
Alias('rpt', buildcmd_rdalibs_rpt )
Alias('add', buildcmd_rdalibs_add )
Alias('mklib', buildcmd_rdalibs_mklib )
Alias('mkvir', buildcmd_rdalibs_mkvir )
Alias('mkmisc', buildcmd_rdalibs_mkmisc )
Alias('mbplus', buildcmd_rdalibs_mbplus )
Alias('chglib', buildcmd_rdalibs_chglib )
Alias('cver', buildcmd_rdalibs_cver )
Alias('cscn', buildcmd_rdalibs_cscn )
Alias('mkgag', buildcmd_rdalibs_mkgag )
Alias('tool', buildcmd_rdalibs_tool )


#Alias('viewpdf', viewpdfenv.Program(target='viewpdf', source=[localc_dir+'/utl/viewpdf.c'])  )
Alias('viewpdf-podofo', buildcmd_viewpdfpodofo )
Alias('vtest', buildcmd_vtest )
Alias('viewpdf', buildcmd_viewpdf )
Alias('testpdf', buildcmd_testpdf )
Alias('vmime-sendmail', buildcmd_vmimesendmail )

Alias('3rdparty', [ 
	'viewpdf', buildcmd_viewpdf,
	'vmime-sendmail', buildcmd_vmimesendmail,
	])

#Default('no_target_specfied')
Default(None)
Alias('makelibs', [ 
	'misc', buildcmd_rdalibs_misc,
	'nrd', buildcmd_rdalibs_nrd,
	'gui', buildcmd_rdalibs_gui,
	'trans', buildcmd_rdalibs_trans,
	'olh', buildcmd_rdalibs_olh,
	'sec', buildcmd_rdalibs_sec,
	'mix', buildcmd_rdalibs_mix,
	'rpt', buildcmd_rdalibs_rpt,
	'add', buildcmd_rdalibs_add,
	'mklib', buildcmd_rdalibs_mklib,
	'mkvir', buildcmd_rdalibs_mkvir,
	'mkmisc', buildcmd_rdalibs_mkmisc,
	'mbplus', buildcmd_rdalibs_mbplus,
	'chglib', buildcmd_rdalibs_chglib,
	'cver', buildcmd_rdalibs_cver,
	'cscn', buildcmd_rdalibs_cscn,
	'mkgag', buildcmd_rdalibs_mkgag,
	'tool', buildcmd_rdalibs_tool,
	] )

#Alias('makelibs', [ 'misc', 'nrd', 'gui', 'trans', 'olh', 'sec', 'mix', 'rpt', 'add', 'mklib', mkvir', 'mkmisc', 'mbplus', 'chglib', 'cver', 'cscn', 'mkgag', 'tool' ] )
#rda_apps_env = rdalibs_env.Clone()



rda_apps_env = base_env.Clone()
conf = rda_apps_env.Configure()
conf.env.Append(CPPDEFINES = [ '__VER__=\\"'+datetime.datetime.now().strftime("%m/%d/%Y")+'\\"' ])
#conf.env.Append(CPPDEFINES = [ '__NAM__=\\"domenu.lnx\\"' ])

if platform.system() == 'Linux':
	print "Using standard paths for DEFINES"
	if GetOption('debug_build') == 1:
        	conf.env.Append(CFLAGS = [ '-O0', '-ggdb3' ])
        	conf.env.Append(CXXFLAGS = [ '-O0', '-ggdb3' ])
	else:
       		conf.env.Append(CFLAGS = [ '-O1' ])

	conf.env.Append(CFLAGS = [ '-m64', '-std=c99', '-c', '-pipe', '-Wunused', '-Wno-main', '-Wno-write-strings' ])

	# Added '-fpermissive' to bypass gcc error/warning about passing pointers of unknown/incompatible types 
       	conf.env.Append(CFLAGS = [ '-fpermissive' ])
        conf.env.Append(CXXFLAGS = [ '-std=gnu++11' ])
       	conf.env.Append(CXXFLAGS = [ '-m64', '-Wno-write-strings' , '-Wold-style-cast' , '-fpermissive' ])
       	conf.env.Append(CPPDEFINES = [ 'RDA_64BITS' ])

	if GetOption('use_rdadiag') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_RDA_DIAGNOSTICS' ])


        conf.env.Append(CPPDEFINES = [ 'linux' ])
        conf.env.Append(CPPDEFINES = [ 'LINUX2_2' ])
	if GetOption('enable_mysql_server') == 1:
       		conf.env.Append(CPPDEFINES = [ 'USE_MYSQL' ])
	if GetOption('enable_myisam_engine') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYISAM' ])
        conf.env.Append(CPPDEFINES = [ 'SKIP_YWINDOWS' ])
        conf.env.Append(CPPDEFINES = [ 'SYSV' ])
        conf.env.Append(CPPDEFINES = [ '_POSIX_C_SOURCE=199309L' ])
        conf.env.Append(CPPDEFINES = [ '_POSIX_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ '_XOPEN_SOURCE=500L' ])
        conf.env.Append(CPPDEFINES = [ 'BSD_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ '_SVID_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ 'NeedFunctionPrototypes' ])
        conf.env.Append(CPPDEFINES = [ 'X_WCHAR' ])
        conf.env.Append(CPPDEFINES = [ 'FUNCPROTO=15' ])
        conf.env.Append(CPPDEFINES = [ 'NARROWPROTO' ])
        conf.env.Append(CPPDEFINES = [ 'MAP_TO_USE_RAID' ])
        conf.env.Append(CPPDEFINES = [ 'DBUG_OFF' ])

	#conf.env.Append(CPPDEFINES = [ '__NAM__=\\"lnx\\"' ])

	if os.environ.has_key('BUILDENV'):
		if os.environ.get('BUILDENV') == "ubuntu1204":
        		conf.env.Append(CPPDEFINES = [ 'UBUNTU_OS' ])
        		#conf.env.Append(CFLAGS = [ '-fno-stack-protector', '-D_FORTIFY_SOURCE=0' ])
			#conf.env.Append(LINKFLAGS = [ '-Wl,-z,norelro', '-z', 'execstack' ])

	#if LOCAL_CPP:
	#	conf.env.AppendUnique(CPPPATH = [ '/opt/mysql-connector-c++/include' ])
	#	conf.env.Append(LIBPATH = [ '/opt/mysql-connector-c++/lib' ])
	

	if GetOption('GUI_TYPE') == 'WT':
        	conf.env.Append(CPPDEFINES = [ 'CPPMAIN' ])

        	conf.env.Prepend(CPPPATH = [ '/usr/local/cyassl/include' ])
       		conf.env.Prepend(CPPPATH = [ MYSQL_PATH+'/include' ])
        	conf.env.Prepend(CPPPATH = [ localc_dir+'/mb2i_wt' ])

		conf.env.Prepend(LIBPATH = [ '/usr/local/cyassl/lib' ])
		conf.env.Prepend(LIBPATH = [ MYSQL_PATH+'/lib' ])
        	#conf.env.Prepend(LIBPATH = [ localc_dir+'/mb2l_wt' ])

		if GetOption('use_fastcgi') == 1:
        		conf.env.Append(CPPDEFINES = [ 'WT_FASTCGI' ])

		if os.path.exists('/usr/include/Wt'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/include' ])
		elif os.path.exists('/usr/local/include/Wt'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/local/include' ])

		if os.path.exists('/usr/include/c++'):
        		conf.env.Append(CPPPATH = [ '/usr/include/c++' ])

		if os.path.exists('/usr/include/boost'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/include' ])
		elif os.path.exists('/opt/boost_1_45_0'):
        		conf.env.Append(CPPPATH = [ '/opt/boost_1_45_0' ])


        	#conf.env.Append(CPPPATH = [ localc_dir+'/rda_wt' ])
        	#conf.env.Append(CPPPATH = [ '/usr/X11R6/include' ])

		if os.path.exists('/usr/lib/libwt.a'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/lib' ])
		elif os.path.exists('/usr/local/lib/libwt.a'):
        		conf.env.AppendUnique(CPPPATH = [ '/usr/local/lib' ])

		if os.path.exists('/usr/lib/libboost_system.a'):
        		conf.env.AppendUnique(LIBPATH = [ '/usr/lib' ])
		elif os.path.exists('/opt/boost_1_45_0/stage/lib'):
        		conf.env.Append(LIBPATH = [ '/opt/boost_1_45_0/stage/lib' ])


	else:
		print "Using standard paths for app env INCLUDES"
	       	conf.env.Append(CPPPATH = [ localc_dir+'/mb2i_qt' ])
	       	conf.env.Prepend(CPPPATH = [ MYSQL_PATH+'/include' ])
	       	conf.env.Append(CPPPATH = [ '/usr/local/cyassl/include' ])
	       	conf.env.Append(CPPPATH = [ '/usr/X11R6/include' ])
	       	conf.env.Append(CPPPATH = [ '/usr/local/rdaqt/include' ])
	       	conf.env.Append(CPPPATH = [ '/usr/local/rdaqt/include/qtc' ])
		conf.env.Append(LIBPATH = [ '/usr/local/rdaqt/lib' ])


	print "Using standard paths for app env LIBS"

        #conf.env.Append(LIBPATH = [ localc_dir+'/mb2l_qt' ])

	#conf.env.Append(LIBPATH = [ '/rdadev/build/etch64-new' ])
	#conf.env.Append(LIBPATH = [ '/usr/local/cyassl/lib' ])
	#conf.env.Append(LIBS = get_addlibs(localc_dir+"/utl/", "domenu", ".c") )


	conf.env.Prepend(LIBPATH = [ build_dir ])
        rda_apps_env['PROGSUFFIX'] = [rda_apps_env['PROGSUFFIX'], '.lnx']

	
	if GetOption('use_strip') and GetOption('debug_build') == 0:
		conf.env.Append(LINKCOM = [ 'strip ${TARGET}' ])


	if GetOption('use_qadir'):
		print 'Using QADIR'
		if os.environ.has_key('BUILDENV'):
			if GetOption('GUI_TYPE') == 'WT':
				if not os.path.isdir('/rdadev/QA/' + os.environ.get('BUILDENV') + '-wt'):
				        os.makedirs('/rdadev/QA/' + os.environ.get('BUILDENV') + '-wt')
				conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} /rdadev/QA/' + os.environ.get('BUILDENV') + '-wt/'  ])
			else:
				if not os.path.isdir('/rdadev/QA/' + os.environ.get('BUILDENV')):
				        os.makedirs('/rdadev/QA/' + os.environ.get('BUILDENV'))
				conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} /rdadev/QA/' + os.environ.get('BUILDENV') + '/'  ])
	elif os.environ.has_key('EXDIR'):
		print 'Using EXDIR'
		#if GetOption('use_exdir'):
		if os.environ.get('BUILDENV') == "ubuntu1204":
			if GetOption('GUI_TYPE') != 'WT':
				conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '/' ])
			else:
				conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '-wt/' ])
		else:
			conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  os.environ.get('EXDIR') + '/' ])
	else:
		if GetOption('use_fastcgi') == 1:
			print 'Using FASTCGI DIR'
			conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms-fastcgi/' ])
		else:
			print 'Using WTHTTP DIR'
			if os.environ.get('BUILDENV') == "ubuntu1204":
				conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms-wt/' ])
			else:
				conf.env.Append(LINKCOM = [ 'mv -v ${TARGET} ' +  '/rda/xpgms/' ])
			


	#base_env['LINKCOM'] = [base_env['LINKCOM'], 'strip ${TARGET}', 'cp -v ${TARGET} ${EXDIR}' ]


if platform.system() == 'Windows':
	print "rdalibs_env system() is Windows"
        conf.env.Append(CFLAGS = [ '/MD', '/w', '/EHsc', '/O1' ])
        conf.env.Append(CPPDEFINES = [ '__OSTYPE__="MSWINDOWS_NT"' ])
        conf.env.Append(CPPDEFINES = [ '_POSIX_SOURCE' ])
        conf.env.Append(CPPDEFINES = [ 'POSIX' ])
        conf.env.Append(CPPDEFINES = [ 'NDEBUG' ])
        conf.env.Append(CPPDEFINES = [ '_WINDOWS' ])
        conf.env.Append(CPPDEFINES = [ 'SKIP_YWINDOWS' ])
	if GetOption('enable_mysql_server') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYSQL' ])

	if GetOption('enable_myisam_engine') == 1:
        	conf.env.Append(CPPDEFINES = [ 'USE_MYISAM' ])
	#conf.env.Append(CPPDEFINES = [ '__NAM__=\\"exe\\"' ])

	if GetOption('use_win32_windows_subsystem'):
		conf.env.Append(LINKFLAGS = [ '/SUBSYSTEM:WINDOWS' ])
	else:
		conf.env.Append(LINKFLAGS = [ '/SUBSYSTEM:CONSOLE' ])

	conf.env.Append(CPPPATH = [ 'C:\\src\\mysql-5.0.37\\myisam' ])
	conf.env.Append(CPPPATH = [ 'C:\\src\\mysql-5.0.37\\include' ])
	conf.env.Append(CPPPATH = [ 'C:\\src\\mysql-5.0.37' ])
	conf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\cyassl-1.4.0\\include' ])



	if GetOption('GUI_TYPE') == 'WT':
		print "rdalibs_env system() is Windows and wt env"
        	conf.env.Append(CPPPATH = [ 'C:\\localc\\mb2i_wt' ])
        	conf.env.Append(LIBPATH = [ 'C:\\localc\\mb2l_wt' ])
	else:
		print "rdalibs_env system() is Windows but not wt env"
		conf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qtc-qt3\\clib' ])
		conf.env.Append(CPPPATH = [ 'C:\\qt3-dev\\qtc-qt3\\clib\\qtc' ])
        	conf.env.Append(CPPPATH = [ 'mb2i_qt' ])

		conf.env.Append(LIBPATH = [ build_dir ])
        	conf.env.Append(LIBPATH = [ 'mb2l_qt' ])
        	conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\zlib' ])
        	conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libjpeg' ])
        	conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libpng' ])
        	conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\src\\3rdparty\\libmng' ])
        	conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qt-3.3.x-p8\\lib' ])
		conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\qtc-qt3\\clib\\qtc' ])

		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\client\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\myisam\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\strings\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\mysys\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\dbug\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\zlib\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\extra\\yassl\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\src\\mysql-5.0.37\\extra\\yassl\\taocrypt\\release' ])
		conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\cyassl-1.4.0\\Release' ])
		conf.env.Append(LIBPATH = [ 'C:\\qt3-dev\\cyassl-1.4.0\\ctaocrypt\\Release' ])

	print "Current Include path is: "+str(rda_apps_env['CPPPATH'])
	print "Current Lib path is: "+str(rda_apps_env['LIBPATH'])
        conf.env.Append(LIBS = [ 'zlib' ])
        conf.env.Append(LIBS = [ 'libpng' ])
        conf.env.Append(LIBS = [ 'libjpeg' ])


#C:\qt3-dev\qt-3.3.x-p8\src\3rdparty\zlib



conf.Finish()
print "Current RDA Apps Include path is: "+str(rda_apps_env['CPPPATH'])+'\n'
print "Current RDA Apps Library path is: "+str(rda_apps_env['LIBPATH'])+'\n'
#rda_apps_env.PrintENV()
#rda_apps_env.get_addlibs(localc_dir+"/utl/", "domenu", ".c") 
#buildcmd_rda_apps_domenu = rda_apps_env.buildpgms("utl/", "domenu", ".c") 
#buildcmd_rda_apps_domenu = rda_apps_env.Program(target='domenu', source=[ 'utl/domenu.c', get_oparts_builddir(localc_dir+"/utl/", "domenu", ".c", "utl/") ])
#rda_apps_env.PrintENV()

#buildcmd_rda_apps_domenu = rda_apps_env.Program(target='domenu', source=[ 'utl/domenu.c', get_oparts_builddir(localc_dir+"/utl/", "domenu", ".c", "") ])
#Alias('domenu', buildcmd_rda_apps_domenu )
#rda_apps_env.Program(target='domenu', source=[ 'utl/domenu.c' ])





# This command should parse the filename and properly set variables
# then add the .Program result to an individual env and to the list 
# passed in the second argument.  This will build all needed aliases too.
#rda_apps_env.buildpgms3("utl/mtnmst.c", UTILITIES_LIST)) 

#print_env_vars( [ rdalibs_misc_env.Dictionary() ] )
#rdalibs_misc_env.PrintENV()




#this is regulat Java build for scons
#import os
#Import ("env")
#denv=env.Copy()
#java_env = base_env.Clone()
#classes=java_env.JavaDir(target='classes',source=['com'])
#set correct path for jar
#java_env['JARCHDIR']=os.path.join(java_denv.Dir('.').get_abspath(),'classes')
#java_env.Jar('HelloApplet',classes)


# From VMIME build script, do a take off of this to process file lists and aliases.

#409	for i in range(len(libvmime_all_sources)):
#410	        f = libvmime_all_sources[i]
#411	        if f[-4:] == '.hpp':
#412	                libvmime_all_sources[i] = 'vmime/' + f
#413	        else:
#414	                libvmime_all_sources[i] = 'src/' + f
#415	
#416	for p in libvmime_messaging_proto_sources:
#417	        for f in p[1]:
#418	                if f[-4:] == '.hpp':
#419	                        libvmime_all_sources.append('vmime/' + f)
#420	                else:
#421	                        libvmime_all_sources.append('src/' + f)



MODS=dict()

# Begin build lists/aliases


#rda_apps_env.buildpgms3("tools/getmysql.cpp","ALLPGMS,XPERT,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkALLaddcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkALLchgcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkALLimpcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkALLmtncd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkALLrptcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkaddcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkchgcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkdefcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkezcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkfilecd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkgsvcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkhelpcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkimpcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mklistcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkloadcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkmenucd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkmtncd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mknrdcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkprtcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkrptcd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkscncd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mktablecd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mkvircd.cpp","ALLPGMS,CODETOOLS")
rda_apps_env.buildpgms3("tools/mktools.cpp","ALLPGMS,CODETOOLS")

rda_apps_env.buildpgms3("utl/addmenu.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/checkemail.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/chgengine.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/convdata.cpp","ALLPGMS") 
rda_apps_env.buildpgms3("utl/convmenu.cpp","ALLPGMS")
rda_apps_env.buildpgms3("utl/crtfilesec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/crtfldsec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/crtie.cpp","ALLPGMS")
rda_apps_env.buildpgms3("utl/crtimpexp.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/crtprocsec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dat2conv.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfacct.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfadd.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfchg.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfgsv.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfhelp.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfie.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dflist.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfload.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfmatrix.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfmenu.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfmtn.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfnrd.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfprt.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfrpt.cpp","ALLPGMS,XPERT,UTILITIES") 
rda_apps_env.buildpgms3("utl/dfscn.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfscndef.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dftable.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfvir.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dfwww.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doadd.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doadds.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dobackup.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dochg.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/docrontab.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doftpget.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doftpzip.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dogsv.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dohttp.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doie.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/domenu.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doreport.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/doreportall.cpp","ALLPGMS,XPERT,UTILITIES") 
rda_apps_env.buildpgms3("utl/dorestore.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/ezrpt.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/filesec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/fldtest.cpp","ALLPGMS")
rda_apps_env.buildpgms3("utl/gsvscn.cpp","ALLPGMS,XPERT,UTILITIES") 
rda_apps_env.buildpgms3("utl/getforms.cpp","ALLPGMS,XPERT,UTILITIES") 
rda_apps_env.buildpgms3("utl/laser.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/massdel.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/massdfnrd.cpp","ALLPGMS")
rda_apps_env.buildpgms3("utl/massdiff.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/massie.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/massremdel.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/undel.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkcoas.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkdat.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkgui.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkhelp.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkolh.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkrpt.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mksec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkutl.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkvirt.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mkwww.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/mtnmst.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/showfile.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/ok.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/plainpaper.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("utl/progupd.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/procsec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/rbdfil.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/rdabackup.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/rdadiag.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/rdaout.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/remdel.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/restruct.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/setprt.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/procsec.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/uimetric.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/undolog.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/redolog.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/viewlic.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/wwwdoadd.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/wwwdorpt.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/xpertstp.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/sftpfile.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/sftplist.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/setupsftp.cpp","ALLPGMS,XPERT,UTILITIES")
rda_apps_env.buildpgms3("utl/dosshget.cpp","ALLPGMS,XPERT,UTILITIES")
#rda_apps_env.buildpgms3("utl/news.cpp","ALLPGMS,XPERT,UTILITIES")
#rda_apps_env.buildpgms3("utl/convscrn.cpp","ALLPGMS")
#rda_apps_env.buildpgms3("utl/crontran.cpp","ALLPGMS,XPERT,UTILITIES")
#rda_apps_env.buildpgms3("utl/rnumtran.cpp","ALLPGMS,XPERT,UTILITIES")
#rda_apps_env.buildpgms3("utl/undotran.cpp","ALLPGMS,XPERT,UTILITIES")
#rda_apps_env.buildpgms3("utl/viewtran.cpp","ALLPGMS,XPERT,UTILITIES")
#rda_apps_env.buildpgms3("utl/remtrans.cpp","ALLPGMS,XPERT,UTILITIES")

rda_apps_env.buildpgms3("openrda3/openrda.cpp","ALLPGMS,XPERT,OPENRDA")

rda_apps_env.buildpgms3("mkarchive/doarchive.cpp","ALLPGMS,XPERT,ARCHIVE")
rda_apps_env.buildpgms3("mkarchive/mkarchive.cpp","ALLPGMS,XPERT,ARCHIVE")

rda_apps_env.buildpgms3("finmgt/finstp.cpp","ALLPGMS,XPERT,FINMGT")
rda_apps_env.buildpgms3("finmgt/dquery.cpp","ALLPGMS,XPERT,FINMGT")

rda_apps_env.buildpgms3("venpmt/venstp.cpp","ALLPGMS,XPERT,VENPMT")
rda_apps_env.buildpgms3("venpmt/vensetr.cpp","ALLPGMS,XPERT,VENPMT")
rda_apps_env.buildpgms3("venpmt/venschk.cpp","ALLPGMS,XPERT,VENPMT")

rda_apps_env.buildpgms3("purord/prtapo.cpp","ALLPGMS,XPERT,PURORD")
rda_apps_env.buildpgms3("purord/prtapoa.cpp","ALLPGMS,XPERT,PURORD")
rda_apps_env.buildpgms3("purord/purapv.cpp","ALLPGMS,XPERT,PURORD")
rda_apps_env.buildpgms3("purord/purapvgusd.cpp","ALLPGMS,XPERT,PURORD")
rda_apps_env.buildpgms3("purord/purarec.cpp","ALLPGMS,XPERT,PURORD")
rda_apps_env.buildpgms3("purord/purcpo.cpp","ALLPGMS,XPERT,PURORD")
rda_apps_env.buildpgms3("purord/purstp.cpp","ALLPGMS,XPERT,PURORD")

rda_apps_env.buildpgms3("payroll/paydds.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/paysel.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/paynet.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/paymts.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payschk.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/paysddn.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payjmst.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/paybc.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payavw.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/paystp.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payterm.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payunmts.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payvschk.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/payvsetr.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("payroll/subsel.cpp","ALLPGMS,XPERT,PAYROLL")

rda_apps_env.buildpgms3("lvemgt/lveass.cpp","ALLPGMS,XPERT,LVEMGT")

rda_apps_env.buildpgms3("empabs/empaabs.cpp","ALLPGMS,XPERT,EMPABS")
rda_apps_env.buildpgms3("empabs/empaabsw.cpp","ALLPGMS,XPERT,EMPABS")

rda_apps_env.buildpgms3("submgt/subcp.cpp","ALLPGMS,XPERT,SUBMGT")
rda_apps_env.buildpgms3("submgt/subcsub.cpp","ALLPGMS,XPERT,SUBMGT")
rda_apps_env.buildpgms3("submgt/subdtyp.cpp","ALLPGMS,XPERT,SUBMGT")

rda_apps_env.buildpgms3("xpert/vencheckmicr.cpp","ALLPGMS,XPERT,VENPMT")
rda_apps_env.buildpgms3("xpert/vencheckmicra.cpp","ALLPGMS,XPERT,VENPMT")
rda_apps_env.buildpgms3("xpert/dirdep.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdepa.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdep1.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdep1a.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdep2.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdep2a.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdeptxt.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/netzero.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheck.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paychecka.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheck1.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheck1a.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheck2.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheck2a.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheckmicr.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paycheckmicra.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/paychecktxt.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/payvwcheckds.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/payvwcheckmicr.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/payvwcheckmicra.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdepform.cpp","ALLPGMS,XPERT,PAYROLL")
rda_apps_env.buildpgms3("xpert/dirdepforma.cpp","ALLPGMS,XPERT,PAYROLL")

rda_apps_env.buildpgms3("common/fincoa.cpp","ALLPGMS,FINMGT,XPERT,COMMON")
rda_apps_env.buildpgms3("common/comprlst.cpp","ALLPGMS,RLSTMGT,XPERT,COMMON")
rda_apps_env.buildpgms3("common/comprlstbkld.cpp","ALLPGMS,RLSTMGT,XPERT,COMMON")
rda_apps_env.buildpgms3("common/cshaidc.cpp","ALLPGMS,CSHDWR,XPERT,COMMON")
rda_apps_env.buildpgms3("common/itronin.cpp","ALLPGMS,UTLBLL,XPERT,COMMON")
rda_apps_env.buildpgms3("common/itronout.cpp","ALLPGMS,UTLBLL,XPERT,COMMON")
rda_apps_env.buildpgms3("common/miscsstm.cpp","ALLPGMS,MISCBILL,XPERT,COMMON")
rda_apps_env.buildpgms3("common/occcgl.cpp","ALLPGMS,OCCTAX,XPERT,COMMON")
rda_apps_env.buildpgms3("common/prpdmv.cpp","ALLPGMS,PROPERTY,XPERT,COMMON")
rda_apps_env.buildpgms3("common/prpstt.cpp","ALLPGMS,PROPERTY,XPERT,COMMON")
rda_apps_env.buildpgms3("common/puraidc.cpp","ALLPGMS,PURORD,XPERT,COMMON")
rda_apps_env.buildpgms3("common/restsstm.cpp","ALLPGMS,RLSTMGT,XPERT,COMMON")

rda_apps_env.buildpgms3("reqmgt/reqadd.cpp","ALLPGMS,XPERT,REQMGT")
rda_apps_env.buildpgms3("reqmgt/reqchg.cpp","ALLPGMS,XPERT,REQMGT")
rda_apps_env.buildpgms3("reqmgt/reqcpo.cpp","ALLPGMS,XPERT,REQMGT")

rda_apps_env.buildpgms3("rlstmgt/restnew.cpp","ALLPGMS,XPERT,RLSTMGT")

rda_apps_env.buildpgms3("appmgt/appadd.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/appai.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/apppi.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/appsi.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/appsaa.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/appspa.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/appssa.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/appstp.cpp","ALLPGMS,XPERT,APPMGT")
rda_apps_env.buildpgms3("appmgt/apphire.cpp","ALLPGMS,XPERT,APPMGT")

rda_apps_env.buildpgms3("bftmgt/bftabv.cpp","ALLPGMS,XPERT,BFTMGT")
rda_apps_env.buildpgms3("bftmgt/bftddac.cpp","ALLPGMS,XPERT,BFTMGT")
rda_apps_env.buildpgms3("bftmgt/bftmdac.cpp","ALLPGMS,XPERT,BFTMGT")
rda_apps_env.buildpgms3("bftmgt/bftdes.cpp","ALLPGMS,XPERT,BFTMGT")
rda_apps_env.buildpgms3("bftmgt/bftmst.cpp","ALLPGMS,XPERT,BFTMGT")
rda_apps_env.buildpgms3("bftmgt/bfttran.cpp","ALLPGMS,XPERT,BFTMGT")

#rda_apps_env.buildpgms3("mkbnkrec/mkbnkrec.cpp","ALLPGMS,XPERT,BNKREC")
#rda_apps_env.buildpgms3("mkbudprep/mkbudprep.cpp","ALLPGMS,XPERT,BUDPREP")
#rda_apps_env.buildpgms3("mkcatalogue/mkcatalogue.cpp","ALLPGMS,XPERT,PURORD")
#rda_apps_env.buildpgms3("mkfinmgt/mkfinmgt.cpp","ALLPGMS,XPERT,FINMGT")
#rda_apps_env.buildpgms3("mkfixass/mkfixass.cpp","ALLPGMS,XPERT,FIXASS")
#rda_apps_env.buildpgms3("mkinvctl/mkinvctl.cpp","ALLPGMS,XPERT,INVCTL")
#rda_apps_env.buildpgms3("mkpurord/mkpurord.cpp","ALLPGMS,XPERT,PURORD")
#rda_apps_env.buildpgms3("mkreqmgt/mkreqmgt.cpp","ALLPGMS,XPERT,REQMGT")
#rda_apps_env.buildpgms3("mkvenpmt/mkvenpmt.cpp","ALLPGMS,XPERT,VENPMT")
#rda_apps_env.buildpgms3("mkrlstmgt/mkrlstmgt.cpp","ALLPGMS,XPERT,RLSTMGT")
#rda_apps_env.buildpgms3("mkproperty/mkproperty.cpp","ALLPGMS,XPERT,PROPERTY")
#rda_apps_env.buildpgms3("mkcshdwr/mkcshdwr.cpp","ALLPGMS,XPERT,CSHDWR")
#rda_apps_env.buildpgms3("mkmiscbill/mkmiscbill.cpp","ALLPGMS,XPERT,MISCBILL")
#rda_apps_env.buildpgms3("mkocctax/mkocctax.cpp","ALLPGMS,XPERT,OCCTAX")
#rda_apps_env.buildpgms3("mkutlbll/mkutlbll.cpp","ALLPGMS,XPERT,UTLBLL")
#rda_apps_env.buildpgms3("mkbldprmt/mkbldprmt.cpp","ALLPGMS,XPERT,BLDPRMT")
#rda_apps_env.buildpgms3("vairms/mkvairms.cpp","ALLPGMS,XPERT,VAIRMS")
#rda_apps_env.buildpgms3("mkpayroll/mkpayroll.cpp","ALLPGMS,XPERT,PAYROLL")
#rda_apps_env.buildpgms3("mkbftmgt/mkbftmgt.cpp","ALLPGMS,XPERT,PAYROLL")
#rda_apps_env.buildpgms3("common/mkw2.cpp","ALLPGMS,XPERT,PAYROLL")
#rda_apps_env.buildpgms3("common/mk1099.cpp","ALLPGMS,XPERT,VENPMT")
#rda_apps_env.buildpgms3("mklvemgt/mklvemgt.cpp","ALLPGMS,XPERT,LVEMGT")
#rda_apps_env.buildpgms3("mkpostrk/mkpostrk.cpp","ALLPGMS,XPERT,POSTRK")
#rda_apps_env.buildpgms3("mkprsnnl/mkprsnnl.cpp","ALLPGMS,XPERT,PRSNNL")
#rda_apps_env.buildpgms3("mkopenss/mkopenss.cpp","ALLPGMS,XPERT,OPENSS")
#rda_apps_env.buildpgms3("mkappmgt/mkappmgt.cpp","ALLPGMS,XPERT,APPMGT")
#rda_apps_env.buildpgms3("mkempabs/mkempabs.cpp","ALLPGMS,XPERT,EMPABS")
#rda_apps_env.buildpgms3("mksubmgt/mksubmgt.cpp","ALLPGMS,XPERT,SUBMGT")

# Begin build Alias groupings.
for key in MODS:
	#print key+' '+str(MODS[key])
	Alias(key, MODS[key])
