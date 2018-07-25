#include "stdafx.h"
#include "CTurboPmacManager.h"
#include "Runtime.h"

#include "tinyxml2.h"

namespace jmp
{
	unsigned int CTurboPmacManager::kDprOffset = 0x1140; // 0x450 * 4

	CTurboPmacManager::CTurboPmacManager()
		: IMotionManager()
	{
	}

	CTurboPmacManager::~CTurboPmacManager()
	{
	}

	template<typename TK, typename TV>
	std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
		std::vector<TK> retval;
		for (auto const& element : input_map) {
			retval.push_back(element.first);
		}
		return retval;
	}

	template<typename TK, typename TV>
	std::vector<TV> extract_values(std::map<TK, TV> const& input_map) {
		std::vector<TV> retval;
		for (auto const& element : input_map) {
			retval.push_back(element.second);
		}
		return retval;
	}	

	int CTurboPmacManager::init(std::map<int, std::vector<int>>& channelMap)
	{
		mChannelMap = std::move(channelMap);

		loadParameter();

		if (OpenRuntimeLink() == nullptr)
		{
			return -1;
		}
		
		std::vector<int> keys = extract_keys(mChannelMap);

		mDeviceNumbers = keys;

		for (size_t n = 0; n < mDeviceNumbers.size(); n++)
		{
			DeviceOpen(mDeviceNumbers[n]);
		}		
		
		// DPRAM을 사용해서 위치 및 각종 데이터를 읽어오도록 설정한다.
		// (여기서는 DPRAM Realtime Update 기능을 사용하도록 설정한다.
		// DPR Realtime Fixed Data Buffer Enable
		bool enabled = false;
		/*for (size_t n = 0; n < deviceNumbers_.size(); n++)
		{
			enabled = DeviceDPRRealTimeEx(
				deviceNumbers_[n],
				0x0F,
				1,
				true
			);
		}*/
		
		for (const auto& number : mDeviceNumbers)
		{
			enabled = DeviceDPRRealTimeEx(
						number,
						0x0F,
						1,
						true
			);
		}
		 
		/*char chrResponse[255] = { 0, }, chrCmd[255] = { 0, };
		sprintf_s(chrCmd, 255, "P%d=%d", 8000, 1111);
		DeviceGetResponse(0, chrResponse, 255, chrCmd);
*/
		//DeviceSetVariableLong(0, 'P', 8000, 1832);

		//long value = DeviceGetVariableLong(0, 'P', 8000, 0);
	
		IMotionManager::init();

		mRotaryBufferLoopThread = std::thread(&executeRotaryBufferLoop, this);

		return 0;
	}

	void CTurboPmacManager::dispose()
	{
		IMotionManager::dispose();

		// Dpr
		terminated_ = true;
		mRotaryBufferLoopThread.join();
		abortRotaryBuffer();

		// Device Close
		for (const auto& item : mDeviceNumbers)
		{
			DeviceClose(item);
		}

		CloseRuntimeLink();
	}

	int CTurboPmacManager::loadParameter()
	{
		static const char* xml = "<element/>";
		tinyxml2::XMLDocument doc;
		doc.Parse(xml);

		int ret = doc.ErrorID();
		return 0;
	}
	
	void CTurboPmacManager::getStatus()
	{
		for (const auto& deviceNumber : mDeviceNumbers)
		{
			// 1. DPRAM Realtime update buffer를 이용한 위치 및 각종 Status 업데이트
			if (DeviceDPRUpdateRealtime(deviceNumber) == TRUE)
			{
				double commandPos = 0.0;
				double actualPos = 0.0;
				long value = 0; 

				commandPos = DeviceDPRGetCommandedPos(deviceNumber, 0, 1);
				actualPos = DeviceDPRPosition(deviceNumber, 0, 1);

				commandPos = DeviceDPRGetCommandedPos(deviceNumber, 0, 1);
				actualPos = DeviceDPRPosition(deviceNumber, 0, 1);
			}
		
			// 2. UMAC 내부의 PLC를 이용한 DRRAM Update사용
			// 각 모터의 Flag Register 및 ACC-11E의 I/O 입출력 상태 모니터링.
			// 첨부된 PMAC File의 "ACC_11Eexam.pmc"와 "DPRUpdatePLC.pmc" 파일을 참조
			// 할당된 DPR Address는 $60450 ~ $60459 까지...
			DeviceDPRGetMem(deviceNumber, kDprOffset, sizeof(PMAC_DPRTYPE) * 10, &mDprData);
		}		
	}

	bool CTurboPmacManager::loadProgramFile(const std::string& fileName)
	{
		fopen_s(&mInFile, fileName.c_str(), "rt");
		if (mInFile == nullptr)
		{
			return false;
		}

		int lineCount = 0;
		char line[255] = "";
		char* dummy = nullptr;
		do
		{
			dummy = fgets(line, 254, mInFile);
			if (dummy)
			{
				lineCount++;
			}
		} while (dummy != nullptr);
		rewind(mInFile);

		mLineCount = lineCount;

		return true;
	}

	bool CTurboPmacManager::initRotaryBuffer()
	{
		char buf[256] = "";
		char cmdbuf[256] = "";

		for (const auto& deviceNumber : mDeviceNumbers)
		{
			if (DeviceDPRRotBufChange(deviceNumber, 0, mRotateSize) < 0)
			{
				return false;
			}

			// 1번 좌표계에 있는 모든 모터들을 Closed Loop으로 만듭니다. (Ctrl + A)
			DeviceGetResponse(deviceNumber, buf, 255, "&1A");

			// 열려있는 모든 Buffer를 닫아 줍니다.
			DeviceGetResponse(deviceNumber, buf, 255, "CLOSE");

			// 정의되어 있는 모든 Rotary Buffer를 지웁니다.
			// Rotaty Buffer는 정의할 때에는 내림차순으로,
			// 지울 때에는 오름차순으로 명령을 씁니다.
			// 재 정의할 필요가 없을 때에는 지울 필요가 없겠죠...
			DeviceGetResponse(deviceNumber, buf, 255, "&1 DEL ROT");
			/*	DeviceGetResponse(m_nDeviceNo, buf, 255, "&2 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&3 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&4 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&5 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&6 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&7 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&8 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&9 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&10 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&11 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&12 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&13 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&14 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&15 DEL ROT");
			DeviceGetResponse(m_nDeviceNo, buf, 255, "&16 DEL ROT");
			*/
			// 실제 프로그램에서 사용할 Rotary Buffer의 Size를 설정해 줍니다.
			// DPRAM의 Binary Rotary 버퍼의 크기는 32비트이며,
			// 실제 내부 프로그램에서 사용하는 버퍼의 크기는 48비트이므로,
			// 실제 사이즈는 Binary Rotary Buffer Size의 1/2로 설정해 줍니다.
			sprintf_s(cmdbuf, sizeof(cmdbuf)/sizeof(char), "&1 DEF ROT %d", mRotateSize / 2);
			DeviceGetResponse(deviceNumber, buf, 255, cmdbuf);

			// 해당 좌표계의 Rotary Buffer Program을 "Run"시킵니다.
			// Rotary Buffer에 해당하는 프로그램 번호는 "0"번 입니다.
			DeviceGetResponse(deviceNumber, buf, 255, "&1B0R");

			// Binary Rotary Buffer를 열어 줍니다.
			// 이후부터는 Rotary Buffer에 명령을 보내면, 즉시 실행하게 됩니다.
			DeviceGetResponse(deviceNumber, buf, 255, "OPEN BIN ROT");
		}

		return true;
	}

	bool CTurboPmacManager::startRotaryBuffer()
	{
		if (mInFile == nullptr)
		{
			return false;
		}

		if (-1 != mSequenceNumber)
		{
			return false;
		}
		
		mIsDprError = false;

		mSequenceNumber = 0;
		return true;
	}

	void executeRotaryBufferLoop(void* obj)
	{
		CTurboPmacManager* pTurboPmacManager = static_cast<CTurboPmacManager*>(obj);
		char buf[256] = "";

		while (pTurboPmacManager->terminated_ == false)
		{
			switch (pTurboPmacManager->mSequenceNumber)
			{
			case 0:
				// file의 Pointer를 처음으로 옮깁니다.
				if (pTurboPmacManager->mInFile)
				{
					rewind(pTurboPmacManager->mInFile);
					pTurboPmacManager->mLineDownloadCount = 0;
				}
				else
					pTurboPmacManager->mIsDprError = true;
				pTurboPmacManager->mSequenceNumber = 1;
				break;

			case 1:
				// file에서 각 Line을 읽어와서 Binary Rotary Buffer로 전송합니다.
				fgetpos(pTurboPmacManager->mInFile, &pTurboPmacManager->mInFilePos);

				if (fgets(buf, 250, pTurboPmacManager->mInFile) == nullptr)
				{
					// 파일 끝인지를 확인합니다.
					if (feof(pTurboPmacManager->mInFile))
					{
						//MessageBox("파일의 끝입니다.","File Status",MB_OK);
						pTurboPmacManager->mSequenceNumber = 10;
						break;
					}
					/*else
					{
						MessageBox("파일 에러가 감지되었습니다.", "File Error", MB_OK);
					}*/
				}
				else
				{
					pTurboPmacManager->mLineDownloadCount++;					

					SHORT test = DeviceDPRAsciiStrToRot(0, buf, 0);
					if (test == DprOk)
					{
						//m_rdoBufferFull.SetCheck(0);
					}
					else if (test == DprBufBsy)
					{
						//m_rdoBufferFull.SetCheck(1);
						pTurboPmacManager->mSequenceNumber = 2;
					}
					else if (test == DprEOF)
					{
						pTurboPmacManager->mIsDprError = true;
						//m_rdoBufferFull.SetCheck(0);
					}
					else if (test == -70)
					{
						pTurboPmacManager->mIsDprError = true;
						//MessageBox("알수 없는 DPRAM 에러 입니다.");
					}

				}
				break;

			case 2:
			{
				if (DeviceDPRAsciiStrToRot(0, "", 0) == DprOk)
				{
					pTurboPmacManager->mSequenceNumber = 1;
					//m_rdoBufferFull.SetCheck(0);
				}
			}
			break;

			case 10:
				//pTurboPmacManager->stopRotaryBuffer();
				pTurboPmacManager->mSequenceNumber = -1;

				/*KillTimer(0);
				m_btnSelectFile.EnableWindow(TRUE);
				m_btnBegin.EnableWindow(FALSE);
				m_btnHalt.EnableWindow(FALSE);
				AfxMessageBox("프로그램 다운로드가 완료되었습니다.");*/
				break;
			}

			if (pTurboPmacManager->mIsDprError)
			{
				pTurboPmacManager->mSequenceNumber = -1;
				pTurboPmacManager->stopRotaryBuffer();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	bool CTurboPmacManager::stopRotaryBuffer()
	{
		char buf[256] = "";
		for (const auto& deviceNumber : mDeviceNumbers)
		{
			//  &1 좌표계의 Rotary Buffer Program 동작을 중지시킵니다.
			DeviceGetResponse(deviceNumber, buf, 255, "&1A");

			// Rotary Buffer를 닫아 줍니다.
			DeviceGetResponse(deviceNumber, buf, 255, "CLOSE");			
		}

		return true;
	}

	bool CTurboPmacManager::abortRotaryBuffer()
	{
		mSequenceNumber = -1;

		char buf[256] = "";
		for (const auto& deviceNumber : mDeviceNumbers)
		{
			//  &1 좌표계의 Rotary Buffer Program 동작을 중지시킵니다.
			DeviceGetResponse(deviceNumber, buf, 255, "&1A");

			// Rotary Buffer를 닫아 줍니다.
			DeviceGetResponse(deviceNumber, buf, 255, "CLOSE");

			// Rotary Buffer를 지워줍니다. 
			// 계속해서 사용할 경우에는 지워줄 필요는 없겠지요...
			DeviceGetResponse(deviceNumber, buf, 255, "&1 DEL ROT");

			// DPRAM에 정의되어 있는 Rotary Buffer를 지워줍니다.
			if (DeviceDPRBufLast(deviceNumber) > 0)		// 마지막 버퍼 번호를 얻어 옵니다. 
				DeviceDPRRotBufRemove(deviceNumber, 0);		// 현재는 1번 좌표계만 사용하고 있으므로...
		}
		
		if (mInFile)
		{
			fclose(mInFile);
			mInFile = nullptr;
		}

		return true;
	}
}