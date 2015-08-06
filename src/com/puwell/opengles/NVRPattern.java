package com.puwell.opengles;

import com.showmo.util.LogUtils;

import android.R.integer;
import android.util.Log;

public class NVRPattern {

/*-- Add by lgy --*/	
	public float[]			m_pfBackColor;
	public CameraPosture	m_clCamState;
	public Posture			m_clWorldPosture;	
	public int				m_nGroupCount;
	public int				m_nMemTotal;
	public int 				m_nVertexTotal;
	
	public int[]			m_plGroupPriorityList;
	public CameraPosture[]	m_pclGroupCamStateList;
	public Posture[]		m_pclGroupPostureList;	
	
	public int[]			m_nGroupMemCountList;
	public int[]			m_nGroupMemOffSetList;
	public int[]			m_nGroupVerOffSetList;	
	public float[]			m_fGroupVer2OriList;	
	
	public Posture[]		m_pclMemberPostureList;	
	public int[]			m_pnPointCountRowList;
	public int[]			m_pnPointCountColList;
	public int[]			m_pnPointCountLayList;
	
	public int[]			m_pnMemberVerOffsetList;

	public float[]			m_pfVertexList_3; // ... Xi, Yi, Zi ...
	public float[]			m_pfTexCordList_2; // ... Pi, Ti ...	
	public float[]			m_pfVertexOriList_3; // ... Xi, Yi, Zi ...
	
	public int[]			m_plBoardRotatePri_3;
	public int[]			m_plBoardTransPri_3;
	public int[]			m_plBoardScalePri_3;
	public float[]			m_fBoardRotate_3;
	public float[]			m_fBoardTrans_3;
	public float[]			m_fBoardScale_3; 
	
	public float[]			m_pfBoardVertexList_3; // ... Xi, Yi, Zi ...
	public float[]			m_pfBoardTexCordList_2; // ... Pi, Ti ...	
	
	private final String TAG="NVRPattern";
	
	public NVRPattern() {
		m_pfBackColor				= new float[4];
		m_clCamState                = new CameraPosture();
		m_clWorldPosture            = new Posture();
		m_nGroupCount				= -1;
		m_nMemTotal					= -1;
		m_nVertexTotal				= -1;
	}
	
	public void Reset( int nGroupCount, int nMemTotal, int nVertexTotal ) {
		int i;
		//Log.i(TAG, "nGroupCount:"+nGroupCount+"--nMemTotal:"+nMemTotal+"--nVertexTotal:"+nVertexTotal);
		
		if( nGroupCount > 0 && m_nGroupCount != nGroupCount ){	
			m_plGroupPriorityList		= new int[nGroupCount];	
			m_pclGroupCamStateList		= new CameraPosture[nGroupCount];		
			m_pclGroupPostureList		= new Posture[nGroupCount];
		
			for( i=0; i<nGroupCount; i++ ){
				m_pclGroupCamStateList[i] = new CameraPosture();
				m_pclGroupPostureList[i] = new Posture();
			}			
			m_nGroupMemCountList		= new int[nGroupCount];
			m_nGroupMemOffSetList		= new int[nGroupCount];
			m_nGroupVerOffSetList		= new int[nGroupCount];
			m_fGroupVer2OriList			= new float[nGroupCount];
			m_nGroupCount				= nGroupCount;
		}

		if( nMemTotal > 0 && m_nMemTotal != nMemTotal ){	
			m_pclMemberPostureList		= new Posture[nMemTotal];
			
			for( i=0; i<nMemTotal; i++ ){			
				m_pclMemberPostureList[i] = new Posture();
			}
			
			m_pnPointCountRowList		= new int[nMemTotal];
			m_pnPointCountColList		= new int[nMemTotal];
			m_pnPointCountLayList		= new int[nMemTotal];	
			m_pnMemberVerOffsetList		= new int[nMemTotal];	
			m_nMemTotal = nMemTotal;
		}
		if( nVertexTotal > 0 && m_nVertexTotal != nVertexTotal ){
			m_pfVertexList_3			= new float[nVertexTotal * 3];
			m_pfVertexOriList_3			= new float[nVertexTotal * 3];
			m_pfTexCordList_2			= new float[nVertexTotal * 2];
			m_nVertexTotal = nVertexTotal;
		}		
		
		m_plBoardRotatePri_3		= new int[3];
		m_plBoardTransPri_3			= new int[3];
		m_plBoardScalePri_3			= new int[3];
		m_fBoardRotate_3			= new float[3];
		m_fBoardTrans_3				= new float[3];
		m_fBoardScale_3				= new float[3];
	
		m_pfBoardVertexList_3		= new float[18];
		m_pfBoardTexCordList_2		= new float[12];
	}
	public void SetPriority( int nGroupNo, int nPriority ) {
		//LogUtils.v("_gl_Draw", "SetPriority nGroupNo"+nGroupNo+" "+nPriority);
		m_plGroupPriorityList[nGroupNo] = nPriority;
	}
	public void SetBackColor( float fRed, float fGreen, float fBlue, float fAlpha ) {
		//Log.i(TAG, "nRed:"+fRed+"--nGreen:"+fGreen+"--nBlue:"+fBlue+"--nAlpha:"+fAlpha);
		m_pfBackColor[0] = fRed;
		m_pfBackColor[1] = fGreen;
		m_pfBackColor[2] = fBlue;
		m_pfBackColor[3] = fAlpha;
	}
	
	public void SetData( int nVertexTotal, float[] pfVertexList, float[] pfTextureList ) {
		if( m_nVertexTotal == nVertexTotal ){
			//Log.i(TAG, "m_nVertexTotal:"+m_nVertexTotal);
			if( nVertexTotal * 3 == pfVertexList.length &&
				nVertexTotal * 2 == pfTextureList.length ){
				
				System.arraycopy( pfVertexList, 0, m_pfVertexList_3, 0, pfVertexList.length );
				System.arraycopy( pfTextureList, 0, m_pfTexCordList_2, 0, pfTextureList.length );
				
			}			
		}
	}
	
	public void SetData_2( int nVertexTotal, float[] pfVertexList, float[] pfVertexOriList, float[] pfTextureList ) {
		if( m_nVertexTotal == nVertexTotal ){
			//Log.i(TAG, "m_nVertexTotal:"+m_nVertexTotal);
			if( nVertexTotal * 3 == pfVertexList.length &&
				nVertexTotal * 3 == pfVertexOriList.length &&
				nVertexTotal * 2 == pfTextureList.length ){				
				
				System.arraycopy( pfVertexList, 0, m_pfVertexList_3, 0, pfVertexList.length );
				System.arraycopy( pfVertexOriList, 0, m_pfVertexOriList_3, 0, pfVertexOriList.length );
				System.arraycopy( pfTextureList, 0, m_pfTexCordList_2, 0, pfTextureList.length );
			}			
		}
	}
	
	public void SetData_Board( float[] pfVertexList, float[] pfTextureList ) {
		System.arraycopy( pfVertexList, 0, m_pfBoardVertexList_3, 0, 18 );
//		LogUtils.e("gltest", "m_pfBoardVertexList_3 "+m_pfBoardVertexList_3[0]+" , "+m_pfBoardVertexList_3[1]+" , "+m_pfBoardVertexList_3[2]
//				+" , "+m_pfBoardVertexList_3[3]+" , "+m_pfBoardVertexList_3[4]+" , "+m_pfBoardVertexList_3[5]);
		System.arraycopy( pfTextureList, 0, m_pfBoardTexCordList_2, 0, 12 );
//		LogUtils.e("gltest", "m_pfBoardTexCordList_2 "+m_pfBoardTexCordList_2[0]+" , "+m_pfBoardTexCordList_2[1]+" , "+m_pfBoardTexCordList_2[2]
//				+" , "+m_pfBoardTexCordList_2[3]+" , "+m_pfBoardTexCordList_2[4]+" , "+m_pfBoardTexCordList_2[5]);
	}
	
	public void SetRotate_Board( int iPriX, int iPriY, int iPriZ, float pfVX, float pfVY, float pfVZ ) {
		m_plBoardRotatePri_3[0] = iPriX;
		m_plBoardRotatePri_3[1] = iPriY;
		m_plBoardRotatePri_3[2] = iPriZ;
		m_fBoardRotate_3[0] = pfVX;
		m_fBoardRotate_3[1] = pfVY;
		m_fBoardRotate_3[2] = pfVZ;		
	}
	public void SetTrans_Board( int iPriX, int iPriY, int iPriZ, float pfVX, float pfVY, float pfVZ ) {
		m_plBoardTransPri_3[0] = iPriX;
		m_plBoardTransPri_3[1] = iPriY;
		m_plBoardTransPri_3[2] = iPriZ;
		m_fBoardTrans_3[0] = pfVX;
		m_fBoardTrans_3[1] = pfVY;
		m_fBoardTrans_3[2] = pfVZ;				
	}
	public void SetScale_Board( int iPriX, int iPriY, int iPriZ, float pfVX, float pfVY, float pfVZ ) {
		m_plBoardScalePri_3[0] = iPriX;
		m_plBoardScalePri_3[1] = iPriY;
		m_plBoardScalePri_3[2] = iPriZ;
		m_fBoardScale_3[0] = pfVX;
		m_fBoardScale_3[1] = pfVY;
		m_fBoardScale_3[2] = pfVZ;		
	}
	
	public void SetCam( int nGroupNo, boolean bEnable,
						float fViewAngle, float fMinDistance, float fMaxDistance,
						float fPan, float fTilt, float fRotate,
						float fPosiX, float fPosiY, float fPosiZ
			) {
		//Log.i(TAG, "SetCam nGroupNo:"+nGroupNo);
//		Log.i(TAG, "SetCam bEnable:"+bEnable+"--fViewAngle:"+fViewAngle+"--fMinDistance:"+fMinDistance
//				+"--fMaxDistance:"+fMaxDistance+"--fPan:"+fPan+"--fTilt:"+fTilt
//				+"--fRotate:"+fRotate+"--fPosiX:"+fPosiX+"--fPosiY:"+fPosiY
//				+"--fPosiZ:"+fPosiZ);
		if( nGroupNo < 0 ){			
			m_clCamState.bEnable = bEnable;
			m_clCamState.fViewAngle = fViewAngle;
			m_clCamState.fMinDistance = fMinDistance;
			m_clCamState.fMaxDistance = fMaxDistance;
			m_clCamState.fPan = fPan;
			m_clCamState.fTilt = fTilt;
			m_clCamState.fRotate = fRotate;
			m_clCamState.fPosiX = fPosiX;
			m_clCamState.fPosiY = fPosiY;
			m_clCamState.fPosiZ = fPosiZ;		
		}
		else if( nGroupNo >= 0 && nGroupNo < m_nGroupCount ){
			int nGroupMemOffset = m_nGroupMemOffSetList[nGroupNo];
			//Log.i(TAG, "SetCam nGroupMemOffset:"+nGroupMemOffset);
			m_pclGroupCamStateList[nGroupNo].bEnable = bEnable;
			m_pclGroupCamStateList[nGroupNo].fViewAngle = fViewAngle;
			m_pclGroupCamStateList[nGroupNo].fMinDistance = fMinDistance;
			m_pclGroupCamStateList[nGroupNo].fMaxDistance = fMaxDistance;
			m_pclGroupCamStateList[nGroupNo].fPan = fPan;
			m_pclGroupCamStateList[nGroupNo].fTilt = fTilt;
			m_pclGroupCamStateList[nGroupNo].fRotate = fRotate;
			m_pclGroupCamStateList[nGroupNo].fPosiX = fPosiX;
			m_pclGroupCamStateList[nGroupNo].fPosiY = fPosiY;
			m_pclGroupCamStateList[nGroupNo].fPosiZ = fPosiZ;				
		}
	}
	
	public void SetPosture( int nGroupNo, int nMemberNo, 
			boolean bNeedRotate, float fRotate_X, float fRotate_Y, float fRotate_Z,
			boolean bNeedTranslate, float fTranslate_X, float fTranslate_Y, float fTranslate_Z,
			boolean bNeedScale, float fScale_X, float fScale_Y, float fScale_Z
			) {
//		Log.i(TAG, "SetPosture nGroupNo:"+nGroupNo+"SetPosture nMemberNo:"+nMemberNo);
//		Log.i(TAG, "SetPosture bNeedRotate:"+bNeedRotate+"--fRotate_X:"+fRotate_X+"--fRotate_Y:"+fRotate_Y+"--fRotate_Z:"+fRotate_Z+
//				"--bNeedTranslate:"+bNeedTranslate+"--fTranslate_X:"+fTranslate_X+"--fTranslate_Y:"+fTranslate_Y+"--fTranslate_Z:"+fTranslate_Z+
//				"--bNeedScale:"+bNeedScale+"--fScale_X:"+fScale_X+"--fScale_Y:"+fScale_Y+"--fScale_Z:"+fScale_Z);
		if( nGroupNo < 0 ){			
			m_clWorldPosture.bNeedRotate = bNeedRotate;
			m_clWorldPosture.fRotate_X = fRotate_X;
			m_clWorldPosture.fRotate_Y = fRotate_Y;
			m_clWorldPosture.fRotate_Z = fRotate_Z;

			m_clWorldPosture.bNeedTranslate = bNeedTranslate;
			m_clWorldPosture.fTranslate_X = fTranslate_X;
			m_clWorldPosture.fTranslate_Y = fTranslate_Y;
			m_clWorldPosture.fTranslate_Z = fTranslate_Z;

			m_clWorldPosture.bNeedScale = bNeedScale;
			m_clWorldPosture.fScale_X = fScale_X;
			m_clWorldPosture.fScale_Y = fScale_Y;
			m_clWorldPosture.fScale_Z = fScale_Z;
		}
		else if( nGroupNo >= 0 && nGroupNo < m_nGroupCount){
			int nGroupMemOffset = m_nGroupMemOffSetList[nGroupNo];
			Log.i(TAG, "SetPosture nGroupMemOffset:"+nGroupMemOffset);
			if( nMemberNo < 0 ) {
				m_pclGroupPostureList[nGroupNo].bNeedRotate = bNeedRotate;
				m_pclGroupPostureList[nGroupNo].fRotate_X = fRotate_X;
				m_pclGroupPostureList[nGroupNo].fRotate_Y = fRotate_Y;
				m_pclGroupPostureList[nGroupNo].fRotate_Z = fRotate_Z;

				m_pclGroupPostureList[nGroupNo].bNeedTranslate = bNeedTranslate;
				m_pclGroupPostureList[nGroupNo].fTranslate_X = fTranslate_X;
				m_pclGroupPostureList[nGroupNo].fTranslate_Y = fTranslate_Y;
				m_pclGroupPostureList[nGroupNo].fTranslate_Z = fTranslate_Z;

				m_pclGroupPostureList[nGroupNo].bNeedScale = bNeedScale;
				m_pclGroupPostureList[nGroupNo].fScale_X = fScale_X;
				m_pclGroupPostureList[nGroupNo].fScale_Y = fScale_Y;
				m_pclGroupPostureList[nGroupNo].fScale_Z = fScale_Z;
			}
			if( nMemberNo >=0 && nMemberNo < m_nGroupMemCountList[nGroupNo] ) {
				nGroupMemOffset += nMemberNo;
				Log.i(TAG, "SetPosture nGroupMemOffset:"+nGroupMemOffset);
				m_pclMemberPostureList[nGroupMemOffset].bNeedRotate = bNeedRotate;
				m_pclMemberPostureList[nGroupMemOffset].fRotate_X = fRotate_X;
				m_pclMemberPostureList[nGroupMemOffset].fRotate_Y = fRotate_Y;
				m_pclMemberPostureList[nGroupMemOffset].fRotate_Z = fRotate_Z;

				m_pclMemberPostureList[nGroupMemOffset].bNeedTranslate = bNeedTranslate;
				m_pclMemberPostureList[nGroupMemOffset].fTranslate_X = fTranslate_X;
				m_pclMemberPostureList[nGroupMemOffset].fTranslate_Y = fTranslate_Y;
				m_pclMemberPostureList[nGroupMemOffset].fTranslate_Z = fTranslate_Z;

				m_pclMemberPostureList[nGroupMemOffset].bNeedScale = bNeedScale;
				m_pclMemberPostureList[nGroupMemOffset].fScale_X = fScale_X;
				m_pclMemberPostureList[nGroupMemOffset].fScale_Y = fScale_Y;
				m_pclMemberPostureList[nGroupMemOffset].fScale_Z = fScale_Z;
			}
		}
	}
	
	public void SetGroupFlag( int nGroupNo, int nMemberCount, int nMemberOffset, int VerOffset ) {
		m_nGroupMemCountList[nGroupNo] = nMemberCount;
		m_nGroupMemOffSetList[nGroupNo] = nMemberOffset;
		m_nGroupVerOffSetList[nGroupNo] = VerOffset;
	}
	
	public void SetGroupVer2Ori( int nGroupNo, float fVer2Ori ) {
		m_fGroupVer2OriList[nGroupNo] = fVer2Ori;
	}

	public void SetMemFlag( int nMemberOffset, int nCountRow, int nCountCol, int nCountLay, int VerOffset ) {
		m_pnPointCountRowList[nMemberOffset] = nCountRow;
		m_pnPointCountColList[nMemberOffset] = nCountCol;
		m_pnPointCountLayList[nMemberOffset] = nCountLay;
		m_pnMemberVerOffsetList[nMemberOffset] = VerOffset;
	}
	
}
