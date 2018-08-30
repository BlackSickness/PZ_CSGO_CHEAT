#pragma once
 
class CPredictionSystem {
public:
	void StartPrediction();
	void EndPrediction();
 
private:
	float m_flOldCurtime;
	float m_flOldFrametime;
	CMoveData m_MoveData;
 
	int* m_pPredictionRandomSeed;
};