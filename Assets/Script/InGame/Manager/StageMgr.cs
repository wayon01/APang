using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using SerializableCollections;
using UnityEngine;

public class StageMgr : MonoBehaviour {
    public GameObject TileManager;
    public GameObject MainCamera;
    private TileMgr tileMgr;

    private int m_currentStage;
    private Dictionary<int, StringReader> m_stageString;

    private bool isStageMoved;
    private bool isStageMoving;
    private Vector3 cameraPos;

    // Use this for initialization
    void Start () {
	    m_currentStage = 0;
        //m_stageString = new Dictionary<int, StringReader>();
        tileMgr = TileManager.GetComponent<TileMgr>();
	    isStageMoved = false;
        isStageMoving = false;
        cameraPos = Vector3.zero;
    }
	
	// Update is called once per frame
	void Update () {

        if (isStageMoved) {

	        if (MainCamera.transform.localPosition.x < -15.9f) {
	            cameraPos.x = 0;
                tileMgr.SettingMap(m_currentStage);
                isStageMoved = false;
	        }else
	            cameraPos.x = -16;
	    }

	    if (isStageMoving) {
	        MainCamera.transform.localPosition = Vector3.LerpUnclamped(MainCamera.transform.localPosition, cameraPos, 0.1f);

	        if (!isStageMoved &&
                Math.Abs(Math.Abs(MainCamera.transform.localPosition.x) - Math.Abs(cameraPos.x)) < 0.1f) {
                cameraPos.x = 0;
                MainCamera.transform.localPosition = cameraPos;
                isStageMoving = false;
	        }
	    }
	}

    public int GetCurrentStageId() {
        return m_currentStage;
    }

    public void SetCurrentStageId(int id) {
        m_currentStage = id;
    }

    public void AddStageString(string str, int stageId) {
        if (string.IsNullOrEmpty(str)) return;
        if (m_stageString == null) m_stageString = new Dictionary<int, StringReader>();
        m_stageString.Add(stageId, new StringReader(str));
    }

    public StringReader GetStageStringReader(int index) {
        return (StringReader) m_stageString[index];
    }

    public void SetStageChanged(int stageId) {
        if (stageId == -1) return;
        m_currentStage = stageId;
        isStageMoved = true;
        isStageMoving = true;
        //tileMgr.SettingMap(stageId);
        cameraPos = MainCamera.transform.localPosition;
        tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isCleared = false;
    }
}
