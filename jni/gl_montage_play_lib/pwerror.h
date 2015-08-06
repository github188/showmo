
/*----------------------------------------------------------------------------------------------
*
* This file is Puwell's property. It contains Puwell's trade secret, proprietary and 		
* confidential information. 
* 
* The information and code contained in this file is only for authorized Puwell employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify Puwell and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*---------------------------------------------------------------------------------------------*/

#ifndef __PWERROR_H__
#define __PWERROR_H__



#define GERR_NONE						0
#define GOK								0



#define GERR_BASIC_BASE					0X0001
#define GERR_UNKNOWN					GERR_BASIC_BASE
#define GERR_INVALID_PARAM				(GERR_BASIC_BASE+1)
#define GERR_UNSUPPORTED				(GERR_BASIC_BASE+2)
#define GERR_NO_MEMORY					(GERR_BASIC_BASE+3)
#define GERR_BAD_STATE					(GERR_BASIC_BASE+4)
#define GERR_USER_CANCEL				(GERR_BASIC_BASE+5)
#define GERR_EXPIRED					(GERR_BASIC_BASE+6)
#define GERR_USER_PAUSE					(GERR_BASIC_BASE+7)
#define GERR_BUFFER_OVERFLOW		    (GERR_BASIC_BASE+8)
#define GERR_BUFFER_UNDERFLOW		    (GERR_BASIC_BASE+9)
#define GERR_NO_DISKSPACE				(GERR_BASIC_BASE+10)



#define GERR_FILE_BASE					0X1000
#define GERR_FILE_GENERAL				GERR_FILE_BASE
#define GERR_FILE_NOT_EXIST				(GERR_FILE_BASE+1)
#define GERR_FILE_EXIST					(GERR_FILE_BASE+2)
#define GERR_FILE_EOF					(GERR_FILE_BASE+3)
#define GERR_FILE_FULL					(GERR_FILE_BASE+4)
#define GERR_FILE_SEEK					(GERR_FILE_BASE+5)
#define GERR_FILE_READ					(GERR_FILE_BASE+6)
#define GERR_FILE_WRITE					(GERR_FILE_BASE+7)
#define GERR_FILE_OPEN					(GERR_FILE_BASE+8)
#define GERR_FILE_DELETE				(GERR_FILE_BASE+9)
#define GERR_FILE_RENAME				(GERR_FILE_BASE+10)

#define GO(res, EXIT, FUNCTION, OK)		{if((res = FUNCTION) != OK) goto EXIT;} 
#define UNGO(res, EXIT, FUNCTION, NOK)		{if((res = FUNCTION) == NOK) goto EXIT;} 
//	GO(res, EXIT, begood, GOK)

#define GOE(res, ERR, EXIT)			{res = ERR; goto EXIT;} 
//	GOE(res, GERR_NO_MEMORY, EXT);

#define GERR_VM_BASE				0X2000
#define GERR_VM_WRONG_CARD			(GERR_VM_BASE+1)
#define GERR_VM_DAMAGE_CARD_DPS		(GERR_VM_BASE+2)
#define GERR_VM_THREAD_ERR			(GERR_VM_BASE+3)
#define GERR_VM_ENGINE_ERR			(GERR_VM_BASE+4)
#define GERR_VM_SERIAL_ERR			(GERR_VM_BASE+5)
#define GERR_VM_UNSUPPORT_EQUIT		(GERR_VM_BASE+6)
#define GERR_VM_LIST_FULL			(GERR_VM_BASE+7)
#define GERR_VM_UNBUILT_FILE		(GERR_VM_BASE+8)
#define GERR_VM_SYSTEM_THREAD		(GERR_VM_BASE+9)

#define GERR_FS_BASE				0X3000				// FreeSee
#define GERR_FS_INVALID_PARAM		(GERR_FS_BASE+1)
#define GERR_FS_UNSUPPORTED			(GERR_FS_BASE+2)
#define GERR_FS_NO_MEMORY			(GERR_FS_BASE+3)
#define GERR_FS_NO_MSGRETURN_BUFF	(GERR_FS_BASE+4)
#define GERR_FS_NO_COMRETURN_BUFF	(GERR_FS_BASE+5)

#define GERR_TD_BASE				0X4000				// TargetDetection
#define GERR_TD_INVALID_PARAM		(GERR_TD_BASE+1)
#define GERR_TD_UNSUPPORTED			(GERR_TD_BASE+2)
#define GERR_TD_NO_MEMORY			(GERR_TD_BASE+3)
#define GERR_TD_NO_MSGRETURN_BUFF	(GERR_TD_BASE+4)

#define GERR_AG_BASE				0X5000				// Agent
#define GERR_AG_INVALID_PARAM		(GERR_AG_BASE+1)
#define GERR_AG_UNSUPPORTED			(GERR_AG_BASE+2)
#define GERR_AG_NO_MEMORY			(GERR_AG_BASE+3)
#define GERR_AG_NO_MSGRETURN_BUFF	(GERR_AG_BASE+4)

#define GERR_CM_BASE				0X6000				// Commander
#define GERR_CM_INVALID_PARAM		(GERR_CM_BASE+1)
#define GERR_CM_UNSUPPORTED			(GERR_CM_BASE+2)
#define GERR_CM_NO_MEMORY			(GERR_CM_BASE+3)
#define GERR_CM_NO_MSGRETURN_BUFF	(GERR_CM_BASE+4)

#define GERR_MT_BASE				0X7000				// Matrix
#define GERR_MT_INVALID_PARAM		(GERR_MT_BASE+1)
#define GERR_MT_UNSUPPORTED			(GERR_MT_BASE+2)
#define GERR_MT_NO_MEMORY			(GERR_MT_BASE+3)
#define GERR_MT_NO_MSGRETURN_BUFF	(GERR_MT_BASE+4)
#define GERR_MT_NO_MATRIX_HANDLE	(GERR_MT_BASE+5)
#define GERR_MT_NO_MATRIX_DATABASE	(GERR_MT_BASE+6)


#endif
 
