package com.puwell.opengles;

public class PatternGroup {
	public float BackColor[]; // GRBA:0~1 ; 0 == fBackColor[3] : no u
	public CameraPosture CamState;
	public Posture GroupPosture;
	public int Priority;
	public MemCFG CfgGroup;
	public char TextureName[];
	public MemTexCFG GenerateTexture;
	public int MemberCount;
	public PatternMem[] PatternMemberList;
	public Object object;

	public PatternGroup() {
	}

	public PatternGroup(float[] BackColor, CameraPosture cameraPosture, Posture posture, int priority, /*MemCFG memCFG,*/ char[] name,/* MemTexCFG memTexCFG,*/ int memCount, 
			PatternMem[] PatternMemList) {
		this.BackColor = new float[BackColor.length];
		System.arraycopy(BackColor, 0, this.BackColor, 0, BackColor.length);
		CamState = new CameraPosture(cameraPosture);
		GroupPosture = new Posture(posture);
		Priority = priority;
		//CfgGroup = new MemCFG(memCFG);
		TextureName = new char[name.length];
		System.arraycopy(name, 0, this.TextureName, 0, name.length);
		//GenerateTexture = new MemTexCFG(memTexCFG);
		MemberCount = memCount;
//		PatternMemberList = new PatternMem[PatternMemList.length];
//		for (int i = 0; i < PatternMemList.length; i++) {
//			PatternMem mem= new PatternMem(PatternMemList[i]);
//			PatternMemList[i] = mem;
//		}
//		System.arraycopy( PatternMemberList, 0, PatternMemList, 0, PatternMemList.length );
//		PatternMemberList = PatternMemList;
	}
}
