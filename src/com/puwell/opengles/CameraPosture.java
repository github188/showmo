package com.puwell.opengles;

public class CameraPosture {
	public boolean bEnable;

	public float fViewAngle;
	public float fMinDistance;
	public float fMaxDistance;

	public float fPan;
	public float fTilt;
	public float fRotate;

	public float fPosiX;
	public float fPosiY;
	public float fPosiZ;

	public CameraPosture() {

	}

	public CameraPosture(CameraPosture cameraPosture) {
		bEnable = cameraPosture.bEnable;
		fViewAngle = cameraPosture.fViewAngle;
		fMaxDistance = cameraPosture.fMaxDistance;
		fMinDistance = cameraPosture.fMinDistance;
		fPan = cameraPosture.fPan;
		fTilt = cameraPosture.fTilt;
		fRotate = cameraPosture.fRotate;
		fPosiX = cameraPosture.fPosiX;
		fPosiY = cameraPosture.fPosiY;
		fPosiZ = cameraPosture.fPosiZ;
	}

	public CameraPosture(boolean Enable, float ViewAngle, float MinDistance, float MaxDistance, float Pan, float Tilt, float Rotate, float PosiX, float PosiY, float PosiZ) {
		bEnable = Enable;
		fViewAngle = ViewAngle;
		fMaxDistance = MaxDistance;
		fMinDistance = MinDistance;
		fPan = Pan;
		fTilt = Tilt;
		fRotate = Rotate;
		fPosiX = PosiX;
		fPosiY = PosiY;
		fPosiZ = PosiZ;
	}
}
