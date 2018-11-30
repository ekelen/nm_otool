#include <nm_otool.h>

// TODO: Consistent error codes
int error(const char *arg, int err)
{
	if (err == ERR_USAGE)
	{
		ft_putstr_fd("Unknown flag ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(".  Valid flags:  XXXXX.", 2);
	}
	else if (err == ERR_FILE)
	{
		ft_putstr_fd("Error reading file ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(".", 2);
	}
	else
	{
		ft_putstr_fd(arg, 2);
	}
	ft_putendl_fd("", 2);
	return err;
}

int error_ot(const char *arg, int err, const char *msg)
{
	if (arg)
		ft_printf("%s:\n", arg);
	if (err == ERR_USAGE)
	{
		ft_putstr_fd("Unknown flag ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(".  Valid flag: t", 2);
	}
	else if (err == ERR_FILE)
	{
		ft_putstr_fd("Error reading file ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(".", 2);
	}
	else if (msg)
		ft_putstr_fd(msg, 2);
	ft_putendl_fd("", 2);
	return err;
}

// TODO: Consistent error codes
int error_extended(const char *arg, int err, const char *msg)
{
	ft_putstr_fd("ERROR: ", 2);
	ft_putendl_fd(msg, 2);
	// if (err == ERR_USAGE)
	// {
	// 	ft_putstr_fd("Unknown flag", 2);
	// 	ft_putstr_fd(arg, 2);
	// 	ft_putstr_fd(".  Valid flags:  XXXXX.", 2);
	// }
	// else if (err == ERR_FILE)
	// {
	// 	ft_putstr_fd("Error reading file ", 2);
	// 	ft_putstr_fd(arg, 2);
	// 	ft_putstr_fd(".", 2);
	// }
	// else
	// {
	// 	ft_putstr_fd(arg, 2);
	// }
	// ft_putendl_fd("", 2);
	return err;
}