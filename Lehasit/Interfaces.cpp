#include "stdafx.h"
#include "Interfaces.h"

Interfaces g_interfaces;

typedef void*(*CreateInterfaceFn)(const char* name, int* return_code);

void* GetInterface(const char* interface_name, CreateInterfaceFn factory);

bool Interfaces::load()
{
	//memset(this, 0, sizeof(*this));
	*this = { 0 };
	for (void** p = (void**)this; p < (void**)((size_t)this + sizeof(this)); p++)
		assert(*p == 0);

	CreateInterfaceFn clientFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA("client.dll"), "CreateInterface");
	CreateInterfaceFn engineFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA("engine.dll"), "CreateInterface");
	CreateInterfaceFn vguimatsurfaceFactory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA("vguimatsurface.dll"), "CreateInterface");
	CreateInterfaceFn vgui2Factory = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA("vgui2.dll"), "CreateInterface");

	client = (IBaseClientDLL*)GetInterface("VClient", clientFactory);
	prediction = (IPrediction*)GetInterface("VClientPrediction", clientFactory);
	entityList = (IClientEntityList*)GetInterface("VClientEntityList", clientFactory);

	trace = (IEngineTrace*)GetInterface("EngineTraceClient", engineFactory);
	engine = (IVEngineClient*)GetInterface("VEngineClient", engineFactory);
	modelInfo = (IVModelInfoClient*)GetInterface("VModelInfoClient", engineFactory);

	surface = (ISurface*)GetInterface("VGUI_Surface", vguimatsurfaceFactory);

	panel = (IPanel*)GetInterface("VGUI_Panel", vgui2Factory);

	if (client)
	{
		void** clientVmt = *(void***)client;
		clientMode = **(IClientMode***)((char*)(clientVmt[10]) + 5);
	}

	/*void* client_mode_sig = ScanModule("client.dll", "8B 0D ? ? ? ? D9 45 0C 56");
	if (client_mode_sig != nullptr) {
		client_mode = **reinterpret_cast<IClientMode***>((char*)client_mode_sig + 2);
	} else {
		return false;
	}*/

	// Check all our interface pointers for null
	// Really dirty code, it's left this way because I will be adding more interfaces
	// later and I don't want to forget to add it to a list of nullptr checks
	for (void** p = (void**)this; p < (void**)((char*)this + sizeof(*this)); p++)
	{
		if (*p == nullptr)
			return false;
	}

	return true;
}

void* GetInterface(const char* interface_name, CreateInterfaceFn factory)
{
	// The engine already does a bunch of expensive operations to find our interface,
	// should we really be doing this?

	char buffer[50];
	void* iface;
	for (int i = 0; i < 1000; i++)
	{
		sprintf_s(buffer, "%s%03d", interface_name, i);
		iface = factory(buffer, NULL);
		if (iface)
		{
			//MessageBoxA(NULL, buffer, "Found interface", MB_OK);
			return iface;
		}
	}

	return nullptr;
}