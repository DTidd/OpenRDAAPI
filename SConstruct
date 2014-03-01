# This file will import base OS environment strings and set the 
# default base build environment to be passed to the SConscript file
# for detailed builds in separate environments.
import SCons
import platform
import os
import re


#Detects the number of CPUs on a system. 
def detectCPUs():
	# Linux, Unix and MacOS:
	if hasattr(os, "sysconf"):
	   if os.sysconf_names.has_key("SC_NPROCESSORS_ONLN"):
	       # Linux & Unix:
	       ncpus = os.sysconf("SC_NPROCESSORS_ONLN")
	       if isinstance(ncpus, int) and ncpus > 0:
	           return ncpus
	   else: # OSX:
	       return int(os.popen2("sysctl -n hw.ncpu")[1].read())
	# Windows:
	if os.environ.has_key("NUMBER_OF_PROCESSORS"):
	       ncpus = int(os.environ["NUMBER_OF_PROCESSORS"]);
	       if ncpus > 0:
	           return ncpus
	return 1 # Default


if re.search("localc[(+)][(+)]", os.getcwd()):
	print 'found localc++'
	LOCAL_CPP=int(1)
	print 'PWD '+ os.getcwd()+ '/'
else:	
	print 'found localc'
	print 'PWD '+ os.getcwd()+ '/'
	LOCAL_CPP=int(0)


# This should build OS_ENV, OS_DEBUG_ENV, RDA_BASE_ENV, RDA_BASE_DEBUG_ENV
# It would also ideally build/use a static table of name = primary source file
# for a mymake type functionality. If not found in the table, it would run
# a source parser on the base name in the current directory with the RDA_BASE_ENV

#vars = Variables()
#vars.AddVariables(
#	BoolVariable('warnings', 'compilation with -Wall and similiar', 1)
#	)

#opts = Options()
AddOption(
	'--a',
        default='false',
        dest='delobj',
	action='store_true',
	help='Delete objects for specified targets only.')

AddOption(
	'--enable-warnings',
        default='false',
        dest='enable-warnings',
	action='store_true',
	help='Delete objects for specified targets only.')

AddOption(
	'--use-shm',
        dest='useshm',
	action='store_true',
	help='Use /dev/shm ramdisk for build objects to increase build speed.')

AddOption(
	'--strip',
        default=True,
        dest='use_strip',
	action='store_true',
	help='Strip built binaries.')

AddOption(
	'--no-strip',
        default=True,
        dest='use_strip',
	action='store_false',
	help='Do not strip built binaries.')

AddOption(
	'--debug-build',
        default=False,
        dest='debug_build',
	action='store_true',
	help='Strip built binaries.')

AddOption(
	'--use-rdadiag',
        default='false',
        dest='use_rdadiag',
	action='store_true',
	help='Uses #define to enable RDA Diagnostics.')

AddOption(
	'--use-exdir',
        default='true',
        dest='use_exdir',
	action='store_true',
	help='Copy binaries to $EXDIR.')

AddOption(
	'--no-exdir',
        default='true',
        dest='use_exdir',
	action='store_false',
	help='Copy binaries to $EXDIR.')

AddOption(
	'--use-qadir',
        dest='use_qadir',
	action='store_true',
	help='Copy binaries to QA directory.')

AddOption(
	'--gui',
        default='WT',
        dest='GUI_TYPE',
	type='string',
	nargs=1,
	action='store',
	help='Set GUI build type.  Valid options are QT and WT')

AddOption(
	'--nice',
        dest='NICE_VALUE',
	type='string',
	nargs=1,
	action='store',
	help='Set Nice Value For Build process.  Valid options are 1-20')


#       default='/opt/mariadb-5.5.31-rda',
#	default='/opt/mysql-5.0.37-rda',
AddOption(
	'--mysql-path',
	default='/opt/mysql-5.0.37-rda',
        dest='MYSQL_PATH_VALUE',
	type='string',
	nargs=1,
	action='store',
	help='Set Path for MySQL/MariaDB libs/includes.')


AddOption(
	'--use-clang',
        dest='useclang',
	action='store_true',
	help='Use clang front end to LLVM to compile and link code.')


#if os.environ.has_key('USE_MARIADB'):
#	if os.path.isdir('/opt/mariadb-5.5.33a'):
#		MYSQL_PATH='/opt/mariadb-5.5.33a'
#		STORE_MARIADB=int(1)
#		STORE_MYSQL=int(0)
#elif os.environ.has_key('USE_MYSQL'):
#	if os.path.isdir('/opt/mysql-5.0.37-rda'):
#		MYSQL_PATH='/opt/mysql-5.0.37-rda'
#		STORE_MARIADB=int(0)
#		STORE_MYSQL=int(1)
#else:
#	if os.path.isdir('/opt/mysql-5.0.37-rda'):
#		MYSQL_PATH='/opt/mysql-5.0.37-rda'
#		STORE_MARIADB=int(0)
#		STORE_MYSQL=int(1)
#
#AddOption(
#	'--mysql-path',
#	default=MYSQL_PATH,
#        dest='MYSQL_PATH_VALUE',
#	type='string',
#	nargs=1,
#	action='store',
#	help='Set Path for MySQL/MariaDB libs/includes.')


#AddOption(
#	'--use-mariadb',
#	default=STORE_MARIADB,
#        dest='usemariadb',
#	action='store_true',
#	help='Link against MariaDB instead of MySQL.')
#
#AddOption(
#	'--use-mysql',
#	default=STORE_MYSQL,
#        dest='usemysql',
#	action='store_true',
#	help='Link against MySQL instead of MariaDB.')
#
AddOption(
	'--enable-mysql-server',
        default=1,
        dest='enable_mysql_server',
	action='store_true',
	help='Build with MySQL server support (libmysqlclient).')

AddOption(
	'--disable-mysql-server',
        default=1,
        dest='enable_mysql_server',
	action='store_false',
	help='Build without MySQL server support (libmysqlclient).')


AddOption(
	'--enable-myisam-engine',
        default=1,
        dest='enable_myisam_engine',
	action='store_true',
	help='Build with custom MyISAM engine support (libmyisam).')

AddOption(
	'--disable-myisam-engine',
        default=1,
        dest='enable_myisam_engine',
	action='store_false',
	help='Build without custom MyISAM engine support (libmyisam).')


# Added to allow for selecting WINDOWS or CONSOLE subsystems on Windows			
AddOption(
	'--win32-windows',
        default='true',
        dest='use_win32_windows_subsystem',
	action='store_true',
	help='Copy binaries to $EXDIR.')

AddOption(
	'--win32-console',
        default='true',
        dest='use_win32_windows_subsystem',
	action='store_false',
	help='Copy binaries to QA directory.')

AddOption(
	'--use-fastcgi',
        default='false',
        dest='use_fastcgi',
	action='store_true',
	help='Use FastCGI library with Wt instead of builtin http server library.')


#if GetOption('use_qadir'):
#	SetOption('use_exdir','false')
#else:
	#SetOption('use_exdir',1)


if platform.system() == 'Linux':
	os.nice(15)

if GetOption('num_jobs') == 1:
	
	print 'Number of build threads temporarily defaulting to 1'
	my_num_cpus=detectCPUs()
	if platform.system() == 'Linux':
		if my_num_cpus <= 2:
			SetOption('num_jobs',1)
		elif my_num_cpus <= 8:
			SetOption('num_jobs',my_num_cpus)
		elif my_num_cpus == 16:
			SetOption('num_jobs',(my_num_cpus - 4))
		else:
			SetOption('num_jobs',my_num_cpus)

	if platform.system() == 'Windows':
		SetOption('num_jobs',detectCPUs())
	print 'NUM_CPUS='+str(GetOption('num_jobs'))


print 'FASTCGI='+str(GetOption('use_fastcgi'))

print 'NUM_CPUS='+str(GetOption('num_jobs'))
#SetOption('num_jobs',1)

print "Use MySQL ["+str(GetOption('enable_mysql_server'))+"]"
print "Use MyISAM ["+str(GetOption('enable_myisam_engine'))+"]"

if GetOption('debug_build') == 1:
	print "BUILD_TYPE=" + "debug"
else:
	print "BUILD_TYPE=" + "release"



# Test if MYSQL_PATH value exists.
if not os.path.isdir(GetOption('MYSQL_PATH_VALUE')):
	print 'Invalid Value for MYSQL PATH, [%s] does not exist.' % GetOption('MYSQL_PATH_VALUE')







# Command line Variables
#vars = Variables('default_variables.py')
#vars.Add(EnumVariable('GUI', 'Set GUI build type.  Valid options are QT and WT', 'QT', allowed_values=('QT', 'WT', 'QT4'), ignorecase=2))
#Help(vars.GenerateHelpText(env))

#if env['debug'] == 'full':
#print env['GUI']

#unknown = vars.UnknownVariables()
#if unknown:
#	print "Unknown variables:", unknown.keys()


if platform.system() == 'Linux':
	if os.environ.has_key('BUILDENV'):
		if GetOption('GUI_TYPE') == 'WT':
			print 'GUI_TYPE=WT\n'
			if GetOption('useshm'):
				build_dir = '/dev/shm/build/' + os.environ.get('BUILDENV') + '-wt'
			else:
				build_dir = '/rdadev/build/' + os.environ.get('BUILDENV') + '-wt'
				
			#SetOption('use_exdir','false')
		else:
			print 'GUI_TYPE!=WT\n'
			if GetOption('useshm'):
				build_dir = '/dev/shm/build/' + os.environ.get('BUILDENV') 
			else:
				build_dir = '/rdadev/build/' + os.environ.get('BUILDENV') 

		#build_dir = '/rdadev/build/' + os.environ.get('BUILDENV') 
		#print build_dir
	else:
		print "You must have $BUILDENV set before compiling."
		Exit(1)
	 
if platform.system() == 'Windows':
	if os.environ.has_key('BUILDENV'):
		build_dir = 'C:\\build\\' + os.environ.get('BUILDENV') 
	else:
		print "You must have $BUILDENV set before compiling."
		Exit(1)
 


"""
if os.environ.has_key('BUILD_TYPE'):
	if os.environ.get('BUILD_TYPE') == "debug":
		print "BUILD_TYPE= " + os.environ.get('BUILD_TYPE')
		SetOption('use_strip','false')
		#build_type = '/rdadev/build/' + os.environ.get('BUILDENV') 
		#print build_dir
	else:
		print "BUILD_TYPE=" + "release"
else:
	print "BUILD_TYPE=" + "release"
	#print "You must have $BUILD_TYPE set before compiling."
	#Exit(1)
 
"""



uname = platform.uname()
#print uname
localc_dir=os.getcwd()
Export('localc_dir')
Export('build_dir')
Export('LOCAL_CPP')

print COMMAND_LINE_TARGETS
print "localc_dir=%s" % (localc_dir)
print "build_dir=%s" % (build_dir)

rootEnv = Environment(BUILDROOT="BUILD")
env=rootEnv
if platform.system() == 'Linux':
	testrun = env.SConscript("SConscript",build_dir=build_dir)
	
if platform.system() == 'Windows':
	testrun = env.SConscript("SConscript",build_dir=build_dir)
