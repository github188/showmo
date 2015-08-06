package com.puwell.opengles;

import android.util.Log;

public class PatternMem {
	private final String TAG="PatternMem";
	public int ePatternType; // VTXType
	public int bNeedRotate;
	public int bNeedTranslate;
	public int bNeedScale;
	public int byExtern;
	public float fRotate_X; 
	public float fRotate_Y;
	public float fRotate_Z; 
	public float fTranslate_X;
	public float fTranslate_Y;
	public float fTranslate_Z;
	public float fScale_X;
	public float fScale_Y;
	public float fScale_Z;
	
	public char cTextureName;
	public int dwPointCountRow;
	public int dwPointCountCol;
	public int dwPointCountLay;

	public float[] pfVertexList_3; // ... Xi, Yi, Zi ...
	public float[] pfColorList_4; // ... Ri, Gi, Bi, Ai ...
	public float[] pfTexCordList_2; // ... Pi, Ti ...
	
	public PatternMem() {

	}
	
	public PatternMem(int type, char name, int Row, int Col, int Lay) {
		//stCfgMember = new MemCFG(memCFG);
		Log.i(TAG, "patternMem dwPointCounttype/Row/Col/Lay:"+type+"--"+ Row + "--" + Col + "--"+ Lay);		
		//Log.i(TAG,"VertexListlength"+VertexList.length);
		//Log.i(TAG,"CordListlength"+CordList.length);
		
		
		ePatternType = type;
		cTextureName = name;
		dwPointCountCol = Lay;
		dwPointCountLay = Col;
		dwPointCountRow = Row;
		pfVertexList_3 = new float[Lay * Col * Row * 3];
//		System.arraycopy( VertexList, 0, pfVertexList_3, 0, VertexList.length );
		pfTexCordList_2 = new float[Lay * Col * Row * 2];
//		System.arraycopy( CordList, 0, pfTexCordList_2, 0, CordList.length );
	}
	

	public PatternMem(PatternMem patternMem) {
		ePatternType = patternMem.ePatternType;
	
		dwPointCountCol = patternMem.dwPointCountCol;
		dwPointCountLay = patternMem.dwPointCountLay;
		dwPointCountRow = patternMem.dwPointCountRow;
		pfVertexList_3 = new float[patternMem.pfVertexList_3.length];
		System.arraycopy(patternMem.pfVertexList_3, 0, pfVertexList_3, 0, patternMem.pfVertexList_3.length);
		pfTexCordList_2 = new float[patternMem.pfTexCordList_2.length];
		System.arraycopy(patternMem.pfTexCordList_2, 0, pfTexCordList_2, 0, patternMem.pfTexCordList_2.length);

	}

}
