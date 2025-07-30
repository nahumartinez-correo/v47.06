/**********************************************************************/
/*  MB-Lite (tm)                                                      */
/*                                                                    */
/*     Global variables which can be changed by a 'configurator'      */
/*     utility.  These variables are used by both the runtime and     */
/*     the development environment.                                   */
/*                                                                    */
/**********************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG
syscfig			config;	/* System configuration vars	*/
#else
#   define EXT extern
#endif

#define		QLHELPNAME		"lib\\qlhelp"
#define		LIBDIR			"lib"
#define		DEFAULTDIR		"\\moa"
#define		OFB_INCDIR		"src\\include\\ofb"

#endif
