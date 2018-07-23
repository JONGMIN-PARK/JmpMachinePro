/* RUNTIME.H Header file for runtime.cpp

HISTORY

23Aug99 JET Added PmacDPRNetActualPosition declaration
23Mar99 JET Added GetCurrentLabelEx declaration.
11Jan99 JET remove excess resetcycletime decl., added Pp_Clear_Save_Ranges,Pp_Add_Save_Range
            Pp_Save_Vars and Pp_Load_Vars declarations.
08Aug98 AS  added PmacGetParameters,PmacAbortDownload,PmacSetMaxDownloadErrors,PmacDPRRotBufClr
03Aug98 JET added link to PmacDPRAddressRange()
30Jul98 JET added WriteBufferEx, VarBufInitEx, and VarBufChangeEx for turbo
            change some dprbkg.c functions from returning long to BOOL
            remove redundant GetPmacType definitions.
20Jul98 JET Added GetToolCompWear() and SetToolCompWear()
17Jul98 JET Remove extoffsetset() which is not used or exported.
09Jul98 JET Added variable for link to "PmacDPRBackGroundVar"
19Jun98 JET eliminate duplicate code with NO_EXTERN, reformat for easy maint.
16Jun98 JET add ncPpGetDouble and ncPpSetDouble for parametric programming.
13Apr98 JET add NcAddErrorRecordEx
02Apr98 JET Add DPR Numeric read/write functions, remove redundant typedefs.
13Mar98 JET Add DPRWriteBuffer
12Mar98 JET Add DPRVarBufChange, DPRBackground, DPRFloat, DPRLFixed
13Jan98 JET added GetErrorLevel for probing display update.
11Dec97 JET Made RewindTextBuffer() arguments agree with PMAC.DLL version.
19Nov97 JET added PMACLMH function
*/

#ifndef RUNTIME_H
  #define RUNTIME_H

  #include <pmacu.h>
  extern  HINSTANCE   hPmacLib; // Instantiated in Runtime.CPP

/* 64bit로 컴파일시 전처리기 정의(Preprocessor definitions)란에 WIN32 대신 WIN64로 변경하셔야 합니다. */
#ifdef WIN64
  #define DRIVERNAME          TEXT("Pcomm64.dll") //name of the user-dll 64 Bit driver
#else
  #define DRIVERNAME          TEXT("Pcomm32.dll") //name of the user-dll 32 Bit driver
#endif

  #ifdef  __BORLANDC__
    #ifdef  __BORLANDC__ >= 0x0530 // C Builder 3.0 doesn't do enums
      #define PROGRAM        int   // as ints if we are using vcl so
      #define MOTION         int   // make them ints
      #define MOTIONMODE     int
      #define PMACDEVICETYPE int
      #define ASCIIMODE      int
      #define BUSTYPE        int
      #define LOCATIONTYPE   int
      #define GATMODE        int
    #endif
  #endif

//************************************************************************
// COMM Type DefinesTEet
//************************************************************************
typedef void( CALLBACK *PMACDPRPCITEST ) ( DWORD dwDevice );
typedef BOOL( CALLBACK *OPENPMACDEVICE ) ( DWORD dwDevice );
typedef long (CALLBACK * SELECTPMACDEVICE) ( HWND hwnd );
typedef long (CALLBACK *NUMBEROFDEVICES)();
typedef BOOL (CALLBACK *CLOSEPMACDEVICE) ( DWORD dwDevice );
typedef BOOL (CALLBACK *PMACSETLANGUAGE) ( const DWORD dwDevice, char *locale );
typedef BOOL (CALLBACK *PMACSETWINDOW) ( DWORD dwDevice, HWND hWnd );
typedef void (CALLBACK *PMACLMH) ( HANDLE * hndl );
typedef BOOL (CALLBACK *SETLANGUAGEMODULE) ( const char *locale, const HANDLE hProcess, HANDLE * h );
typedef LPSTR( CALLBACK *SZLOADSTRINGA ) ( HANDLE hInst, int iID );
typedef PCHAR( CALLBACK *SZSTRIPCONTROLCHARA ) ( PCHAR str );
typedef PCHAR (CALLBACK *SZSTRIPWHITESPACEA) ( PCHAR str );
typedef int (CALLBACK *GETERROR) ( DWORD dwDevice );
typedef ASCIIMODE (CALLBACK *GETASCIICOMM) ( DWORD dwDevice );
typedef BOOL (CALLBACK *SETASCIICOMM) ( DWORD dwDevice, ASCIIMODE m );
typedef LOCATIONTYPE (CALLBACK *GETLOCATION) (DWORD dwDevice);
typedef int (CALLBACK *GETENUMERATION) (DWORD dwDevice);
typedef BOOL (CALLBACK *MOTIONBUFOPEN) ( DWORD dwDevice );
typedef BOOL (CALLBACK *ROTBUFOPEN) ( DWORD dwDevice );
typedef BOOL (CALLBACK *GETVARIABLESTRA) ( DWORD dwDevice, CHAR ch, LPSTR str, UINT num );
typedef short int (CALLBACK *GETIVARIABLE) ( DWORD dwDevice, UINT num, short int def );
typedef long (CALLBACK *GETVARIABLELONG) ( DWORD dwDevice, TCHAR ch, UINT num, long def );
typedef double (CALLBACK *GETVARIABLEDOUBLE) ( DWORD dwDevice, TCHAR ch, UINT num, double def );
typedef long (CALLBACK *GETIVARIABLELONG) ( DWORD dwDevice, UINT num, long def );
typedef double (CALLBACK *GETIVARIABLEDOUBLE) ( DWORD dwDevice, UINT num, double def );
typedef void (CALLBACK *SETIVARIABLE) ( DWORD dwDevice, UINT num, short int val );
typedef void (CALLBACK *SETIVARIABLELONG) ( DWORD dwDevice, UINT num, long val );
typedef void (CALLBACK *SETIVARIABLEDOUBLE) ( DWORD dwDevice, UINT num, double val );
//++ Added
typedef void (CALLBACK *SETVARIABLE) (DWORD dwDevice, CHAR ch, UINT num, short int val);
typedef void (CALLBACK *SETVARIABLELONG) (DWORD dwDevice, TCHAR ch, UINT num, long val);
typedef void (CALLBACK *SETVARIABLEDOUBLE) (DWORD dwDevice, TCHAR ch, UINT num, double val);
// by DTK

typedef int (CALLBACK *GETPROGRAMINFO) ( DWORD dwDevice, BOOL plc, int num, UINT * sadr, UINT * fadr );
//typedef int    (CALLBACK* GETProgramInfo)(DWORD dwDevice,BOOL plc,int num,UINT *sadr,UINT *fadr);
typedef PUSER_HANDLE( CALLBACK *GETUSERHANDLE ) ( DWORD dwDevice );
typedef BOOL (CALLBACK *CONFIGURE) ( HWND hwnd, DWORD dwDevice );
typedef void (CALLBACK *LOCKPMAC) ( DWORD dwDevice );
typedef void (CALLBACK *RELEASEPMAC) ( DWORD dwDevice );
typedef BOOL (CALLBACK *SETCRITICALIVARS) ( DWORD dwDevice );
typedef BOOL (CALLBACK *PMACISTURBO) ( DWORD dwDevice );
typedef BOOL (CALLBACK *DOCHECKSUMS) ( DWORD dwDevice, BOOL bVal );

typedef BOOL (CALLBACK *SERDOCHECKSUMS) ( DWORD dwDevice, BOOL do_checksums );
typedef BOOL (CALLBACK *SERCHECKSENDLINEA) ( DWORD dwDevice, char *outstr, char *command_csum );
typedef int (CALLBACK *SERCHECKGETLINEA) ( DWORD dwDevice, PCHAR response, UINT maxchar, PUINT num_char );
typedef int (CALLBACK *SERCHECKRESPONSEA) ( DWORD dwDevice, char *response, UINT maxchar, char *outstr );
typedef int (CALLBACK *SERCHECKALLWENTWELL)(DWORD dwDevice,int ret,char *cp,char csum) ;

typedef DWORD( CALLBACK *SERGETPORT ) ( DWORD dwDevice );
//typedef BOOL (CALLBACK *SERSETPORT) ( DWORD dwDevice, DWORD p );
typedef DWORD (CALLBACK *SERGETBAUDRATE) ( DWORD dwDevice );
typedef BOOL (CALLBACK *SERSETBAUDRATE) ( DWORD dwDevice, DWORD br );

// ASCII string exported functions
typedef PCHAR (CALLBACK *GETROMDATEA) ( DWORD dwDevice, LPSTR s, int maxchar );
typedef PCHAR (CALLBACK *GETROMVERSIONA) ( DWORD dwDevice, LPSTR s, int maxchar );
typedef int (CALLBACK *GETPMACTYPE) ( DWORD dwDevice );
typedef BOOL (CALLBACK *GETIVARIABLESTRA) ( DWORD dwDevice, LPSTR str, UINT num );
typedef int (CALLBACK *MULTIDOWNLOADA) ( DWORD dwDevice, DOWNLOADMSGPROC msgp, LPCSTR outfile,
                LPCSTR inifile, LPCSTR szUserId, BOOL macro, BOOL map, BOOL log, BOOL dnld );
typedef int (CALLBACK *ADDDOWNLOADFILEA) ( DWORD dwDevice, LPCSTR inifile, LPCSTR szUserId, LPCSTR szDLFile );
typedef int (CALLBACK *REMOVEDOWNLOADFILEA) ( DWORD dwDevice, LPCSTR inifile, LPCSTR szUserId, LPCSTR szDLFile );
typedef void (CALLBACK *RENUMBERFILESA) ( DWORD dwDevice, int file_num, LPCSTR szIniFile );
typedef int (CALLBACK *GETERRORSTRA) ( DWORD dwDevice, LPSTR str, int maxchar );

// Unicode string exported functions
typedef BOOL (CALLBACK *GETIVARIABLESTRW) ( DWORD dwDevice, LPWSTR str, UINT num );
typedef WORD( CALLBACK *GETPLCSTATUS ) ( DWORD dwDevice, TOTAL_PLC_STATUS_STRUCT *plc_stat );
typedef WORD( CALLBACK *ISANYPLCRUNNING) (DWORD dwDevice);

typedef int (CALLBACK *MULTIDOWNLOADW) ( DWORD dwDevice, DOWNLOADMSGPROC msgp, LPWSTR outfile,
                LPWSTR inifile, LPWSTR szUserId, BOOL macro, BOOL map, BOOL log, BOOL dnld );
typedef int (CALLBACK *ADDDOWNLOADFILEW) ( DWORD dwDevice, LPWSTR inifile, LPWSTR szUserId, LPWSTR szDLFile );
typedef int (CALLBACK *REMOVEDOWNLOADFILEW) ( DWORD dwDevice, LPWSTR inifile, LPWSTR szUserId, LPWSTR szDLFile );
typedef void (CALLBACK *RENUMBERFILESW) ( DWORD dwDevice, int file_num, LPWSTR szIniFile );
typedef int (CALLBACK *GETERRORSTRW) ( DWORD dwDevice, LPWSTR str, int maxchar );


typedef BOOL (CALLBACK *READREADY) ( DWORD dwDevice );
typedef int (CALLBACK *SENDCHARA) ( DWORD dwDevice, CHAR outch );
typedef int (CALLBACK *SENDCHARW) ( DWORD dwDevice, WCHAR outch );
typedef int (CALLBACK *SENDLINEA) ( DWORD dwDevice, LPSTR outstr );
typedef int (CALLBACK *SENDLINEW) ( DWORD dwDevice, LPWSTR outstr );
typedef int (CALLBACK *GETLINEA) ( DWORD dwDevice, LPSTR s, UINT maxchar );
typedef int (CALLBACK *GETLINEEXA) ( DWORD dwDevice, LPSTR s, UINT maxchar );
typedef int (CALLBACK *GETLINEEXW) ( DWORD dwDevice, LPWSTR s, UINT maxchar );
typedef int (CALLBACK *WAITGETLINEA) ( DWORD dwDevice, LPSTR s, UINT maxchar );
typedef int (CALLBACK *GETLINEW) ( DWORD dwDevice, LPWSTR s, UINT maxchar );
typedef int (CALLBACK *GETRESPONSEA) ( DWORD dwDevice, LPSTR s, UINT maxchar, LPSTR outstr );
typedef int (CALLBACK *GETRESPONSEW) ( DWORD dwDevice, LPWSTR s, UINT maxchar, LPWSTR outstr );
typedef int (CALLBACK *GETCONTROLRESPONSEA) ( DWORD dwDevice, LPSTR s, UINT maxchar, CHAR outchar );
typedef int (CALLBACK *GETCONTROLRESPONSEW) ( DWORD dwDevice, LPWSTR s, UINT maxchar, WCHAR outchar );
typedef int (CALLBACK *GETRESPONSEEXA) ( DWORD dwDevice, LPSTR s, UINT maxchar, LPSTR outstr );
typedef int (CALLBACK *GETRESPONSEEXW) ( DWORD dwDevice, LPWSTR s, UINT maxchar, LPWSTR outstr );
typedef int (CALLBACK *GETCONTROLRESPONSEEXA) ( DWORD dwDevice, LPSTR s, UINT maxchar, CHAR outchar );
typedef int (CALLBACK *GETCONTROLRESPONSEEXW) ( DWORD dwDevice, LPWSTR s, UINT maxchar, WCHAR outchar );
typedef void (CALLBACK *FLUSH) ( DWORD dwDevice );
typedef void (CALLBACK *SENDCOMMANDA) ( DWORD dwDevice, LPSTR outchar );
typedef void (CALLBACK *SENDCOMMANDW) ( DWORD dwDevice, LPWSTR outstr );
typedef int (CALLBACK *SENDCTRLCHARA) ( DWORD dwDevice, CHAR outstr );
typedef int (CALLBACK *SENDCTRLCHARW) ( DWORD dwDevice, WCHAR outstr );
typedef BOOL (CALLBACK *INBOOTSTRAP) ( DWORD dwDevice );
typedef int (CALLBACK *GETBUFFERA) ( DWORD dwDevice, LPSTR s, UINT maxchar );
typedef BOOL (CALLBACK *PMACCONFIGURE) ( HANDLE hwnd, DWORD dwDevice );
typedef LONG( CALLBACK *TESTDPRAM ) ( DWORD dwDevice, DPRTESTMSGPROC msgp, DPRTESTPROGRESS prgp );
typedef void  (CALLBACK *TESTDPRABORT) ( void );

typedef void (CALLBACK *DPRSETHOSTBUSYBIT) ( DWORD dwDevice, int value );
typedef int (CALLBACK *DPRGETHOSTBUSYBIT) ( DWORD dwDevice );
typedef int (CALLBACK *DPRGETPMACBUSYBIT) ( DWORD dwDevice );
typedef int (CALLBACK *DPRGETSERVOTIMER) ( DWORD dwDevice );
typedef void (CALLBACK *DPRSETMOTORS) ( DWORD dwDevice, UINT n );
typedef BOOL  (CALLBACK *DPRAVAILABLE) ( DWORD dwDevice );
typedef PVOID( CALLBACK *DPRGETPTR ) ( DWORD dwDevice );

typedef BOOL  (CALLBACK *RESTORELINKLIST) ( char *buf );


typedef BOOL  (CALLBACK *GETUSRESPONSE)(DWORD dwDevice, PCHAR buf, UINT maxchar);
typedef BOOL  (CALLBACK *GETUSRESPONSEW)(DWORD dwDevice, PCHAR buf, UINT maxchar);
typedef BOOL  (CALLBACK *USREADREADY) (DWORD dwDevice);
typedef BOOL  (CALLBACK *STOREUSRESPONSE)(DWORD dwDevice,CHAR * buf, DWORD size);

typedef BOOL  (CALLBACK *CHANGEGET)(DWORD dwDevice, PCHAR buf, UINT maxchar);
typedef BOOL  (CALLBACK *CHANGEREADREADY) (DWORD dwDevice);
typedef BOOL  (CALLBACK *CHANGESTORE)(DWORD dwDevice,CHAR * buf);

typedef DWORD (CALLBACK *MAPDEVICENUMTOENUANDLOC)(LOCATIONTYPE loc,DWORD dwEnum);
typedef DWORD (CALLBACK *MAPLOCANDENUMFROMDEVICENUM) (DWORD dwDevNum, LOCATIONTYPE * loc,DWORD * dwEnum);
typedef BOOL  (CALLBACK *DEVICESHUTDOWNPMAC)(DWORD dwDevice,BOOL bForceShutdown);


typedef void (CALLBACK *DPRROTBUFCLEAR) ( DWORD dwDevice, UINT bufnum );
typedef SHORT( CALLBACK *DPRROTBUFPUT ) ( DWORD dwDevice, int num_put, LPDWORD rotdat, int bufnum );
typedef void (CALLBACK *SETMAXDOWNLOADERRORS) ( UINT max );
typedef void (CALLBACK *ABORTDOWNLOAD) ( DWORD dwDevice );

// Turbo Specific
// Turbo Handshaking
typedef void (CALLBACK *DPRRESETDATAREADYBIT) ( DWORD dwDevice );
typedef long (CALLBACK *DPRGETDATAREADYBIT) ( DWORD dwDevice );
//typedef long (CALLBACK*  DPRUPDATEREALTIME)(DWORD dwDevice);
typedef BOOL (CALLBACK *DPRUPDATEREALTIME) ( DWORD dwDevice );
// Turbo Initialization
// Data Access
//typedef struct ssTurbo( CALLBACK *DPRMOTORSERVOSTATUSTURBO ) ( DWORD dwDevice, int mtr );

typedef  BOOL (CALLBACK *DPRREALTIME) ( DWORD dwDevice, UINT period, int on );
typedef  void (CALLBACK *DPRREALTIMESETMOTOR) ( DWORD dwDevice, long mask );
typedef  BOOL (CALLBACK *DPRBACKGROUND) ( DWORD dwDevice, int on );
typedef  BOOL (CALLBACK *DPRBACKGROUNDEX) ( DWORD dwDevice, int on, UINT period, UINT crd );
typedef  BOOL (CALLBACK *DPRBACKGROUNDVAR) ( DWORD dwDevice, int on );
typedef  BOOL (CALLBACK *DPRREALTIMEEX) ( DWORD dwDevice, long mask, UINT period, int on );

typedef int (CALLBACK *DOWNLOADA) ( DWORD dwDevice, DOWNLOADMSGPROC msgp, DOWNLOADGETPROC getp,
                            DOWNLOADPROGRESS ppgr, PCHAR filename, BOOL macro, BOOL map, BOOL log, BOOL dnld );
typedef int (CALLBACK *DOWNLOAD_SHOWPROGRESS)(BOOL bDoChecksums,BOOL bAlwaysShow,HWND hWnd,  DWORD dwDevice,
                            PCHAR filename, BOOL macro, BOOL map, BOOL log, BOOL dnld );

typedef int (CALLBACK *DOWNLOADTHREADA) ( DWORD dwDevice, DOWNLOADMSGPROC msgp, DOWNLOADGETPROC getp,
                            DOWNLOADPROGRESS ppgr, PCHAR filename, BOOL macro, BOOL map, BOOL log, BOOL dnld );
typedef int (CALLBACK *DOWNLOADW) ( DWORD dwDevice, DOWNLOADMSGPROC msgp, DOWNLOADGETPROC getp,
                            DOWNLOADPROGRESS ppgr, LPWSTR fname, BOOL macro, BOOL map, BOOL log, BOOL dnld );
typedef void (CALLBACK *DOWNLOADFILE) ( DWORD dwDevice, char *fname );
typedef BOOL (CALLBACK *COMPILEPLCC) ( DWORD dwDevice, char *plccName, char *outName );
typedef BOOL (CALLBACK *WRITEDICTIONARY) ( const char *tblName, PMACRO * root );
typedef BOOL (CALLBACK *READDICTIONARY) ( const char *tblName, PMACRO * root );
typedef int (CALLBACK *DOWNLOADFIRMWAREFILE) ( DWORD dwDevice, char *cFilename, DOWNLOADMSGPROC msgp );
typedef void (CALLBACK *ABORTDOWNLOAD) ( DWORD dwDevice );
typedef void (CALLBACK *SETMAXDOWNLOADERRORS) ( UINT max );

typedef void (CALLBACK *DPRSTATUS) ( DWORD dwDevice, UINT *comm, UINT *bg, UINT *bgv,
                UINT *rt, UINT *cp, UINT *rot );
// Lips stuff
typedef long (CALLBACK *WHYMOTORNOTMOVING) ( DWORD dwDevice, UINT motor );
typedef LPCSTR( CALLBACK *WHYMOTORNOTMOVINGSTRING ) ( long err );
typedef long (CALLBACK *WHYMOTORNOTMOVINGTURBO) ( DWORD dwDevice, UINT motor );
typedef LPCSTR (CALLBACK *WHYMOTORNOTMOVINGSTRINGTURBO) ( long err );
typedef long (CALLBACK *WHYCSNOTMOVING) ( DWORD dwDevice, UINT CS );
typedef LPCSTR (CALLBACK *WHYCSNOTMOVINGSTRING) ( long err );
typedef LPCSTR (CALLBACK *WHYCSNOTMOVINGSTRINGTURBO) ( long err );
typedef long (CALLBACK *WHYCSNOTMOVINGTURBO) ( DWORD dwDevice, UINT CS );
typedef int (CALLBACK *INITDATAGARHERING) ( DWORD dwDevice, int size );
typedef int (CALLBACK *STARTGATHER) ( DWORD dwDevice );
typedef int (CALLBACK *STOPGATHER) ( DWORD dwDevice );
typedef int (CALLBACK *IGETNUMGATHERSAMPLES) ( DWORD dwDevice );
typedef BOOL (CALLBACK *GETBITVALUE) ( char *s, int bit );
typedef long (CALLBACK *HEXLONG2) ( char *in_str, int str_ln );
typedef BOOL (CALLBACK *INBUFFEREDMODE) ( DWORD dwDevice );

// Numeric read/write functions
// Standard
typedef PVOID (CALLBACK *PMACDPRGETMEM)( DWORD dwDevice, DWORD offset, INT count, PVOID val );
typedef PVOID (CALLBACK *PMACDPRSETMEM)( DWORD dwDevice, DWORD offset, INT count, PVOID val );


typedef WORD (CALLBACK *DPRGETWORD) ( DWORD dwDevice, UINT offset );
typedef void (CALLBACK *DPRSETWORD) ( DWORD dwDevice, UINT offset, WORD val );
typedef DWORD (CALLBACK *DPRGETDWORD) ( DWORD dwDevice, UINT offset );
typedef void (CALLBACK *DPRSETDWORD) ( DWORD dwDevice, UINT offset, DWORD val );
typedef float (CALLBACK *DPRGETFLOAT) ( DWORD dwDevice, UINT offset );
typedef void (CALLBACK *DPRSETFLOAT) ( DWORD dwDevice, UINT offset, double val );
// Masking
typedef BOOL (CALLBACK *DPRDWORDBITSET) ( DWORD dwDevice, UINT offset, UINT bit );
typedef void (CALLBACK *DPRSETDWORDBIT) ( DWORD dwDevice, UINT offset, UINT bit );
typedef void (CALLBACK *DPRRESETDWORDBIT) ( DWORD dwDevice, UINT offset, UINT bit );
typedef void (CALLBACK *DPRSETDWORDMASK) ( DWORD dwDevice, UINT offset, DWORD val, BOOL onoff );
typedef DWORD (CALLBACK *DPRGETDWORDMASK) ( DWORD dwDevice, UINT offset, DWORD val );

typedef double (CALLBACK *DPRFLOAT) ( long d[], double scale );
typedef double (CALLBACK *DPRLFIXED) ( long d[], double scale );
typedef long (CALLBACK *DPRADDRESSRANGE) ( DWORD dwDevice, BOOL Upper, BOOL PC_offsets );


typedef double (CALLBACK *DPRCOMMANDED) ( DWORD dwDevice, int coord, char axchar );
typedef double (CALLBACK *DPRVELOCITY) ( DWORD dwDevice, int mtr, double units );
typedef double (CALLBACK *DPRVECTORVELOCITY) ( DWORD dwDevice, int num, int mtr[], double units[] );
typedef BOOL (CALLBACK *DPRSETBACKGROUND) ( DWORD dwDevice );

// Functions pertaining to coord systems
typedef long (CALLBACK *DPRPE) ( DWORD dwDevice, int cs );
typedef BOOL (CALLBACK *DPRROTBUFFULL) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRSYSINPOSITION) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRSYSWARNFERROR) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRSYSFATALFERROR) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRSYSRUNTIMEERROR) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRSYSCIRCLERADERROR) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRSYSAMPFAULTERROR) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRPROGRUNNING) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRPROGSTEPPING) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRPROGCONTMOTION) ( DWORD dwDevice, int crd );
typedef BOOL (CALLBACK *DPRPROGCONTREQUEST) ( DWORD dwDevice, int crd );
typedef int (CALLBACK *DPRPROGREMAINING) ( DWORD dwDevice, int crd );

// Functions pertaining to individual motors
// Background-Functions pertaining to individual motors
typedef BOOL (CALLBACK *DPRAMPENABLED) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRWARNFERROR) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRFATALFERROR) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRAMPFAULT) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRONPOSITIONLIMIT) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRHOMECOMPLETE) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRINPOSITION) ( DWORD dwDevice, int mtr );
typedef double (CALLBACK *DPRGETTARGETPOS) ( DWORD dwDevice, int motor, double posscale );
typedef double (CALLBACK *DPRGETBIASPOS) ( DWORD dwDevice, int motor, double posscale );
typedef long (CALLBACK *DPRTIMEREMINMOVE) ( DWORD dwDevice, int cs );
typedef long (CALLBACK *DPRTIMEREMINTATS) ( DWORD dwDevice, int cs );


// Logical query functions
typedef PROGRAM (CALLBACK *DPRGETPROGRAMMODE) ( DWORD dwDevice, int csn );
typedef MOTIONMODE (CALLBACK *DPRGETMOTIONMODE) ( DWORD dwDevice, int csn );

////////////////////////////////////////////////////////////////////////////
// DPR Control Panel functions
////////////////////////////////////////////////////////////////////////////
typedef BOOL (CALLBACK *DPRCONTROLPANEL) ( DWORD dwDevice, long on );
typedef void (CALLBACK *DPRSETJOGPOSBIT) ( DWORD dwDevice, long motor, long onoff );
typedef long (CALLBACK *DPRGETJOGPOSBIT) ( DWORD dwDevice, long motor );
typedef void (CALLBACK *DPRSETJOGNEGBIT) ( DWORD dwDevice, long motor, long onoff );
typedef long (CALLBACK *DPRGETJOGNEGBIT) ( DWORD dwDevice, long motor );
typedef void (CALLBACK *DPRSETJOGRETURNBIT) ( DWORD dwDevice, long motor, long onoff );
typedef long (CALLBACK *DPRGETJOGRETURNBIT) ( DWORD dwDevice, long motor );
typedef void (CALLBACK *DPRSETRUNBIT) ( DWORD dwDevice, long cs, long onoff );
typedef long (CALLBACK *DPRGETRUNBIT) ( DWORD dwDevice, long cs );
typedef void (CALLBACK *DPRSETSTOPBIT) ( DWORD dwDevice, long cs, long onoff );
typedef long (CALLBACK *DPRGETSTOPBIT) ( DWORD dwDevice, long cs );
typedef void (CALLBACK *DPRSETHOMEBIT) ( DWORD dwDevice, long cs, long onoff );
typedef long (CALLBACK *DPRGETHOMEBIT) ( DWORD dwDevice, long cs );
typedef void (CALLBACK *DPRSETHOLDBIT) ( DWORD dwDevice, long cs, long onoff );
typedef long (CALLBACK *DPRGETHOLDBIT) ( DWORD dwDevice, long cs );
typedef long (CALLBACK *DPRGETSTEPBIT) ( DWORD dwDevice, long cs );
typedef void (CALLBACK *DPRSETSTEPBIT) ( DWORD dwDevice, long cs, long onoff );
typedef long (CALLBACK *DPRGETREQUESTBIT) ( DWORD dwDevice, long mtrcrd );
typedef void (CALLBACK *DPRSETREQUESTBIT) ( DWORD dwDevice, long mtrcrd, long onoff );
typedef long (CALLBACK *DPRGETFOENABLEBIT) ( DWORD dwDevice, long cs );
typedef void (CALLBACK *DPRSETFOENABLEBIT) ( DWORD dwDevice, long cs, long on_off );
typedef void (CALLBACK *DPRSETFOVALUE) ( DWORD dwDevice, long cs, long value );
typedef long (CALLBACK *DPRGETFOVALUE) ( DWORD dwDevice, long cs );


////////////////////////////////////////////////////////////////////////////
// DPR Real Time Data Buffer functions
////////////////////////////////////////////////////////////////////////////
typedef void (CALLBACK *DPRSETHOSTBUSYBIT) ( DWORD dwDevice, int value );
typedef int (CALLBACK *DPRGETHOSTBUSYBIT) ( DWORD dwDevice );
typedef int (CALLBACK *DPRGETPMACBUSYBIT) ( DWORD dwDevice );
typedef int (CALLBACK *DPRGETSERVOTIMER) ( DWORD dwDevice );
typedef void (CALLBACK *DPRSETMOTORS) ( DWORD dwDevice, UINT n );

typedef double (CALLBACK *DPRGETCOMMANDEDPOS) ( DWORD dwDevice, int mtr, double units );
typedef double (CALLBACK *DPRPOSITION) ( DWORD dwDevice, int i, double units );
typedef double (CALLBACK *DPRNETACTUALPOSITION) ( DWORD dwDevice, int mtr, double units );
typedef double (CALLBACK *DPRFOLLOWERROR) ( DWORD dwDevice, int mtr, double units );
typedef double (CALLBACK *DPRGETVEL) ( DWORD dwDevice, int mtr, double units );
typedef void (CALLBACK *DPRGETMASTERPOS) ( DWORD dwDevice, int mtr, double units, double *the_double );
typedef void (CALLBACK *DPRGETCOMPENSATIONPOS) ( DWORD dwDevice, int mtr, double units, double *the_double );

typedef DWORD (CALLBACK *DPRGETPREVDAC) ( DWORD dwDevice, int mtr );
typedef DWORD (CALLBACK *DPRGETMOVETIME) ( DWORD dwDevice, int mtr );

//Gather Time Buffer-Functions pertaining to individual motors
typedef struct ss( CALLBACK *DPRMOTORSERVOSTATUS ) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRDATABLOCK) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRPHASEDMOTOR) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRMOTORENABLED) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRHANDWHEELENABLED) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPROPENLOOP) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRONNEGATIVELIMIT) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *DPRONPOSITIVELIMIT) ( DWORD dwDevice, int mtr );
typedef void (CALLBACK *DPRSETJOGRETURN) ( DWORD dwDevice, int mtr );

// Logical query functions
typedef MOTION (CALLBACK *DPRGETMOTORMOTION) ( DWORD dwDevice, int mtr );

// Functions pertaining to coord systems
typedef BOOL (CALLBACK *DPRMOTIONBUFOPEN) ( DWORD dwDevice );
typedef BOOL (CALLBACK *DPRROTBUFOPEN) ( DWORD dwDevice );
typedef double (CALLBACK *DPRGETFEEDRATEMODE) ( DWORD dwDevice, int csn, BOOL *mode );

// Function pertaining to global
typedef BOOL (CALLBACK *DPRSYSSERVOERROR) ( DWORD dwDevice );
typedef BOOL (CALLBACK *DPRSYSREENTRYERROR) ( DWORD dwDevice );
typedef BOOL (CALLBACK *DPRSYSMEMCHECKSUMERROR) ( DWORD dwDevice );
typedef BOOL (CALLBACK *DPRSYSPROMCHECKSUMERROR) ( DWORD dwDevice );
typedef void (CALLBACK *DPRGETGLOBALSTATUS ) ( DWORD dwDevice,void *);

typedef BOOL (CALLBACK *INTRSETUPMASK) (DWORD dwDevice, ULONG ulMask);
typedef BOOL (CALLBACK *INTRINIT) ( DWORD dwDevice, DWORD dwCallback, DWORD dwFlags,
                             DWORD dwUser, ULONG mask );
typedef BOOL (CALLBACK *INTRTERMINATE) ( DWORD dwDevice );
typedef BOOL (CALLBACK *INTRWNDMSGINIT) ( DWORD dwDevice, HWND hWnd, UINT msg, ULONG ulMask );
typedef DWORD (CALLBACK *INTRTHREADINIT) ( DWORD *dwDev );
typedef BOOL (CALLBACK *INTRFUNCCALLINIT) ( DWORD dwDevice, PMACINTRPROC pFunc, DWORD msg, ULONG ulMask );

typedef VOID( CALLBACK *INTRCALLBACK ) ( DWORD dwDevice );
typedef VOID (CALLBACK *INTRQUEUE) ( DWORD dwDevice, DWORD dwEvent );
typedef VOID (CALLBACK *INTRCOMPLETE) ( DWORD dwDevice, DWORD dwEvent );
typedef BOOL (CALLBACK *INTRPROCESSFUNCTION) ( DWORD dwDevice, MCFUNC Func, DWORD Param,
                   LPDWORD pResult );

/*****************************************************************************/

//**************************************
// DPR Binary rotary buffer functions
// Initialization/shutdown
typedef SHORT (CALLBACK *DPRROTBUFINIT) ( DWORD dwDevice, USHORT bufnum );
typedef SHORT (CALLBACK *DPRROTBUFREMOVE) ( DWORD dwDevice, USHORT bufnum );
typedef SHORT (CALLBACK *DPRROTBUFCHANGE) ( DWORD dwDevice, USHORT bufnum, USHORT new_size );
typedef void (CALLBACK *DPRROTBUFCLR) ( DWORD dwDevice, USHORT bufnum );
typedef SHORT (CALLBACK *DPRROTBUF) ( DWORD dwDevice, BOOL on );
typedef int (CALLBACK *DPRBUFLAST) ( DWORD dwDevice );

// Transfer functions
typedef SHORT (CALLBACK *DPRASCIISTRTOROTA) ( DWORD dwDevice, PCHAR inpstr, USHORT bufnum );
typedef SHORT (CALLBACK *DPRASCIISTRTOROTEX) ( DWORD dwDevice, PCHAR inpstr, USHORT bufnum, BOOL bSendRemaining );
typedef SHORT (CALLBACK *DPRSENDBINARYCOMMANDA) ( DWORD dwDevice, PCHAR inpstr, USHORT bufnum );
typedef SHORT (CALLBACK *DPRASCIISTRTOBINARYFILEA) ( PCHAR inpstr, FILE * outfile );

typedef SHORT (CALLBACK *DPRASCIISTRTOROTW) ( DWORD dwDevice, LPWSTR inpstr, USHORT bufnum );
typedef SHORT (CALLBACK *DPRSSNDBINARYCOMMANDW) ( DWORD dwDevice, LPWSTR inpstr, USHORT bufnum );
typedef SHORT (CALLBACK *DPRASCIISTRTOBINARYFILEW) ( LPWSTR inpstr, FILE * outfile );


typedef SHORT (CALLBACK *DPRASCIIFILETOROT) ( DWORD dwDevice, FILE * inpfile, USHORT bufnum );
typedef SHORT (CALLBACK *DPRBINARYFILETOROT) ( DWORD dwDevice, FILE * inpfile, USHORT bufnum );
typedef SHORT (CALLBACK *DPRASCIITOBINARYFILE) ( FILE * inpfile, FILE * outfile );
typedef SHORT (CALLBACK *DPRBINARYTOROT) ( DWORD dwDevice, WORD *inpbinptr, WORD **outbinptr, WORD bufnum );

// Variable Background Buffer Functions
// Initialization
typedef long (CALLBACK *DPRVARBUFINIT) ( DWORD dwDevice, long new_num_entries, PLONG addrarray );
typedef long (CALLBACK *DPRVARBUFINITEX) ( DWORD dwDevice, long new_num_entries, PLONG addrarray, PUINT addrtype );
typedef BOOL (CALLBACK *DPRVARBUFREMOVE) ( DWORD dwDevice, long h );
typedef BOOL (CALLBACK *DPRVARBUFCHANGE) ( DWORD dwDevice, long handle, long new_num_entries, long *addrarray );
typedef BOOL (CALLBACK *DPRVARBUFCHANGEEX) ( DWORD dwDevice, long handle, long new_num_entries, long *addrarray, PUINT addrtype );
typedef BOOL (CALLBACK *DPRVARBUFREAD) ( DWORD dwDevice, long h, long entry_num, PLONG long_2 );
typedef BOOL (CALLBACK *DPRVARBUFREADEX) ( DWORD dwDevice, long h, PLONG long_x );
typedef long (CALLBACK *DPRGETVBGADDRESS) ( DWORD dwDevice, long h, long entry_num );

typedef long (CALLBACK *DPRGETVBGNUMENTRIES) ( DWORD dwDevice, long h );
typedef long (CALLBACK *DPRGETVBGDATAOFFSET) ( DWORD dwDevice, long h );
typedef long (CALLBACK *DPRGETVBGADDROFFSET) ( DWORD dwDevice, long h );

// Both
typedef UINT (CALLBACK *DPRGETVBGSERVOTIMER) ( DWORD dwDevice );
typedef UINT (CALLBACK *DPRGETVBGSTARTADDR) ( DWORD dwDevice );
typedef int (CALLBACK *DPRGETVBGTOTALENTRIES) ( DWORD dwDevice );

typedef int (CALLBACK *DPRWRITEBUFFER) ( DWORD dwDevice, int num_entries,
                struct VBGWFormat * the_data );
typedef int (CALLBACK *DPRWRITEBUFFEREX) ( DWORD dwDevice, int num_entries,
                struct VBGWFormat * the_data, long *addresses, UINT *types );
typedef void (CALLBACK *DOWNLOADGANTRYAUTOTUNINGPLC) ( DWORD dwDevice,
          DWORD dwMotor1, DWORD dwMotor2, BOOL current_loop_used, BOOL bypass_jog, float maxtravel, long lTime );
typedef void (CALLBACK *DOWNLOADGANTRYSTEPPLC) ( DWORD dwDevice, DWORD dwMotor1, DWORD dwMotor2 );
typedef void (CALLBACK *DOWNLOADGANTRYPARABOLICPLC) ( DWORD dwDevice, DWORD dwMotor1, DWORD dwMotor2 );

////////////////////////////////////////////////////////////////////////////
// Data Gather functions
////////////////////////////////////////////////////////////////////////////
typedef UINT (CALLBACK *GETGATHERPERIOD) ( DWORD dwDevice );
typedef double (CALLBACK *GETGATHERSAMPLETIME) ( DWORD dwDevice );
typedef UINT (CALLBACK *GETNUMGATHERSOURCES) ( DWORD dwDevice );
typedef UINT (CALLBACK *GETNUMGATHERSAMPLES) ( DWORD dwDevice );
typedef UINT (CALLBACK *SETGATHERPERIOD) ( DWORD dwDevice, UINT msec );
typedef double (CALLBACK *SETGATHERSAMPLETIME) ( DWORD dwDevice, double msec );
typedef BOOL (CALLBACK *SETGATHERENABLE) ( DWORD dwDevice, UINT num, BOOL ena );
typedef BOOL (CALLBACK *GETGATHERENABLE) ( DWORD dwDevice, UINT num );
typedef BOOL (CALLBACK *SETCURRENTGATHER) ( DWORD dwDevice, UINT mask, BOOL ena );
typedef BOOL (CALLBACK *SETGATHER) ( DWORD dwDevice, UINT num, LPSTR str, BOOL ena );
typedef BOOL (CALLBACK *SETQUICKGATHER) ( DWORD dwDevice, UINT mask, BOOL ena );
typedef BOOL (CALLBACK *SETQUICKGATHEREX) ( DWORD dwDevice, PWTG_EX mask, BOOL ena );
typedef BOOL (CALLBACK *SETQGWITHDIRECTCUR) ( DWORD dwDevice, UINT mask, BOOL ena );
typedef BOOL (CALLBACK *SETQGWITHDIRECTCUREX) ( DWORD dwDevice, PWTG_EX mask, BOOL ena );
typedef BOOL (CALLBACK *GETGATHER) ( DWORD dwDevice, UINT num, LPSTR str, UINT maxchar );
typedef void (CALLBACK *CLEARGATHER) ( DWORD dwDevice );
typedef BOOL (CALLBACK *INITGATHER) ( DWORD dwDevice, UINT size, double msec );
typedef void (CALLBACK *CLEARGATHERDATA) ( DWORD dwDevice );
typedef double * ( CALLBACK *COLLECTGATHERDATA ) ( DWORD dwDevice, PUINT sources, PUINT samples );
typedef BOOL (CALLBACK *GETGATHERSAMPLES) ( DWORD dwDevice, UINT source, PUINT sample, double *p, UINT max );
typedef BOOL (CALLBACK *GETGATHERPOINT) ( DWORD dwDevice, UINT source, UINT sample, double *p );

typedef int (CALLBACK *STARTGATHER) ( DWORD dwDevice );
typedef int (CALLBACK *STOPGATHER) ( DWORD dwDevice );

// Real time
typedef BOOL (CALLBACK *INITRTGATHER) ( DWORD dwDevice );
typedef void (CALLBACK *CLEARRTGATHER) ( DWORD dwDevice );
typedef BOOL (CALLBACK *ADDRTGATHER) ( DWORD dwDevice, ULONG val );
typedef double * ( CALLBACK *COLLECTRTGATHERDATA ) ( DWORD dwDevice, PUINT sources );

typedef DWORD (CALLBACK *ULTRALIGHTANALYZE)( DWORD dwDevice,ULTRALIGHT_INFO * ui);
typedef BOOL (CALLBACK *MACROUPLOADCONFIG) ( DWORD dwDevice, DOWNLOADPROGRESS prgp, char *fname );
typedef long (CALLBACK *MACROGETIVARIABLELONG) ( DWORD dwDevice, DWORD node, UINT num, long def );
typedef BOOL (CALLBACK *MACROSETIVARIABLESTRING)( DWORD dwDevice, DWORD node, UINT num, char *szMIVarValue);
typedef PCHAR (CALLBACK *MACROGETIVARIABLESTRING)( DWORD dwDevice, DWORD node, UINT num, PCHAR def );

typedef BOOL (CALLBACK *AMPENABLED) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *AMPFAULT) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *ONNEGATIVELIMIT) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *ONPOSITIVELIMIT) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *FATALFERROR) ( DWORD dwDevice, int mtr );
typedef double (CALLBACK *FOLLOWERROR) ( DWORD dwDevice, int mtr, double units );
typedef BOOL (CALLBACK *PHASEDMOTOR) ( DWORD dwDevice, int mtr );
typedef BOOL (CALLBACK *GETMOTORSTATUS) (DWORD dwDevice,UINT mtr,DWORD *status);

typedef BOOL   (CALLBACK* VMETESTOPENPMACDEVICE)(DWORD dwDevice,BOOL bMailboxOnly);
typedef BOOL   (CALLBACK* VMETESTCLOSEPMACDEVICE)(DWORD dwDevice);
typedef BOOL   (CALLBACK* VMESETUP)(DWORD SERIALDev, DWORD VMEDev);

typedef BOOL  (CALLBACK* PMACSOCKISMULTIHOST) (DWORD dwDevice);
typedef BOOL  (CALLBACK* PMACSOCKISINVALID) (DWORD dwDevice);

//2013.12.19 added by DTK
typedef struct gs (CALLBACK *DPRGLOBALSTATUS) ( DWORD dwDevice );
typedef struct ssTURBO (CALLBACK *DPRMOTORSERVOSTATUSTURBO) ( DWORD dwDevice, int mtr );
typedef struct msTURBO (CALLBACK *DPRMOTORSTATUSTURBO) ( DWORD dwDevice, int mtr );
typedef struct realm (CALLBACK *DPRMOTORREALTIMESTRUCT) ( DWORD dwDevice, int mtr );
typedef struct realmTURBO (CALLBACK *DPRMOTORREALTIMESTRUCTTURBO) ( DWORD dwDevice, int mtr );
//end of 2013.12.19


///////////////////////////////////////////////////////////////////////////
// Functions


  #ifdef NO_EXTERN
    #define EXTRN
  #else
    #define EXTRN extern
  #endif

  #ifdef __cplusplus
extern "C" {
  #endif

  HINSTANCE OpenRuntimeLink();
  void      CloseRuntimeLink();
  void DoNCFunctions( BOOL yesno );

  // Comm functions
  EXTRN PMACDPRPCITEST DeviceDPRPCITest;
  EXTRN DOCHECKSUMS DeviceDoChecksums;
  EXTRN READREADY DeviceReadReady;
  EXTRN FLUSH DeviceFlush;
  EXTRN OPENPMACDEVICE DeviceOpen;
  EXTRN SELECTPMACDEVICE DeviceSelect;
  EXTRN NUMBEROFDEVICES DeviceNumberOfDevices;
  EXTRN CLOSEPMACDEVICE DeviceClose;
  EXTRN PMACSETLANGUAGE DeviceSetLanguage;
  EXTRN PMACSETWINDOW   DeviceSetWindow;
  EXTRN PMACLMH DeviceLMH;
  EXTRN SETLANGUAGEMODULE DeviceSetLanguageModule;
  EXTRN CONFIGURE DeviceConfigure;
  EXTRN SENDLINEA DeviceSendLine;
  EXTRN SENDLINEW DeviceSendLineW;							// 2016.08.03. 추가
  EXTRN GETLINEA DeviceGetLine;
  EXTRN GETLINEW DeviceGetLineW;							// 2016.08.03. 추가
  EXTRN GETLINEEXA DeviceGetLineEx;
  EXTRN GETLINEEXW DeviceGetLineExW;						// 2016.08.03. 추가
  EXTRN WAITGETLINEA DeviceWaitGetLine;
  EXTRN SENDCHARA DeviceSendChar;
  EXTRN GETRESPONSEA DeviceGetResponse;
  EXTRN GETRESPONSEW DeviceGetResponseW;					// 2016.08.03. 추가
  EXTRN GETCONTROLRESPONSEA DeviceGetControlResponse;
  EXTRN GETCONTROLRESPONSEW DeviceGetControlResponseW;		// 2016.08.03. 추가
  EXTRN GETRESPONSEEXA DeviceGetResponseEx;
  EXTRN GETRESPONSEEXW DeviceGetResponseExW;				// 2016.08.03. 추가
  EXTRN GETCONTROLRESPONSEEXA DeviceGetControlResponseEx;
  EXTRN GETCONTROLRESPONSEEXW DeviceGetControlResponseExW;	// 2016.08.03. 추가
  EXTRN GETASCIICOMM DeviceGetAsciiComm;
  EXTRN SETASCIICOMM DeviceSetAsciiComm;
  EXTRN GETLOCATION DeviceGetPmacLocation;
  EXTRN GETENUMERATION DeviceGetPmacEnumeration;
  EXTRN GETERROR DeviceGetError;
  EXTRN DOWNLOADA DeviceDownload;
  EXTRN DOWNLOAD_SHOWPROGRESS DeviceDownloadProgress;
  EXTRN DOWNLOADTHREADA DeviceDownloadThread;
  EXTRN INTRSETUPMASK DeviceINTRSetupMask;
  EXTRN INTRTERMINATE DeviceINTRTerminate;
  EXTRN INTRWNDMSGINIT  DeviceINTRWndMsgInit;
  EXTRN INTRFUNCCALLINIT DeviceINTRFuncCallInit;
  EXTRN LOCKPMAC   DeviceLock;
  EXTRN RELEASEPMAC DeviceRelease;
  EXTRN SETCRITICALIVARS DeviceSetCriticalIVars;
  EXTRN GETVARIABLESTRA DeviceGetVariableStr;
  EXTRN GETVARIABLELONG DeviceGetVariableLong;
  EXTRN GETVARIABLEDOUBLE DeviceGetVariableDouble;
  EXTRN GETIVARIABLELONG DeviceGetIVariableLong;
  EXTRN SETIVARIABLELONG DeviceSetIVariableLong;
  EXTRN GETIVARIABLEDOUBLE DeviceGetIVariableDouble;
  EXTRN SETIVARIABLEDOUBLE DeviceSetIVariableDouble;
  //++ Added
  EXTRN SETVARIABLE DeviceSetVariable;
  EXTRN SETVARIABLELONG DeviceSetVariableLong;
  EXTRN SETVARIABLEDOUBLE DeviceSetVariableDouble;
  // by DTK
  EXTRN GETUSERHANDLE DeviceGetUserHandle;
  EXTRN SZLOADSTRINGA DeviceLoadString;
  EXTRN INBOOTSTRAP DeviceInBootStrapMode;
  EXTRN GETBUFFERA  DeviceGetBuffer;
  EXTRN PMACCONFIGURE DevicePmacConfigure;
  EXTRN TESTDPRAM DeviceTestDPRAM;
  EXTRN GETPMACTYPE DeviceGetPmacType;
  EXTRN TESTDPRABORT DeviceTestDPRAMAbort;
  
  EXTRN SERDOCHECKSUMS DeviceSERDoChecksums;
  EXTRN SERCHECKSENDLINEA DeviceSERCheckSendLineA;
  EXTRN SERCHECKGETLINEA DeviceSERCheckGetLineA;
  EXTRN SERCHECKRESPONSEA DeviceSERCheckResponseA;
  EXTRN SERCHECKALLWENTWELL  DeviceSERCheckAllWentWell;

  EXTRN PMACISTURBO  DevicePmacIsTurbo;


  // Binary Rotary Buffer
  EXTRN DPRROTBUFINIT DeviceDPRRotBufInit;
  EXTRN DPRROTBUF DeviceDPRRotBuf;
  EXTRN DPRBUFLAST  DeviceDPRBufLast;
  EXTRN DPRROTBUFREMOVE DeviceDPRRotBufRemove;
  EXTRN DPRROTBUFCHANGE DeviceDPRRotBufChange;
  EXTRN DPRASCIISTRTOROTA DeviceDPRAsciiStrToRot;
  EXTRN DPRASCIISTRTOROTEX DeviceDPRAsciiStrToRotEx;
  EXTRN DPRREALTIME DeviceDPRRealTime;
  EXTRN DPRSETHOSTBUSYBIT DeviceDPRSetHostBusyBit;
  EXTRN DPRGETHOSTBUSYBIT DeviceDPRGetHostBusyBit;
  EXTRN DPRGETPMACBUSYBIT DeviceDPRGetPMACBusyBit;
  EXTRN DPRGETSERVOTIMER DeviceDPRGetServoTimer;
  EXTRN DPRSETMOTORS DeviceDPRSetMotors;
  EXTRN DPRGETCOMMANDEDPOS DeviceDPRGetCommandedPos;
  EXTRN DPRPOSITION DeviceDPRPosition;
  EXTRN DPRNETACTUALPOSITION DeviceDPRNetActualPosition;
  EXTRN DPRFOLLOWERROR DeviceDPRFollowError;
  EXTRN DPRGETVEL DeviceDPRGetVel;
  EXTRN DPRGETMASTERPOS DeviceDPRGetMasterPos;
  EXTRN DPRGETCOMPENSATIONPOS DeviceDPRGetCompensationPos;
  EXTRN DPRGETPREVDAC DeviceDPRGetPrevDAC;
  EXTRN DPRGETMOVETIME DeviceDPRGetMoveTime;
  EXTRN DPRAVAILABLE DeviceDPRAvailable;
//  EXTRN DPRGETPTR DeviceDPRGetPtr;

  EXTRN RESTORELINKLIST  DeviceRestoreLinkList;

  EXTRN GETUSRESPONSE DeviceGetUSResponse;
  EXTRN USREADREADY DeviceUSReadReady;
  EXTRN STOREUSRESPONSE DeviceStoreUSResponse;

  EXTRN CHANGEGET DeviceChangeGet;
  EXTRN CHANGEREADREADY DeviceChangeReadReady;
  EXTRN CHANGESTORE DeviceChangeStore;
  EXTRN MAPDEVICENUMTOENUANDLOC DeviceMapDeviceNumToEnumAndLocation;
  EXTRN MAPLOCANDENUMFROMDEVICENUM DeviceMapLocAndEnumFromDeviceNum;
  EXTRN DEVICESHUTDOWNPMAC DeviceShutdownPmac;


  EXTRN DPRROTBUFCLEAR  DeviceDPRRotBufClear;
  EXTRN DPRROTBUFPUT DeviceDPRRotBufPut;

  EXTRN SETMAXDOWNLOADERRORS DeviceSetMaxDownloadErrors;
  EXTRN ABORTDOWNLOAD DeviceAbortDownload;

  // Turbo Specific
//  EXTRN DPRRESETDATAREADYBIT DeviceDPRResetDataReadyBit;
//  EXTRN DPRGETDATAREADYBIT DeviceDPRGetDataReadyBit;
  EXTRN DPRUPDATEREALTIME DeviceDPRUpdateRealtime;
  //EXTRN DPRREALTIMETURBO DeviceDPRRealTimeTurbo;
  //EXTRN DPRREALTIMESETMOTORMASK DeviceDPRRealTimeSetMotorMask;
  EXTRN DPRREALTIMESETMOTOR DeviceDPRRealTimeSetMotor;
  EXTRN DPRREALTIMEEX  DeviceDPRRealTimeEx;
//  EXTRN DPRMOTORSERVOSTATUSTURBO DeviceDPRMotorServoStatusTurbo;
  EXTRN DPRGETTARGETPOS DeviceDPRGetTargetPos;
  EXTRN DPRGETBIASPOS DeviceDPRGetBiasPos;
  EXTRN DPRPE DeviceDPRPe;
  EXTRN DPRTIMEREMINMOVE DeviceDPRTimeRemainingInMove;
  EXTRN DPRTIMEREMINTATS DeviceTimeRemInTATS;
  EXTRN DPRPROGREMAINING DeviceDPRProgRemaining;
  EXTRN DPRCOMMANDED DeviceDPRCommanded;
  // background buffer commands
  EXTRN DPRVARBUFINIT DeviceDPRVarBufInit;
  EXTRN DPRVARBUFINITEX DeviceDPRVarBufInitEx;
  EXTRN DPRVARBUFREMOVE DeviceDPRVarBufRemove;
  EXTRN DPRVARBUFCHANGE DeviceDPRVarBufChange;
  EXTRN DPRVARBUFCHANGEEX DeviceDPRVarBufChangeEx;
  EXTRN DPRBACKGROUND   DeviceDPRBackground;
  EXTRN DPRBACKGROUNDEX DeviceDPRBackgroundEx;
  EXTRN DPRBACKGROUNDVAR DeviceDPRBackGroundVar;
  EXTRN DPRWRITEBUFFER  DeviceDPRWriteBuffer;
  EXTRN DPRWRITEBUFFEREX  DeviceDPRWriteBufferEx;
  EXTRN DPRGETVBGSERVOTIMER DeviceDPRGetVBGServoTimer;
  EXTRN DPRVELOCITY DeviceDPRVelocity;
  EXTRN DPRVARBUFREAD DeviceDPRVarBufRead;
  EXTRN DPRVARBUFREADEX DeviceDPRVarBufReadEx;
  EXTRN GETPLCSTATUS DeviceGetPlcStatus;
  EXTRN ISANYPLCRUNNING DeviceIsAnyPLCRunning;
  EXTRN GETROMDATEA DeviceGetRomDate;
  EXTRN GETROMVERSIONA DeviceGetRomVersion;
  EXTRN GETERRORSTRA DeviceGetErrorStr;
  EXTRN MULTIDOWNLOADA DeviceMultiDownload;
  EXTRN ADDDOWNLOADFILEA DeviceAddDownloadFile;
  EXTRN REMOVEDOWNLOADFILEA DeviceRemoveDownloadFile;
  EXTRN RENUMBERFILESA DeviceRenumberFiles;
  EXTRN DOWNLOADFIRMWAREFILE DeviceLoadFirmwareFile;
  EXTRN SERGETPORT DeviceSERGetPort;
//  EXTRN SERSETPORT DeviceSERSetPort;
  EXTRN SERGETBAUDRATE DeviceSERGetBaudrate;
//  EXTRN SERSETBAUDRATE DeviceSERSetBaudrate;
  EXTRN DPRGETMOTORMOTION DeviceDPRGetMotorMotion;
  EXTRN DPRGETPROGRAMMODE DeviceDPRGetProgramMode;
  EXTRN DPRSYSINPOSITION DeviceDPRSysInposition;
  EXTRN DPRROTBUFOPEN DeviceDPRRotBufOpen;
  EXTRN DPRGETFEEDRATEMODE DeviceDPRGetFeedRateMode;
  EXTRN WHYMOTORNOTMOVING DeviceWhyMotorNotMoving;
  EXTRN WHYMOTORNOTMOVINGSTRING DeviceWhyMotorNotMovingString;
  EXTRN WHYCSNOTMOVINGSTRING DeviceWhyCsNotMovingString;
  EXTRN WHYCSNOTMOVING  DeviceWhyCsNotMoving;
  EXTRN WHYMOTORNOTMOVINGTURBO DeviceWhyMotorNotMovingTURBO;
  EXTRN WHYMOTORNOTMOVINGSTRINGTURBO DeviceWhyMotorNotMovingStringTURBO;
  EXTRN WHYCSNOTMOVINGSTRINGTURBO DeviceWhyCsNotMovingStringTURBO;
  EXTRN WHYCSNOTMOVINGTURBO  DeviceWhyCsNotMovingTURBO;

  EXTRN GETBITVALUE DeviceGetBitValue;
  EXTRN HEXLONG2 DeviceHexLong2;
  EXTRN INBUFFEREDMODE DeviceInBufferedMode;

  // Numeric read/write functions
  // Standard
  EXTRN DPRGETWORD DeviceDPRGetWord;
  EXTRN DPRSETWORD DeviceDPRSetWord;
  EXTRN DPRGETDWORD DeviceDPRGetDWord;
  EXTRN DPRSETDWORD DeviceDPRSetDWord;
  EXTRN DPRGETFLOAT DeviceDPRGetFloat;
  EXTRN DPRSETFLOAT DeviceDPRSetFloat;
  EXTRN PMACDPRSETMEM DeviceDPRSetMem;
  EXTRN PMACDPRGETMEM DeviceDPRGetMem;

  // Masking
  EXTRN DPRDWORDBITSET DeviceDPRDWordBitSet;
  EXTRN DPRSETDWORDBIT DeviceDPRSetDWordBit;
  EXTRN DPRRESETDWORDBIT DeviceDPRResetDWordBit;
  EXTRN DPRSETDWORDMASK DeviceDPRSetDWordMask;
  EXTRN DPRGETDWORDMASK DeviceDPRGetDWordMask;
  EXTRN DPRFLOAT DeviceDPRFloat;
  EXTRN DPRLFIXED DeviceDPRLFixed;
  EXTRN DPRADDRESSRANGE DeviceDPRAddressRange;

  // Control Panel
  EXTRN DPRCONTROLPANEL DeviceDPRControlPanel;
  EXTRN DPRSETJOGPOSBIT DeviceDPRSetJogPosBit;
  EXTRN DPRGETJOGPOSBIT DeviceDPRGetJogPosBit;
  EXTRN DPRSETJOGNEGBIT DeviceDPRSetJogNegBit;
  EXTRN DPRGETJOGNEGBIT DeviceDPRGetJogNegBit;
  EXTRN DPRSETJOGRETURNBIT DeviceDPRSetJogReturnBit;
  EXTRN DPRGETJOGRETURNBIT DeviceDPRGetJogReturnBit;
  EXTRN DPRSETRUNBIT DeviceDPRSetRunBit;
  EXTRN DPRGETRUNBIT DeviceDPRGetRunBit;
  EXTRN DPRSETSTOPBIT DeviceDPRSetStopBit;
  EXTRN DPRGETSTOPBIT DeviceDPRGetStopBit;
  EXTRN DPRSETHOMEBIT DeviceDPRSetHomeBit;
  EXTRN DPRGETHOMEBIT DeviceDPRGetHomeBit;
  EXTRN DPRSETHOLDBIT DeviceDPRSetHoldBit;
  EXTRN DPRGETHOLDBIT DeviceDPRGetHoldBit;
  EXTRN DPRGETSTEPBIT DeviceDPRGetStepBit;
  EXTRN DPRSETSTEPBIT DeviceDPRSetStepBit;
  EXTRN DPRGETREQUESTBIT DeviceDPRGetRequestBit;
  EXTRN DPRSETREQUESTBIT DeviceDPRSetRequestBit;
  EXTRN DPRGETFOENABLEBIT DeviceDPRGetFOEnableBit;
  EXTRN DPRSETFOENABLEBIT DeviceDPRSetFOEnableBit;
  EXTRN DPRSETFOVALUE DeviceDPRSetFOValue;
  EXTRN DPRGETFOVALUE DeviceDPRGetFOValue;
  // Data Gathering
  EXTRN GETGATHERSAMPLES DeviceGetGatherSamples;
  EXTRN GETGATHERPERIOD DeviceGetGatherPeriod;
  EXTRN GETGATHERSAMPLETIME DeviceGetGatherSampleTime;
  EXTRN SETGATHERSAMPLETIME DeviceSetGatherSampleTime;
  EXTRN GETNUMGATHERSOURCES DeviceGetNumGatherSources;
  EXTRN GETNUMGATHERSAMPLES DeviceGetNumGatherSamples;
  EXTRN SETGATHERPERIOD DeviceSetGatherPeriod;
  EXTRN SETGATHERENABLE DeviceSetGatherEnable;
  EXTRN GETGATHERENABLE DeviceGetGatherEnable;
  EXTRN SETCURRENTGATHER DeviceSetCurrentGather;
  EXTRN SETGATHER DeviceSetGather;
  EXTRN SETQUICKGATHER DeviceSetQuickGather;
  EXTRN SETQUICKGATHEREX DeviceSetQuickGatherEx;
  EXTRN SETQGWITHDIRECTCUR DeviceSetQGWithDirectCur;
  EXTRN SETQGWITHDIRECTCUREX DeviceSetQGWithDirectCurEx;
  EXTRN GETGATHER DeviceGetGather;
  EXTRN CLEARGATHER DeviceClearGather;
  EXTRN INITGATHER DeviceInitGather;
  EXTRN CLEARGATHERDATA DeviceClearGatherData;
  EXTRN COLLECTGATHERDATA DeviceCollectGatherData;
  EXTRN GETGATHERPOINT DeviceGetGatherPoint;
  EXTRN STARTGATHER DeviceStartGather;
  EXTRN STOPGATHER DeviceStopGather;
  // Real time
  EXTRN INITRTGATHER DeviceInitRTGather;
  EXTRN CLEARRTGATHER DeviceClearRTGather;
  EXTRN ADDRTGATHER DeviceAddRTGather;
  EXTRN COLLECTRTGATHERDATA DeviceCollectRTGatherData;
  EXTRN ULTRALIGHTANALYZE DeviceUltraLightAnalyze;
  EXTRN MACROGETIVARIABLELONG DeviceMACROGetIVariableLong;
  EXTRN MACROSETIVARIABLESTRING DeviceMACROSetIVariableString;
  EXTRN MACROGETIVARIABLESTRING DeviceMACROGetIVariableString;
  EXTRN MACROUPLOADCONFIG DeviceMACROUploadConfig;

  // Turbo setup
  EXTRN AMPENABLED DeviceAmpEnabled;
  EXTRN AMPFAULT DeviceAmpFault;
  EXTRN ONNEGATIVELIMIT DeviceOnNegativeLimit;
  EXTRN ONPOSITIVELIMIT DeviceOnPositiveLimit;
  EXTRN FATALFERROR DeviceFatalFError;
  EXTRN FOLLOWERROR DeviceFollowError;
  EXTRN PHASEDMOTOR DevicePhasedMotor;
  EXTRN GETMOTORSTATUS DeviceGetMotorStatus;

  EXTRN DOWNLOADGANTRYAUTOTUNINGPLC DeviceDownloadGantryAutotuningPLC;
  EXTRN DOWNLOADGANTRYSTEPPLC DeviceDownloadGantryStepPLC;
  EXTRN DOWNLOADGANTRYPARABOLICPLC DeviceDownloadGantryParabolicPLC;

//  EXTRN VMETESTOPENPMACDEVICE  DeviceVMETestOpen;
//  EXTRN VMETESTCLOSEPMACDEVICE  DeviceVMETestClose;
//  EXTRN VMESETUP  DeviceVMESetup;

  EXTRN PMACSOCKISMULTIHOST DeviceSockIsMultiHost;
  EXTRN PMACSOCKISINVALID DeviceSockIsInvalid;

//2013.12.19 added by DTK
  EXTRN DPRGLOBALSTATUS				DeviceDPRGlobalStatus;
  EXTRN DPRMOTORSERVOSTATUSTURBO	DeviceDPRMotorServoStatusTurbo;
  EXTRN DPRMOTORSTATUSTURBO			DeviceDPRMotorStatusTurbo;
  EXTRN DPRMOTORREALTIMESTRUCT		DeviceDPRMotorRealtimeStruct;
  EXTRN DPRMOTORREALTIMESTRUCTTURBO	DeviceDPRMotorRealtimeStructTurbo;

  EXTRN DPRDATABLOCK		DeviceDPRDataBlock;
  EXTRN DPRPHASEDMOTOR		DeviceDPRPhasedMotor;
  EXTRN DPRMOTORENABLED		DeviceDPRMotorEnabled;
  EXTRN DPRHANDWHEELENABLED	DeviceDPRHandwheelEnabled;
  EXTRN DPROPENLOOP			DeviceDPROpenLoop;
  EXTRN DPRONNEGATIVELIMIT	DeviceDPROnNegativeLimit;
  EXTRN DPRONPOSITIVELIMIT	DeviceDPROnPositiveLimit;
//end of 2013.12.19


  #ifdef __cplusplus

}

  #endif

#endif
