#ifndef SWERROR_H
#define SWERROR_H
#define SWERROR_DIR "lib"


extern const char  Error_Msg_DIR[];

	//XDLL char		*swerrormsg(register int);
	//XDLL void 		swerrorvmsg(char *space, int n, ... );
	char	*swerrormsg(register int);
	void	swerrorvmsg(char *space, int n, ... );
	void	swvargerror(int	n, ...);

	void	swerror(int);
	char	*swtextmsg(register int);
	char	*swerrnomsg(register int errno);
	void	swvargerror(int n, ...);
	void	swerrorvmsg(char *space, int n, ...);
	void	swtextvmsg(char *buf, int n, ...);
	int		show_message(int hwnd, char *message, char *caption, int code);
#endif
