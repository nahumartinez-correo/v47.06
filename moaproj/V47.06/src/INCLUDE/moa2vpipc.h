//Codigos de retorno generales
#define VPI_OK											0		//Operaci�n exitosa. En el caso de que se est� enviando un comando que recupera varios registros, indica que es el �ltimo registro que queda. 
#define VPI_MORE_REC	          		1		//El registro fue recuperado en forma exitosa, pero faltan registros por enviar
#define VPI_FAIL									 11		//La operaci�n no pudo efectuarse o bien el comando no pudo ser enviado. 
#define VPI_TIMEOUT_EXP	           12		//Tiempo de espera agotado sin que se obtenga una respuesta
#define VPI_INVALID_IN_CMD	      901		//El comando no es soportado por el POS.
#define VPI_INVALID_IN_PARAM	    902		//El formato de los par�metros de entrada no es correcto.
#define VPI_INVALID_OUT_CMD	      903		//La respuesta enviada por el POS no es v�lida. 
#define VPI_GENERAL_FAIL	        999		//Error general en la operaci�n

//Codigos de errores en la informaci�n enviada al POS
#define VPI_INVALID_TICKET	      102		//El n�mero de cup�n no existe en el lote del POS. 
#define VPI_INVALID_PLAN	        103		//El c�digo de plan no existe en las tablas del POS. 
#define VPI_INVALID_INDEX	        104		//El �ndice requerido no existe en la tabla del POS. 
#define VPI_EMPTY_BATCH	          105		//El lote se encuentra vac�o. 

//Codigos de errores en la informaci�n ingresada por el usuario
#define VPI_TRX_CANCELED	        201		//La operaci�n fue cancelada por el usuario 
#define VPI_DIF_CARD	            202		//La tarjeta deslizada por el usuario no coincide con la pedida
#define VPI_INVALID_CARD	        203		//La tarjeta deslizada no es v�lida o hubo un error en la lectura de la banda
#define VPI_EXPIRED_CARD	        204		//La tarjeta deslizada est� vencida. 
#define VPI_INVALID_TRX	          205   //Los datos ingresados no se corresponden con ninguna transacci�n existente en el lote. 

//C�digos de errores en el cierre de la transacci�n
#define VPI_ERR_COM	              301		//El POS no pudo comunicarse con el host.
#define VPI_ERR_PRINT	            302   //El POS no pudo imprimir el ticket.

//Codigos de las transacciones
#define VPI_PURCHASE			          1		//Venta
#define VPI_VOID                    2		//Anulaci�n de venta 
#define VPI_REFUND                  3		//Devoluci�n 
#define VPI_REFUND_VOID             4		//Anulaci�n de devoluci�n 

//Timeout
#define VPI_TIMEOUT_STD            90		//Timeout

//Codigo de respuesta del HOST
#define VPI_HOST_RESPONSE_00			 00   //APROBADA  (authno) - Operaci�n aprobada, emitir cup�n (cargo o ticket). 
#define VPI_HOST_RESPONSE_01       01   //PEDIR AUTORIZACION - Solicitar autorizaci�n telef�nica, en caso de ser aprobada, cargar el c�digo obtenido y dejar la operaci�n en OFFLINE
#define VPI_HOST_RESPONSE_02			 02   //PEDIR AUTORIZACION - Solicitar autorizaci�n telef�nica, en caso de ser aprobada, cargar el c�digo obtenido y dejar la operaci�n en OFFLINE
#define VPI_HOST_RESPONSE_03			 03   //COMERCIO INVALIDO - Verificar par�metros del sistema, c�digo de comercio mal cargado 
#define VPI_HOST_RESPONSE_04       04   //CAPTURAR TARJETA - Denegada, capturar la tarjeta. 
#define VPI_HOST_RESPONSE_05       05   //DENEGADA - Denegada. 
#define VPI_HOST_RESPONSE_07       07   //RETENGA Y LLAME - Denegada, llamar al Centro de Autorizaciones. 
#define VPI_HOST_RESPONSE_08       08   //APROBADA PEDIR IDENT
#define VPI_HOST_RESPONSE_11       11   //APROBADA - Operaci�n aprobada, emitir cup�n (cargo o ticket). 
#define VPI_HOST_RESPONSE_12       12   //TRANSAC. INVALIDA - Verificar el sistema, transacci�n no reconocida en el sistema. 
#define VPI_HOST_RESPONSE_13       13   //MONTO INVALIDO - Verificar el sistema, error en el formato del campo importe
#define VPI_HOST_RESPONSE_14       14   //TARJETA INVALIDA - Denegada, tarjeta no corresponde. 
#define VPI_HOST_RESPONSE_19       19   //REINICIE OPERACI�N 
#define VPI_HOST_RESPONSE_25       25   //NO EXISTE ORIGINAL - Denegada, registro no encontrado en el archivo de transacciones
#define VPI_HOST_RESPONSE_30       30   //ERROR EN FORMATO - Verificar el sistema, error en el formato del mensaje. 
#define VPI_HOST_RESPONSE_38       38   //EXCEDE ING.DE PIN - Denegada, excede cantidad de reintentos de PIN permitidos.
#define VPI_HOST_RESPONSE_43       43   //RETENER TARJETA - Denegada, retener tarjeta. 
#define VPI_HOST_RESPONSE_45       45   //NO OPERA EN CUOTAS - Denegada, tarjeta inhibida para operar en cuotas.
#define VPI_HOST_RESPONSE_46       46   //TARJETA NO VIGENTE - Denegada, tarjeta no est� vigente a�n. 
#define VPI_HOST_RESPONSE_47       47   //PIN REQUERIDO - Denegada, tarjeta requiere ingreso de PIN. 
#define VPI_HOST_RESPONSE_48       48   //EXCEDE MAX. CUOTAS - Denegada, excede cantidad m�xima de cuotas permitida. 
#define VPI_HOST_RESPONSE_49       49   //ERROR FECHA VENCIM. - Verificar el sistema, error en formato de fecha de expiraci�n (vto) 
#define VPI_HOST_RESPONSE_50       50   //ENTREGA SUPERA LIMIT - Denegada, el monto de ENTREGA ingresado est� fuera de los l�mites permitidos.Verifique el monto ingresado. 
#define VPI_HOST_RESPONSE_51       51   //FONDOS INSUFICIENTES - Denegada, no posee fondos suficientes. 
#define VPI_HOST_RESPONSE_53       53   //CUENTA INEXISTENTE - Denegada, no existe cuenta asociada. 
#define VPI_HOST_RESPONSE_54       54   //TARJETA VENCIDA - Denegada, tarjeta expirada
#define VPI_HOST_RESPONSE_55       55   //PIN INCORRECTO - Denegada, c�digo de identificaci�n personal es incorrecto. 
#define VPI_HOST_RESPONSE_56       56   //TARJ. NO HABILITADA - Denegada, emisor no habilitado en el sistema. 
#define VPI_HOST_RESPONSE_57       57   //TRANS. NO PERMITIDA - Verificar el sistema, transacci�n no permitida a dicha tarjeta. 
#define VPI_HOST_RESPONSE_58       58   //SERVICIO INVALIDO - Verificar el sistema, transacci�n no permitida a dicha terminal.
#define VPI_HOST_RESPONSE_61       61   //EXCEDE LIMITE - Denegada, excede l�mite remanente de la tarjeta.
#define VPI_HOST_RESPONSE_65       65   //EXCEDE LIM. TARJETA - Denegada, excede l�mite remanente de la tarjeta.
#define VPI_HOST_RESPONSE_76       76   //LLAMAR AL EMISOR � ERROR � DESC. PROD. - Solicitar autorizaci�n telef�nica, en caso de ser aprobada, cargar el c�digo obtenido y dejar la operaci�n en OFFLINE. 
#define VPI_HOST_RESPONSE_77			 77   //ERROR PLAN/CUOTAS � ERROR RECONCILIACI�N - Denegada, cantidad de cuotas inv�lida para el plan seleccionado
#define VPI_HOST_RESPONSE_85       85   //APROBADA � LOTE NO ENCONTRADA - Operaci�n aprobada, emitir cup�n (cargo o ticket). 
#define VPI_HOST_RESPONSE_88       88   //APROB. CLIENTE LLAME 
#define VPI_HOST_RESPONSE_89       89   //TERMINAL INVALIDA - Denegada, n�mero de terminal no habilitado por el Emisor. 
#define VPI_HOST_RESPONSE_91       91   //EMISOR FUERA LINEA - Solicitar autorizaci�n telef�nica, en caso de ser aprobada, cargar el c�digo obtenido y dejar la operaci�n en OFFLINE.
#define VPI_HOST_RESPONSE_94			 94   //NRO. SEC. DUPLICAD - Denegada. Error en mensaje. Env�e nuevamente la transacci�n incrementando en uno el system trace de la misma. 
#define VPI_HOST_RESPONSE_95       95   //RE-TRANSMITIENDO - Diferencias en la conciliaci�n del cierre, env�e Batch Upload. 
#define VPI_HOST_RESPONSE_96       96   //ERROR EN SISTEMA � MENSAJE INVALIDO - Mal funcionamiento del sistema. Solicitar autorizaci�n telef�nica. 
#define VPI_HOST_RESPONSE_BB       BB   //COMP NO DISPONILE INTENTE MAS TARDE - El host responde este codgo cuando la captura de lote no esta disponible. EL terminal podr� seguir operando pero no ser� posible efectuar el cierre de lote. 
#define VPI_HOST_RESPONSE_xx       xx   //RECHAZADA (codnum) - Denegada, cualquier otro c�digo no contemplado en tabla. 
