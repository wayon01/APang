using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using SerializableCollections;
using UnityEngine;

public class StageMgr : MonoBehaviour {
    public GameObject TileManager;
    private TileMgr tileMgr;

    private int m_currentStage;
    private Dictionary<int, StringReader> m_stageString;

	// Use this for initialization
	void Start () {
	    m_currentStage = 0;
        //m_stageString = new Dictionary<int, StringReader>();
        tileMgr = TileManager.GetComponent<TileMgr>();
	}
	
	// Update is called once per frame
	void Update () {
	    if (m_stageString.Count == 0) {
	        Debug.Log("fuck");
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
        tileMgr.SettingMap(stageId);
        tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isCleared = false;
    }
}
