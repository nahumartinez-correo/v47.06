
/* "Well-known" Internet port addresses used by ONA NT/UNIX servers */


/* Comm servers have exclusive use of ports 55465-55565 */
/* NOTE:  additional "COMM/NETWORKING" related ports    */
/*        will decrement from the first port below.     */
/*        i.e., if first port is 55555, next is 55554   */
#define PORT_NETGATE		55555
#define PORT_NETBOSS		55556
#define PORT_PPUXSNA		55557
#define PORT_ACI		55558
#define PORT_PPSNA		55559
#define PORT_DNETX25		55560
#define PORT_MOAMSW		55561
#define PORT_PPNG		55562

/* CDS has exclusive use of ports 55566-55699 */
#define PORT_CDS		55566
#define PORT_CDSxmpMAIN		55566
#define PORT_CDSxmpMainShadow (PORT_CDSxmpMAIN+1)
#define PORT_CDSxmpCL		55568
#define	PORT_CDSxmpCLShadows  (PORT_CDSxmpCL+1)
#define PORT_CDSMAIN		55575
#define PORT_CDSMAINPRIMARY	55575
#define PORT_CDSMAINSHADOW	55576
#define PORT_CDSCL		55577
#define PORT_CDSCLPRIMARY	55577
#define PORT_CDSCLSHADOW	55578
#define PORT_CDSunnamed (PORT_CDSCLSHADOW+2)

/* ports 55700-55799 are for use of other moasaic related services */ 
#define PORT_MOAPPS			55700	/* Moasaic applications rt,crt etc */
#define PORT_UDP_ISDS		55710	/* ISDS udp port */
#define PORT_AUXPRINT		55781
#define PORT_STAFFWARE		55790	/* Staffware srvr UDP port */
#define PORT_GCIFGATE		55791	/* GCIF Gateway */
#define PORT_DIALER		55792	/* modem server */
#define PORT_CB			55793	/* credit bureau server */

/* ISDS tcp port does not use udp */
#define PORT_TCP_ISDS		571		

/* This section deals with the messages and formats that are used on UDP ports */

#define SRVR_LOCATE		0x0001		/* try to locate a server */
#define SRVR_LOCATION	0x0002		/* rsp to SRVR_LOCATE */
#define INFOREQ		0x0003		/* info request to server/client */
#define INFORSP		0x0004		/* response to INFOREQ */
#define TERMINATE	0x0005		/* Terminate Process */


/* SRVR_LOCATE request message structure:
    4 byte length
    2 byte message code = INFOREQ
   '|' terminated ASCII computer name          ex. "PPNT-REQ|"
   '|' terminated ASCII network address        ex. "128.159.2.145|"
   '|' terminated ASCII service name           ex. "CDSmain|"
*/

/* SRVR_LOCATION response message structure:
    4 byte length
    2 byte message code = INFOREQ
   '|' terminated ASCII computer name          ex. "PPNT-CDS|"
   '|' terminated ASCII network address        ex. "128.159.2.128|"
   '|' terminated AsCII service name           ex. "CDSmain|"
   '|' terminated ACSII TCP port number        ex. "55678|"
*/


/* INFOREQ request message structure:
    4 byte length
    2 byte message code = INFOREQ
   '|' terminated ASCII computer name          ex. "PPNT-REQ|"
   '|' terminated ASCII network address        ex. "128.159.2.145|"
*/

/* INFORSP response message structure:
    4 byte length
    2 byte message code = INFORSP
   '|' terminated ASCII computer name          ex. "PPNT-CDS|"
   '|' terminated ASCII network family         ex. "IP|"
   '|' terminated ASCII network address        ex. "128.159.2.45|"
   '|' terminated ASCII desciption             ex. "Central Data Server - CDS|"
   '|' terminated ASCII service name           ex. "CDS-main"
   '|' terminated ASCII executable name        ex. "/usr/iscbr/bin/cds|"
   '|' terminated ASCII part number            ex. "85-12345-A99|"
   '|' terminated ASCII version (n.nn)         ex. "1.99|"
   '|' terminated ASCII link/compile date      ex. "01/03/95|"
   '|' terminated ASCII current number of client connections   ex. "34|"
   '|' terminated ASCII user id				   ex. "root"
   '|' terminated ASCII TCP port number        ex. "1234|"
   '|' terminated ASCII UDP port number        ex. "56789|"
   '|' terminated ASCII Process up time        ex. "hhh:mm:ss|"
   '|' terminated ASCII Free Form Status       ex. "NOT RUN|"
   '|' terminated ASCII Process ID             ex. "12345|"
*/
