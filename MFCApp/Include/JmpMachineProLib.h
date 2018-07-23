// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JMPMACHINEPROLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JMPMACHINEPROLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef JMPMACHINEPROLIB_EXPORTS
#define JMPMACHINEPROLIB_API __declspec(dllexport)
#else
#define JMPMACHINEPROLIB_API __declspec(dllimport)
#endif

// This class is exported from the JmpMachineProLib.dll
class JMPMACHINEPROLIB_API CJmpMachineProLib {
public:
	CJmpMachineProLib(void);
	// TODO: add your methods here.
};

extern JMPMACHINEPROLIB_API int nJmpMachineProLib;

JMPMACHINEPROLIB_API int fnJmpMachineProLib(void);
