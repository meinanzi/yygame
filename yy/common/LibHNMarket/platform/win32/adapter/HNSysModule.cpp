#include "HNSysModule.h"
#include <windows.h> 
#include <conio.h>

#define MODULE_NAME     "sysmodule"

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace HN
{
	create_impl(SysModule)

    SysModule::SysModule() : Module(MODULE_NAME)
    {
		resisterMethod("getBuildVersion", HN_SELECTOR(SysModule::getBuildVersion));
		resisterMethod("getVersion", HN_SELECTOR(SysModule::getVersion));
		resisterMethod("getOSVersion", HN_SELECTOR(SysModule::getOSVersion));
		resisterMethod("getDevice", HN_SELECTOR(SysModule::getDevice));
		resisterMethod("getIMEI", HN_SELECTOR(SysModule::getIMEI));
		resisterMethod("GetSerialNumber", HN_SELECTOR(SysModule::GetSerialNumber));
		resisterMethod("installApp", HN_SELECTOR(SysModule::installApp));
		resisterMethod("getNetIp", HN_SELECTOR(SysModule::getNetIp));
    }
    
    SysModule::~SysModule()
    {
    }
    
    std::string SysModule::getVersion(const std::string& args)
    {
        return "3.1.1";
    }
    
    std::string SysModule::getBuildVersion(const std::string& args)
    {
        return "3.1.1";
    }
    
    std::string SysModule::getOSVersion(const std::string& args)
    {
         return "win32";
    }
    
    std::string SysModule::getDevice(const std::string& args)
    {
        return "win32";
    }

    std::string SysModule::getIMEI(const std::string& args)
    {
		GUID uuid; 
		CoCreateGuid(&uuid); 
		// Spit the address out 
		char mac_addr[18]; 
		sprintf(mac_addr,"%02X:%02X:%02X:%02X:%02X:%02X", 
			uuid.Data4[2],uuid.Data4[3],uuid.Data4[4], 
			uuid.Data4[5],uuid.Data4[6],uuid.Data4[7]);
        return mac_addr;
    }

	std::string SysModule::GetSerialNumber(const std::string& args)
	{
		char	volumeInfo[100];
		DWORD   volSerialNumber;
		DWORD   volMaxComponentLength;
		DWORD   volFileSystemFlags;
		char    fileSystemNameBuffer[100];
		GetVolumeInformationA("c:\\", volumeInfo, sizeof(volumeInfo), &volSerialNumber,
			&volMaxComponentLength, &volFileSystemFlags, fileSystemNameBuffer, sizeof(fileSystemNameBuffer));
	
		char str[64];
		sprintf(str, "%u", volSerialNumber);
		return str;

	}
    
    std::string SysModule::getISP(const std::string& args)
    {
        return "win32";
    }

	std::string SysModule::installApp(const std::string& args)
	{
		return "win32-installApp";
	}

	std::string SysModule::getNetIp(const std::string& args)
	{ 
		WSADATA wsaData;  
		int ret = WSAStartup(MAKEWORD(2,2),&wsaData);  
		if (0 != ret)  
		{  
			return "";  
		}  

		char hostname[256] = {0};  
		ret = gethostname(hostname,sizeof(hostname));  
		if (SOCKET_ERROR == ret)  
		{  
			return "";  
		}  

		HOSTENT* host = gethostbyname(hostname);  
		if (nullptr == host)  
		{  
			return "";  
		}  

		char ip[50] = {0};
		strcpy(ip, inet_ntoa(*(in_addr*)*host->h_addr_list));  
		return std::string(ip);  
	}
}

