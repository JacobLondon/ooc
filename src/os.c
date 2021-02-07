#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

#include <ooc/class.h>
#include <ooc/string.h>
#include <ooc/vector.h>
#include <ooc/util.h>
#include <ooc/os.h>

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static int           NamespaceOs_System         (char *command);
static int           NamespaceOs_Chdir          (char *location);
static var           NamespaceOs_Pwd            (void);
static var           NamespaceOs_Ls             (char *directory);

/**********************************************************
 * Definitions
 **********************************************************/

struct NamespaceOs Os = {
	.System = NamespaceOs_System,
	.Chdir  = NamespaceOs_Chdir,
	.Pwd    = NamespaceOs_Pwd,
	.Ls     = NamespaceOs_Ls,
};

/**********************************************************
 * Namespace Functions
 **********************************************************/

static int NamespaceOs_System(char *command)
{
	assert(command);
	return system(command);
}

static int NamespaceOs_Chdir(char *location)
{
	assert(location);
	return chdir(location);
}

static var NamespaceOs_Pwd(void)
{
	char buf[PATH_MAX];
	var path;

	if (getcwd(buf, sizeof(buf)) == NULL) {
		path = New(String.Class, ".");
		return path;
	}

	path = New(String.Class, buf);
	return path;
}

static var NamespaceOs_Ls(char *directory)
{
	DIR *d;
	struct dirent *dir;
	var vec;
	var name;

	if (directory == NULL) {
		directory = ".";
	}

	vec = New(Vector.Class);
	d = opendir(directory);
	if (!d) {
		return vec;
	}

	while ((dir = readdir(d)) != NULL) {
		name = String.Dup(dir->d_name);
		Vector.Take_back(vec, name);
	}

	closedir(d);
	return vec;
}
