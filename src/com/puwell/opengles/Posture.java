package com.puwell.opengles;

public class Posture {
	public boolean bNeedRotate;
	public boolean bNeedTranslate;
	public boolean bNeedScale;
	public boolean byExtern;
	public float fRotate[];
	public float fTranslate[];
	public float fScale[];
	public float fRotate_X;
	public float fRotate_Y;
	public float fRotate_Z;
	public float fTranslate_X;
	public float fTranslate_Y;
	public float fTranslate_Z;
	public float fScale_X;
	public float fScale_Y;
	public float fScale_Z;
	public Posture() {
//		fRotate = new float[3];
//		fTranslate = new float[3];
//		fScale = new float[3];
//		bNeedRotate = 0;
//		bNeedScale = 0;
//		bNeedTranslate = 0;
//		byExtern = 0;
	}
	
	public Posture(Posture posture){
		bNeedRotate = posture.bNeedRotate;
		bNeedScale = posture.bNeedScale;
		bNeedTranslate = posture.bNeedTranslate;
		fRotate = new float[posture.fRotate.length];
		System.arraycopy(posture.fRotate, 0, fRotate, 0, posture.fRotate.length);
		fTranslate = new float[posture.fTranslate.length];
		System.arraycopy(posture.fTranslate, 0, fTranslate, 0, posture.fTranslate.length);
		fScale = new float[posture.fScale.length];
		System.arraycopy(posture.fScale, 0, fScale, 0, posture.fScale.length);
		byExtern = posture.byExtern;
	}
	
	public Posture(boolean needRotate, float[] rotate, boolean needTranslate, float[] translate, boolean needScale, float[] scale, boolean extern){
		bNeedRotate = needRotate;
		bNeedScale = needScale;
		bNeedTranslate = needTranslate;
		fRotate = new float[rotate.length];
		System.arraycopy(rotate, 0, fRotate, 0, rotate.length);
		fTranslate = new float[translate.length];
		System.arraycopy(translate, 0, fTranslate, 0, translate.length);
		fScale = new float[scale.length];
		System.arraycopy(scale, 0, fScale, 0, scale.length);
		byExtern = extern;
	}
}
