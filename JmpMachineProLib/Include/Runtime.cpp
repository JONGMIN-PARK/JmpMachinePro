/*************************************************************************
  File: runtime.cpp - Dynamic link functions.

HISTORY

  02/02/1996  DES created.

**************************************************************************/

#ifdef _MSC
  #include "stdafx.h"
#endif

#pragma hdrstop

#define NO_EXTERN
#include "runtime.h"
#undef  NO_EXTERN

//************************************************************************
// Function instants for runtime linking
//************************************************************************
HINSTANCE hPmacLib = NULL;
BOOL DO_NC_FUNCTIONS = true;// TRUE by default

//----------------------------------------------------------------------------
void DoNCFunctions(BOOL yesno)
{
  DO_NC_FUNCTIONS = yesno;
}
//----------------------------------------------------------------------------
FARPROC GetFuncAddress(HMODULE hModule,LPCSTR lpProcName)
{
 //TCHAR tmp[256];
 FARPROC adr;

 adr = GetProcAddress(hModule,lpProcName);
 if (adr == NULL)
   {
   // sprintf(tmp, _T("Error in loading library function: %s"),lpProcName);
   // MessageBox(NULL,tmp, _T(""),MB_ICONSTOP | MB_OK);
   }
 return adr;
}

//----------------------------------------------------------------------------
void CloseRuntimeLink()
{
 if (hPmacLib != NULL)
   {
    FreeLibrary(hPmacLib);
    hPmacLib = NULL;
   }
}

// Macro to make things consistant and easier
#define DLL_LINK(var,type,name)  var=(type)GetFuncAddress(hPmacLib,name); \
                                 if (var==NULL) goto load_err

//----------------------------------------------------------------------------
HINSTANCE OpenRuntimeLink()
{
  // Get handle to PComm32.DLL
  hPmacLib = LoadLibrary(DRIVERNAME);

  if (hPmacLib == NULL)
  {
    MessageBox(NULL, _T("Error in loading PMAC library."), _T("PMAC"),MB_ICONSTOP | MB_OK | MB_TOPMOST);
    return NULL;
  }
  else
  {
// Get all COMM the function handles required
DLL_LINK(DeviceDPRPCITest              ,PMACDPRPCITEST           ,"PmacDPRPCITest");
DLL_LINK(DeviceOpen                    ,OPENPMACDEVICE           ,"OpenPmacDevice");
DLL_LINK(DeviceSelect                  ,SELECTPMACDEVICE         ,"PmacSelect");
DLL_LINK(DeviceNumberOfDevices         ,NUMBEROFDEVICES          ,"DrvNumberOfDevices");
DLL_LINK(DeviceClose                   ,CLOSEPMACDEVICE          ,"ClosePmacDevice");
DLL_LINK(DeviceSetLanguage             ,PMACSETLANGUAGE          ,"PmacSetLanguage");
DLL_LINK(DeviceLMH                     ,PMACLMH                  ,"PmacLMH");
DLL_LINK(DeviceSetWindow               ,PMACSETWINDOW            ,"PmacSetWindow");
DLL_LINK(DeviceSetLanguageModule       ,SETLANGUAGEMODULE        ,"SetLanguageModule");
DLL_LINK(DeviceConfigure               ,CONFIGURE                ,"PmacConfigure");
DLL_LINK(DeviceLoadString              ,SZLOADSTRINGA            ,"szLoadStringA");
DLL_LINK(DeviceReadReady               ,READREADY                ,"PmacReadReady");
DLL_LINK(DeviceSendLine                ,SENDLINEA                ,"PmacSendLineA");
DLL_LINK(DeviceSendLineW               ,SENDLINEW                ,"PmacSendLineW");
DLL_LINK(DeviceGetLine                 ,GETLINEA                 ,"PmacGetLineA");
DLL_LINK(DeviceGetLineW                ,GETLINEW                 ,"PmacGetLineW");
DLL_LINK(DeviceGetLineEx               ,GETLINEEXA               ,"PmacGetLineExA");
DLL_LINK(DeviceGetLineExW              ,GETLINEEXW               ,"PmacGetLineExW");
DLL_LINK(DeviceSendChar                ,SENDCHARA                ,"PmacSendCharA");
DLL_LINK(DeviceGetResponse             ,GETRESPONSEA             ,"PmacGetResponseA");
DLL_LINK(DeviceGetResponseW            ,GETRESPONSEW             ,"PmacGetResponseW");
DLL_LINK(DeviceGetResponseEx           ,GETRESPONSEEXA           ,"PmacGetResponseExA");
DLL_LINK(DeviceGetResponseExW          ,GETRESPONSEEXW           ,"PmacGetResponseExW");
DLL_LINK(DeviceGetControlResponse      ,GETCONTROLRESPONSEA      ,"PmacGetControlResponseA");
DLL_LINK(DeviceGetControlResponseW     ,GETCONTROLRESPONSEW      ,"PmacGetControlResponseW");
DLL_LINK(DeviceGetControlResponseEx    ,GETCONTROLRESPONSEEXA    ,"PmacGetControlResponseExA");
DLL_LINK(DeviceGetControlResponseExW   ,GETCONTROLRESPONSEEXW    ,"PmacGetControlResponseExW");
DLL_LINK(DeviceGetAsciiComm            ,GETASCIICOMM             ,"PmacGetAsciiComm");
DLL_LINK(DeviceSetAsciiComm            ,SETASCIICOMM             ,"PmacSetAsciiComm");
DLL_LINK(DeviceGetPmacLocation         ,GETLOCATION              ,"PmacGetLocation");
DLL_LINK(DeviceGetPmacEnumeration      ,GETENUMERATION           ,"PmacGetEnumeration");
DLL_LINK(DeviceGetError                ,GETERROR                 ,"PmacGetError");
DLL_LINK(DeviceDownload                ,DOWNLOADA                ,"PmacDownloadA");
DLL_LINK(DeviceDownloadThread          ,DOWNLOADTHREADA          ,"PmacThreadDownloadA");
DLL_LINK(DeviceDownloadProgress        ,DOWNLOAD_SHOWPROGRESS    ,"PmacDownloadProgress");
DLL_LINK(DeviceINTRTerminate           ,INTRTERMINATE            ,"PmacINTRTerminate");
DLL_LINK(DeviceINTRWndMsgInit          ,INTRWNDMSGINIT           ,"PmacINTRWndMsgInit");
DLL_LINK(DeviceINTRSetupMask           ,INTRSETUPMASK            ,"PmacINTRSetupMask");
DLL_LINK(DeviceINTRFuncCallInit        ,INTRFUNCCALLINIT         ,"PmacINTRFuncCallInit");
DLL_LINK(DeviceLock                    ,LOCKPMAC                 ,"LockPmac");
DLL_LINK(DeviceRelease                 ,RELEASEPMAC              ,"ReleasePmac");
DLL_LINK(DeviceSetCriticalIVars        ,SETCRITICALIVARS         ,"PmacSetCriticalIVars");
DLL_LINK(DeviceGetRomDate              ,GETROMDATEA              ,"PmacGetRomDateA");
DLL_LINK(DeviceGetRomVersion           ,GETROMVERSIONA           ,"PmacGetRomVersionA");
DLL_LINK(DeviceGetErrorStr             ,GETERRORSTRA             ,"PmacGetErrorStrA");
DLL_LINK(DeviceMultiDownload           ,MULTIDOWNLOADA           ,"PmacMultiDownloadA");
DLL_LINK(DeviceAddDownloadFile         ,ADDDOWNLOADFILEA         ,"PmacAddDownloadFileA");
DLL_LINK(DeviceRemoveDownloadFile      ,REMOVEDOWNLOADFILEA      ,"PmacRemoveDownloadFileA");
DLL_LINK(DeviceRenumberFiles           ,RENUMBERFILESA           ,"PmacRenumberFilesA");
DLL_LINK(DeviceRemoveDownloadFile      ,REMOVEDOWNLOADFILEA      ,"PmacRemoveDownloadFileA");
DLL_LINK(DeviceInBootStrapMode         ,INBOOTSTRAP              ,"PmacInBootStrapMode");
DLL_LINK(DeviceGetBuffer               ,GETBUFFERA               ,"PmacGetBufferA");
DLL_LINK(DevicePmacConfigure           ,PMACCONFIGURE            ,"PmacConfigure");
DLL_LINK(DeviceTestDPRAM               ,TESTDPRAM                ,"PmacDPRTest");
DLL_LINK(DeviceTestDPRAMAbort          ,TESTDPRABORT             ,"PmacAbortDPRTest");
DLL_LINK(DeviceDPRAvailable            ,DPRAVAILABLE             ,"PmacGetDpramAvailable");
//DLL_LINK(DeviceDPRGetPtr               ,DPRGETPTR                ,"PmacDPRGetPtr");

DLL_LINK(DeviceRestoreLinkList         ,RESTORELINKLIST           ,"RestoreLinkList");
DLL_LINK(DeviceDoChecksums             ,DOCHECKSUMS               ,"PmacDoChecksums");

DLL_LINK(DeviceStoreUSResponse         ,STOREUSRESPONSE           ,"StoreUSResponse");
DLL_LINK(DeviceGetUSResponse           ,GETUSRESPONSE             ,"GetUSResponse");
DLL_LINK(DeviceUSReadReady             ,USREADREADY               ,"USReadReady");

DLL_LINK(DeviceChangeGet           ,CHANGEGET                 ,"ChangeGet");
DLL_LINK(DeviceChangeStore         ,CHANGESTORE               ,"ChangeStore");
DLL_LINK(DeviceChangeReadReady     ,CHANGEREADREADY           ,"ChangeReadReady");

DLL_LINK(DeviceMapDeviceNumToEnumAndLocation  ,MAPDEVICENUMTOENUANDLOC  ,"MapDeviceNumToEnumAndLocation");
DLL_LINK(DeviceMapLocAndEnumFromDeviceNum  ,MAPLOCANDENUMFROMDEVICENUM,"MapLocAndEnumFromDeviceNum");
DLL_LINK(DeviceShutdownPmac,DEVICESHUTDOWNPMAC,"ShutdownPmacDevice");



// Checksum communication rouitnes
DLL_LINK(DeviceSERDoChecksums          ,SERDOCHECKSUMS           ,"PmacSERDoChecksums");
DLL_LINK(DeviceSERCheckSendLineA       ,SERCHECKSENDLINEA        ,"PmacSERCheckSendLineA");
DLL_LINK(DeviceSERCheckGetLineA        ,SERCHECKGETLINEA         ,"PmacSERCheckGetLineA");
DLL_LINK(DeviceSERCheckResponseA       ,SERCHECKRESPONSEA        ,"PmacSERCheckResponseA");
DLL_LINK(DeviceSERCheckAllWentWell     ,SERCHECKALLWENTWELL      ,"PmacSerCheckAllWentWell");

DLL_LINK(DeviceAbortDownload           ,ABORTDOWNLOAD            ,"PmacAbortDownload");
DLL_LINK(DeviceSetMaxDownloadErrors    ,SETMAXDOWNLOADERRORS     ,"PmacSetMaxDownloadErrors");
DLL_LINK(DeviceDPRRotBufClear          ,DPRROTBUFCLEAR           ,"PmacDPRRotBufClr");
DLL_LINK(DeviceDPRRotBufPut            ,DPRROTBUFPUT             ,"PmacDPRrotput");

DLL_LINK(DevicePmacIsTurbo             ,PMACISTURBO              ,"bPMACIsTurbo");

// Turbo Setup
DLL_LINK(DeviceAmpEnabled              ,AMPENABLED               ,"PmacAmpEnabled");
DLL_LINK(DeviceAmpFault                ,AMPFAULT                 ,"PmacAmpFault");
DLL_LINK(DeviceOnNegativeLimit         ,ONNEGATIVELIMIT          ,"PmacOnNegativeLimit");
DLL_LINK(DeviceOnPositiveLimit         ,ONPOSITIVELIMIT          ,"PmacOnPositiveLimit");
DLL_LINK(DeviceFatalFError             ,FATALFERROR              ,"PmacFatalFError");
DLL_LINK(DeviceFollowError             ,FOLLOWERROR              ,"PmacFollowError");
DLL_LINK(DevicePhasedMotor             ,PHASEDMOTOR              ,"PmacPhasedMotor");
DLL_LINK(DeviceGetMotorStatus          ,GETMOTORSTATUS           ,"PmacGetMotorStatus");
// Data Gathering
DLL_LINK(DeviceGetGatherPeriod         ,GETGATHERPERIOD          ,"PmacGetGatherPeriod");
DLL_LINK(DeviceGetGatherSampleTime     ,GETGATHERSAMPLETIME      ,"PmacGetGatherSampleTime");
DLL_LINK(DeviceSetGatherSampleTime     ,SETGATHERSAMPLETIME      ,"PmacSetGatherSampleTime");
DLL_LINK(DeviceGetNumGatherSources     ,GETNUMGATHERSOURCES      ,"PmacGetNumGatherSources");
DLL_LINK(DeviceGetNumGatherSamples     ,GETNUMGATHERSAMPLES      ,"PmacGetNumGatherSamples");
DLL_LINK(DeviceSetGatherPeriod         ,SETGATHERPERIOD          ,"PmacSetGatherPeriod");
DLL_LINK(DeviceSetGatherEnable         ,SETGATHERENABLE          ,"PmacSetGatherEnable");
DLL_LINK(DeviceGetGatherEnable         ,GETGATHERENABLE          ,"PmacGetGatherEnable");
DLL_LINK(DeviceSetCurrentGather        ,SETCURRENTGATHER         ,"PmacSetCurrentGather");
DLL_LINK(DeviceSetGather               ,SETGATHER                ,"PmacSetGather");
DLL_LINK(DeviceSetQuickGather          ,SETQUICKGATHER           ,"PmacSetQuickGather");
DLL_LINK(DeviceSetQuickGatherEx        ,SETQUICKGATHEREX         ,"PmacSetQuickGatherEx");
DLL_LINK(DeviceSetQGWithDirectCur      ,SETQGWITHDIRECTCUR       ,"PmacSetQuickGatherWithDirectCurrent");
DLL_LINK(DeviceSetQGWithDirectCurEx    ,SETQGWITHDIRECTCUREX     ,"PmacSetQuickGatherWithDirectCurrentEx");
DLL_LINK(DeviceGetGather               ,GETGATHER                ,"PmacGetGather");
DLL_LINK(DeviceClearGather             ,CLEARGATHER              ,"PmacClearGather");
DLL_LINK(DeviceInitGather              ,INITGATHER               ,"PmacInitGather");
DLL_LINK(DeviceClearGatherData         ,CLEARGATHERDATA          ,"PmacClearGatherData");
DLL_LINK(DeviceCollectGatherData       ,COLLECTGATHERDATA        ,"PmacCollectGatherData");
DLL_LINK(DeviceGetGatherPoint          ,GETGATHERPOINT           ,"PmacGetGatherPoint");
DLL_LINK(DeviceGetGatherSamples        ,GETGATHERSAMPLES         ,"PmacGetGatherSamples");
DLL_LINK(DeviceStartGather             ,STARTGATHER              ,"PmacStartGather");
DLL_LINK(DeviceStopGather              ,STOPGATHER               ,"PmacStopGather");

// MACRO Station
DLL_LINK(DeviceUltraLightAnalyze       ,ULTRALIGHTANALYZE        ,"UltraLightAnalyze");
DLL_LINK(DeviceMACROGetIVariableLong   ,MACROGETIVARIABLELONG    ,"MACROGetIVariableLong");
DLL_LINK(DeviceMACROUploadConfig       ,MACROUPLOADCONFIG        ,"MACROUploadConfig");

DLL_LINK(DeviceMACROSetIVariableString ,MACROSETIVARIABLESTRING  ,"MACROSetIVariableString");
DLL_LINK(DeviceMACROGetIVariableString ,MACROGETIVARIABLESTRING  ,"MACROGetIVariableString");

DLL_LINK(DeviceGetVariableStr          ,GETVARIABLESTRA          ,"PmacGetVariableStrA");
DLL_LINK(DeviceGetVariableLong         ,GETVARIABLELONG          ,"PmacGetVariableLong");
DLL_LINK(DeviceGetVariableDouble       ,GETVARIABLEDOUBLE        ,"PmacGetVariableDouble");
//++ Added
DLL_LINK(DeviceSetVariable				,SETVARIABLE			,"PmacSetVariable");
DLL_LINK(DeviceSetVariableLong			,SETVARIABLELONG		,"PmacSetVariableLong");
DLL_LINK(DeviceSetVariableDouble		,SETVARIABLEDOUBLE		,"PmacSetVariableDouble");
// by DTK

DLL_LINK(DeviceGetIVariableLong        ,GETIVARIABLELONG         ,"PmacGetIVariableLong");
DLL_LINK(DeviceGetIVariableDouble      ,GETIVARIABLEDOUBLE       ,"PmacGetIVariableDouble");
DLL_LINK(DeviceSetIVariableLong        ,SETIVARIABLELONG         ,"PmacSetIVariableLong");
DLL_LINK(DeviceSetIVariableDouble      ,SETIVARIABLEDOUBLE       ,"PmacSetIVariableDouble");
DLL_LINK(DeviceFlush                   ,FLUSH                    ,"PmacFlush");
DLL_LINK(DeviceGetPlcStatus            ,GETPLCSTATUS             ,"PmacGetPlcStatus");
DLL_LINK(DeviceIsAnyPLCRunning         ,ISANYPLCRUNNING          ,"PmacIsAnyPLCRunning");

DLL_LINK(DeviceGetUserHandle           ,GETUSERHANDLE            ,"PmacGetUserHandle");
DLL_LINK(DeviceGetPmacType             ,GETPMACTYPE              ,"PmacGetPmacType");
DLL_LINK(DeviceWhyMotorNotMoving       ,WHYMOTORNOTMOVING        ,"whyMotorNotMoving");
DLL_LINK(DeviceWhyMotorNotMovingTURBO  ,WHYMOTORNOTMOVINGTURBO   ,"whyMotorNotMovingStringTURBO");
DLL_LINK(DeviceWhyMotorNotMovingString ,WHYMOTORNOTMOVINGSTRING  ,"whyMotorNotMovingString");
DLL_LINK(DeviceWhyMotorNotMovingStringTURBO   ,WHYMOTORNOTMOVINGSTRINGTURBO  ,"whyMotorNotMovingStringTURBO");
DLL_LINK(DeviceWhyCsNotMovingString    ,WHYCSNOTMOVINGSTRING     ,"whyCSNotMovingString");
DLL_LINK(DeviceWhyCsNotMoving          ,WHYCSNOTMOVING           ,"whyCSNotMoving");
DLL_LINK(DeviceWhyCsNotMovingStringTURBO,WHYCSNOTMOVINGSTRINGTURBO,"whyCSNotMovingStringTURBO");
DLL_LINK(DeviceWhyCsNotMovingTURBO     ,WHYCSNOTMOVINGTURBO      ,"whyCSNotMovingTURBO");
DLL_LINK(DeviceGetBitValue             ,GETBITVALUE              ,"getBitValue");
DLL_LINK(DeviceHexLong2                ,HEXLONG2                 ,"hex_long2");
DLL_LINK(DeviceInBufferedMode          ,INBUFFEREDMODE           ,"InBufferedMode");

DLL_LINK(DeviceSERGetPort              ,SERGETPORT               ,"PmacSERGetPort");
//DLL_LINK(DeviceSERSetPort              ,SERSETPORT               ,"PmacSERSetPort");
DLL_LINK(DeviceSERGetBaudrate          ,SERGETBAUDRATE           ,"PmacSERGetBaudrate");
//DLL_LINK(DeviceSERSetBaudrate          ,SERSETBAUDRATE           ,"PmacSERSetBaudrate");

DLL_LINK(DeviceDPRBufLast              ,DPRBUFLAST               ,"PmacDPRBufLast");
DLL_LINK(DeviceDPRRotBuf               ,DPRROTBUF                ,"PmacDPRRotBuf");
DLL_LINK(DeviceDPRAsciiStrToRot        ,DPRASCIISTRTOROTA        ,"PmacDPRAsciiStrToRotA");
DLL_LINK(DeviceDPRAsciiStrToRotEx      ,DPRASCIISTRTOROTEX       ,"PmacDPRAsciiStrToRotEx");
DLL_LINK(DeviceDPRRotBufInit           ,DPRROTBUFINIT            ,"PmacDPRRotBufInit");
DLL_LINK(DeviceDPRRotBufRemove         ,DPRROTBUFREMOVE          ,"PmacDPRRotBufRemove");
DLL_LINK(DeviceDPRRotBufChange         ,DPRROTBUFCHANGE          ,"PmacDPRRotBufChange");
DLL_LINK(DeviceDPRGetVBGServoTimer     ,DPRGETVBGSERVOTIMER      ,"PmacDPRGetVBGServoTimer");
DLL_LINK(DeviceDPRRealTime             ,DPRREALTIME              ,"PmacDPRRealTime");
//DLL_LINK(DeviceDPRSetHostBusyBit       ,DPRSETHOSTBUSYBIT        ,"PmacDPRSetHostBusyBit");
//DLL_LINK(DeviceDPRGetHostBusyBit       ,DPRGETHOSTBUSYBIT        ,"PmacDPRGetHostBusyBit");
//DLL_LINK(DeviceDPRGetPMACBusyBit       ,DPRGETPMACBUSYBIT        ,"PmacDPRGetPmacBusyBit");
DLL_LINK(DeviceDPRGetServoTimer        ,DPRGETSERVOTIMER         ,"PmacDPRGetServoTimer");
DLL_LINK(DeviceDPRSetMotors            ,DPRSETMOTORS             ,"PmacDPRSetMotors");
DLL_LINK(DeviceDPRGetCommandedPos      ,DPRGETCOMMANDEDPOS       ,"PmacDPRGetCommandedPos");
DLL_LINK(DeviceDPRPosition             ,DPRPOSITION              ,"PmacDPRPosition");
DLL_LINK(DeviceDPRNetActualPosition    ,DPRNETACTUALPOSITION        ,"PmacNetActualPosition");
DLL_LINK(DeviceDPRFollowError          ,DPRFOLLOWERROR           ,"PmacDPRFollowError");
DLL_LINK(DeviceDPRGetVel               ,DPRGETVEL                ,"PmacDPRGetVel");
DLL_LINK(DeviceDPRGetMasterPos         ,DPRGETMASTERPOS          ,"PmacDPRGetMasterPos");
DLL_LINK(DeviceDPRGetCompensationPos   ,DPRGETCOMPENSATIONPOS    ,"PmacDPRGetCompensationPos");
DLL_LINK(DeviceDPRGetPrevDAC           ,DPRGETPREVDAC            ,"PmacDPRGetPrevDAC");
DLL_LINK(DeviceDPRGetMoveTime          ,DPRGETMOVETIME           ,"PmacDPRGetMoveTime");
//DLL_LINK(DeviceDPRResetDataReadyBit    ,DPRRESETDATAREADYBIT     ,"PmacDPRResetDataReadyBit");
//DLL_LINK(DeviceDPRGetDataReadyBit      ,DPRGETDATAREADYBIT       ,"PmacDPRGetDataReadyBit");
DLL_LINK(DeviceDPRUpdateRealtime       ,DPRUPDATEREALTIME        ,"PmacDPRUpdateRealTime");
//DLL_LINK(DeviceDPRMotorServoStatusTurbo,DPRMOTORSERVOSTATUSTURBO ,"PmacDPRMotorServoStatusTurbo");
DLL_LINK(DeviceDPRRealTimeEx           ,DPRREALTIMEEX            ,"PmacDPRRealTimeEx");
DLL_LINK(DeviceDPRRealTimeSetMotor     ,DPRREALTIMESETMOTOR      ,"PmacDPRRealTimeSetMotor");
DLL_LINK(DeviceDPRBackground           ,DPRBACKGROUND            ,"PmacDPRBackground");
DLL_LINK(DeviceDPRBackgroundEx         ,DPRBACKGROUNDEX          ,"PmacDPRBackgroundEx");
DLL_LINK(DeviceDPRBackGroundVar        ,DPRBACKGROUNDVAR         ,"PmacDPRBackGroundVar");
DLL_LINK(DeviceDPRGetTargetPos         ,DPRGETTARGETPOS          ,"PmacDPRGetTargetPos");
DLL_LINK(DeviceDPRGetBiasPos           ,DPRGETBIASPOS            ,"PmacDPRGetBiasPos");
DLL_LINK(DeviceDPRPe                   ,DPRPE                    ,"PmacDPRPe");
DLL_LINK(DeviceDPRTimeRemainingInMove  ,DPRTIMEREMINMOVE         ,"PmacDPRTimeRemInMove");
DLL_LINK(DeviceTimeRemInTATS           ,DPRTIMEREMINTATS         ,"PmacDPRTimeRemInTATS");
DLL_LINK(DeviceDPRCommanded            ,DPRCOMMANDED             ,"PmacDPRCommanded");
////////////////////////////////////////////////////////////////////////////
// DPR Control Panel
////////////////////////////////////////////////////////////////////////////
DLL_LINK(DeviceDPRControlPanel         ,DPRCONTROLPANEL          ,"PmacDPRControlPanel");
DLL_LINK(DeviceDPRSetJogPosBit         ,DPRSETJOGPOSBIT          ,"PmacDPRSetJogPosBit");
DLL_LINK(DeviceDPRGetJogPosBit         ,DPRGETJOGPOSBIT          ,"PmacDPRGetJogPosBit");
DLL_LINK(DeviceDPRSetJogNegBit         ,DPRSETJOGNEGBIT          ,"PmacDPRSetJogNegBit");
DLL_LINK(DeviceDPRGetJogNegBit         ,DPRGETJOGNEGBIT          ,"PmacDPRGetJogNegBit");
DLL_LINK(DeviceDPRSetJogReturnBit      ,DPRSETJOGRETURNBIT       ,"PmacDPRSetJogReturnBit");
DLL_LINK(DeviceDPRGetJogReturnBit      ,DPRGETJOGRETURNBIT       ,"PmacDPRGetJogReturnBit");
DLL_LINK(DeviceDPRSetRunBit            ,DPRSETRUNBIT             ,"PmacDPRSetRunBit");
DLL_LINK(DeviceDPRGetRunBit            ,DPRGETRUNBIT             ,"PmacDPRGetRunBit");
DLL_LINK(DeviceDPRSetStopBit           ,DPRSETSTOPBIT            ,"PmacDPRSetStopBit");
DLL_LINK(DeviceDPRGetStopBit           ,DPRGETSTOPBIT            ,"PmacDPRGetStopBit");
DLL_LINK(DeviceDPRSetHomeBit           ,DPRSETHOMEBIT            ,"PmacDPRSetHomeBit");
DLL_LINK(DeviceDPRGetHomeBit           ,DPRGETHOMEBIT            ,"PmacDPRGetHomeBit");
DLL_LINK(DeviceDPRSetHoldBit           ,DPRSETHOLDBIT            ,"PmacDPRSetHoldBit");
DLL_LINK(DeviceDPRGetHoldBit           ,DPRGETHOLDBIT            ,"PmacDPRGetHoldBit");
DLL_LINK(DeviceDPRGetStepBit           ,DPRGETSTEPBIT            ,"PmacDPRGetStepBit");
DLL_LINK(DeviceDPRSetStepBit           ,DPRSETSTEPBIT            ,"PmacDPRSetStepBit");
DLL_LINK(DeviceDPRGetRequestBit        ,DPRGETREQUESTBIT         ,"PmacDPRGetRequestBit");
DLL_LINK(DeviceDPRSetRequestBit        ,DPRSETREQUESTBIT         ,"PmacDPRSetRequestBit");
DLL_LINK(DeviceDPRGetFOEnableBit       ,DPRGETFOENABLEBIT        ,"PmacDPRGetFOEnableBit");
DLL_LINK(DeviceDPRSetFOEnableBit       ,DPRSETFOENABLEBIT        ,"PmacDPRSetFOEnableBit");
DLL_LINK(DeviceDPRSetFOValue           ,DPRSETFOVALUE            ,"PmacDPRSetFOValue");
DLL_LINK(DeviceDPRGetFOValue           ,DPRGETFOVALUE            ,"PmacDPRGetFOValue");
DLL_LINK(DeviceDPRVarBufInit           ,DPRVARBUFINIT            ,"PmacDPRVarBufInit");
DLL_LINK(DeviceDPRVarBufInitEx         ,DPRVARBUFINITEX          ,"PmacDPRVarBufInitEx");
DLL_LINK(DeviceDPRVarBufRemove         ,DPRVARBUFREMOVE          ,"PmacDPRVarBufRemove");
DLL_LINK(DeviceDPRVarBufChange         ,DPRVARBUFCHANGE          ,"PmacDPRVarBufChange");
DLL_LINK(DeviceDPRVarBufChangeEx       ,DPRVARBUFCHANGEEX        ,"PmacDPRVarBufChangeEx");
DLL_LINK(DeviceDPRWriteBuffer          ,DPRWRITEBUFFER           ,"PmacDPRWriteBuffer");
DLL_LINK(DeviceDPRWriteBufferEx        ,DPRWRITEBUFFEREX         ,"PmacDPRWriteBufferEx");
DLL_LINK(DeviceDPRVelocity             ,DPRVELOCITY              ,"PmacDPRVelocity");
DLL_LINK(DeviceDPRVarBufRead           ,DPRVARBUFREAD            ,"PmacDPRVarBufRead");
DLL_LINK(DeviceDPRVarBufReadEx         ,DPRVARBUFREADEX          ,"PmacDPRVarBufReadEx");
DLL_LINK(DeviceDPRGetMotorMotion       ,DPRGETMOTORMOTION        ,"PmacDPRGetMotorMotion");
DLL_LINK(DeviceDPRGetProgramMode       ,DPRGETPROGRAMMODE        ,"PmacDPRGetProgramMode");
DLL_LINK(DeviceDPRSysInposition        ,DPRSYSINPOSITION         ,"PmacDPRSysInposition");
DLL_LINK(DeviceDPRRotBufOpen           ,DPRROTBUFOPEN            ,"PmacDPRRotBufOpen");
DLL_LINK(DeviceDPRProgRemaining        ,DPRPROGREMAINING         ,"PmacDPRProgRemaining");
DLL_LINK(DeviceDPRGetFeedRateMode      ,DPRGETFEEDRATEMODE       ,"PmacDPRGetFeedRateMode");
DLL_LINK(DeviceDPRGetBiasPos           ,DPRGETBIASPOS            ,"PmacDPRGetBiasPos");

// Numeric read/write functions
// Standard
DLL_LINK(DeviceDPRSetMem              ,PMACDPRSETMEM               ,"PmacDPRSetMem");
DLL_LINK(DeviceDPRGetMem              ,PMACDPRGETMEM               ,"PmacDPRGetMem");
DLL_LINK(DeviceDPRGetWord              ,DPRGETWORD               ,"PmacDPRGetWord");
DLL_LINK(DeviceDPRSetWord              ,DPRSETWORD               ,"PmacDPRSetWord");
DLL_LINK(DeviceDPRGetDWord             ,DPRGETDWORD              ,"PmacDPRGetDWord");
DLL_LINK(DeviceDPRSetDWord             ,DPRSETDWORD              ,"PmacDPRSetDWord");
DLL_LINK(DeviceDPRGetFloat             ,DPRGETFLOAT              ,"PmacDPRGetFloat");
DLL_LINK(DeviceDPRSetFloat             ,DPRSETFLOAT              ,"PmacDPRSetFloat");
// Masking
DLL_LINK(DeviceDPRDWordBitSet          ,DPRDWORDBITSET           ,"PmacDPRDWordBitSet");
DLL_LINK(DeviceDPRSetDWordBit          ,DPRSETDWORDBIT           ,"PmacDPRSetDWordBit");
DLL_LINK(DeviceDPRResetDWordBit        ,DPRRESETDWORDBIT         ,"PmacDPRResetDWordBit");
DLL_LINK(DeviceDPRSetDWordMask         ,DPRSETDWORDMASK          ,"PmacDPRSetDWordMask");
DLL_LINK(DeviceDPRGetDWordMask         ,DPRGETDWORDMASK          ,"PmacDPRGetDWordMask");
DLL_LINK(DeviceDPRFloat                ,DPRFLOAT                 ,"PmacDPRFloat");
DLL_LINK(DeviceDPRLFixed               ,DPRLFIXED                ,"PmacDPRLFixed");
DLL_LINK(DeviceDPRAddressRange         ,DPRADDRESSRANGE          ,"PmacDPRAddressRange");
 
// Real time
DLL_LINK(DeviceInitRTGather            ,INITRTGATHER             ,"PmacInitRTGather");
DLL_LINK(DeviceClearRTGather           ,CLEARRTGATHER            ,"PmacClearRTGather");
DLL_LINK(DeviceAddRTGather             ,ADDRTGATHER              ,"PmacAddRTGather");
DLL_LINK(DeviceCollectRTGatherData     ,COLLECTRTGATHERDATA      ,"PmacCollectRTGatherData");

DLL_LINK(DeviceDownloadGantryAutotuningPLC  ,DOWNLOADGANTRYAUTOTUNINGPLC ,"DownloadGantryAutoTuningPLC");
DLL_LINK(DeviceDownloadGantryStepPLC  ,DOWNLOADGANTRYSTEPPLC ,"DoAGantryPositionLoopStep");
DLL_LINK(DeviceDownloadGantryParabolicPLC  ,DOWNLOADGANTRYPARABOLICPLC ,"DoAGantryParabolic");

//DLL_LINK(DeviceVMETestOpen             ,VMETESTOPENPMACDEVICE    ,"VMETestOpenPmacDevice");
//DLL_LINK(DeviceVMETestClose            ,VMETESTCLOSEPMACDEVICE   ,"VMETestClosePmacDevice");
//DLL_LINK(DeviceVMESetup            ,VMESETUP   ,"PmacVMESetup");

DLL_LINK(DeviceSockIsMultiHost, PMACSOCKISMULTIHOST, "PmacSockIsMultiHost");
DLL_LINK(DeviceSockIsInvalid, PMACSOCKISINVALID, "PmacSockIsInvalid");

//2013.12.19 added by DTK
DLL_LINK(DeviceDPRGlobalStatus, DPRGLOBALSTATUS, "PmacDPRGlobalStatus");
DLL_LINK(DeviceDPRMotorServoStatusTurbo, DPRMOTORSERVOSTATUSTURBO, "PmacDPRMotorServoStatusTurbo");
DLL_LINK(DeviceDPRMotorStatusTurbo, DPRMOTORSTATUSTURBO, "PmacDPRMotorStatusTurbo");
DLL_LINK(DeviceDPRMotorRealtimeStruct, DPRMOTORREALTIMESTRUCT, "PmacDPRMotorRealtimeStruct");
DLL_LINK(DeviceDPRMotorRealtimeStructTurbo, DPRMOTORREALTIMESTRUCTTURBO, "PmacDPRMotorRealtimeStructTurbo");

DLL_LINK(DeviceDPRDataBlock, DPRDATABLOCK, "PmacDPRDataBlock");
DLL_LINK(DeviceDPRPhasedMotor, DPRPHASEDMOTOR, "PmacDPRPhasedMotor");
DLL_LINK(DeviceDPRMotorEnabled, DPRMOTORENABLED, "PmacDPRMotorEnabled");
DLL_LINK(DeviceDPRHandwheelEnabled, DPRHANDWHEELENABLED, "PmacDPRHandwheelEnabled");
DLL_LINK(DeviceDPROpenLoop, DPROPENLOOP, "PmacDPROpenLoop");
DLL_LINK(DeviceDPROnNegativeLimit, DPRONNEGATIVELIMIT, "PmacDPROnNegativeLimit");
DLL_LINK(DeviceDPROnPositiveLimit, DPRONPOSITIVELIMIT, "PmacDPROnPositiveLimit");
//end of 2013.12.19

  return hPmacLib;

load_err:
    FreeLibrary(hPmacLib);
    hPmacLib = NULL;
    return NULL;
  }
}










