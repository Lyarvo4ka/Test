#pragma once

#include "Plugin.h"

//class Dll
//{
//private:
//	TCHAR file_name;
//	HMODULE module;
//public:
//	Dll(const CString &name) :module(NULL) {file_name = name;}
//	~Dll() {Unload();}
//
//	BOOL Load(void)
//	{
//		if (!(module = ::LoadLibrary(file_name)))
//			return FALSE;
//		else
//			return TRUE;
//	}
//
//	BOOL Unload(void)
//	{
//		BOOL ret = ::FreeLibrary(module);
//		module = NULL;
//		return ret;
//	}
//
//	FARPROC GetProcAddress(LPCSTR lpProcName) {return ::GetProcAddress(module, lpProcName);}
//
//	const CString &GetFileName() {return file_name;}
//};
//
//class Plugin : public Dll
//{
//private:
//	CString plugin_name;
//	CString plugin_description;
//	RawFileInterface *raw_file_interface;
//public:
//	Plugin(CString &path) : Dll(path) {}
//	~Plugin() {}
//
//	const CString &GetPluginName(void) {return plugin_name;}
//	const CString &GetPluginDescription(void) {return plugin_description;}
//	RawFileInterface *GetRawFileInterface(void) {return raw_file_interface;}
//};
//
//class RawFilesMgr
//{
//private:
//	CString pluginsDirPath;
//	List<Plugin> plugin_list;
//public:
//	RawFilesMgr(void);
//	~RawFilesMgr(void);
//
//	void SetPluginsDirPath(CString &dir) {pluginsDirPath = dir;}
//	CString GetPluginsDirPath(void) {return pluginsDirPath;}
//
//	BOOL UpdateRawFilesList(void);
//};
