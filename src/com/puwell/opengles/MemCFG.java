package com.puwell.opengles;

public class MemCFG {
	public int dwCullFaceCfg; // PMPA_MEM_CFG_CF_
	public int bNeedTexture;
	public int dwTransparency; // 0~255 = 0~1.0f: transparency of member
	public int dwExtern;

	public MemCFG(int CullFaceCfg, int NeedTexture, int Transparency, int Extern) {
		dwCullFaceCfg = CullFaceCfg; // PMPA_MEM_CFG_CF_
		bNeedTexture = NeedTexture;
		dwTransparency = Transparency; // 0~255 = 0~1.0f: transparency of member
		dwExtern = Extern;
	}

	public MemCFG(MemCFG memCFG) {
		dwCullFaceCfg = memCFG.dwCullFaceCfg; // PMPA_MEM_CFG_CF_
		bNeedTexture = memCFG.bNeedTexture;
		dwTransparency = memCFG.dwTransparency; // 0~255 = 0~1.0f: transparency
												// of member
		dwExtern = memCFG.dwExtern;
	}
}
