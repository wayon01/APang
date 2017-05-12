using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using SerializableCollections;
using UnityEngine;

public class StageMgr : MonoBehaviour {
    public GameObject TileManager;
    public GameObject MainCamera;
    private TileMgr tileMgr;

    private int m_currentStage;
    private Dictionary<int, StringBuilder> m_stageString;

    private bool isStageMoved;
    private bool isStageMoving;
    private bool isAnimationUpdating;
    private Vector3 cameraPos;
    private Vector3 playerPos;

    private APlayer player;
    private float PortalTime;

    // Use this for initialization
    void Start () {
        //m_stageString = new Dictionary<int, StringReader>();
        tileMgr = TileManager.GetComponent<TileMgr>();
        player = GameObject.Find("player").GetComponent<APlayer>();

        Init();
    }

    public void Init() {
        m_currentStage = 0;
        isStageMoved = false;
        isStageMoving = false;
        isAnimationUpdating = false;
        cameraPos = Vector3.zero;
        playerPos = -Vector3.one;
    }
	
	// Update is called once per frame
	void Update () {
        if (!tileMgr.isLoad) return;

	    if (isAnimationUpdating) {
            if(PortalTime == 0)
                player.OnTakePortal();

            PortalTime += Time.deltaTime;

	        if (PortalTime >= 1) {
	            isAnimationUpdating = false;
	            PortalTime = 0;
	        }

            return;
	    }

        if (isStageMoved) {

	        if (MainCamera.transform.localPosition.y > 15.9f) {
	            cameraPos.y = 0;
                tileMgr.SettingMap(m_currentStage);
                isStageMoved = false;
	            if (playerPos != -Vector3.one) {
	                SetPlayerPosition();
                    tileMgr.CameraManager.GetComponent<CameraMgr>().ResetRotation();
	                playerPos = -Vector3.one;
	            }
	        }else
	            cameraPos.y = 16;
	    }

	    if (isStageMoving) {
	        MainCamera.transform.localPosition = Vector3.LerpUnclamped(MainCamera.transform.localPosition, cameraPos, 0.1f);

	        if (!isStageMoved &&
                Math.Abs(Math.Abs(MainCamera.transform.localPosition.y) - Math.Abs(cameraPos.y)) < 0.1f) {
                cameraPos.y = 0;
                MainCamera.transform.localPosition = cameraPos;
                isStageMoving = false;
	            isAnimationUpdating = true;
	        }
	    }
	}

    public int GetCurrentStageId() {
        return m_currentStage;
    }

    public void SetCurrentStageId(int id) {
        m_currentStage = id;
    }

    public void AddStageString(StringBuilder str, int stageId) {
        if (string.IsNullOrEmpty(str.ToString())) return;
        if (m_stageString == null) m_stageString = new Dictionary<int, StringBuilder>();
        m_stageString.Add(stageId, str);
    }

    public StringBuilder GetStageStringReader(int index) {
        return (StringBuilder) m_stageString[index];
    }

    public void SetStageChanged(int stageId) {
        if (stageId == -1) return;
        if (stageId == -2) {
            tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isCleared = false;
            tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isFailed = false;
            return;
        }
        m_currentStage = stageId;
        isStageMoved = true;
        isStageMoving = true;
        isAnimationUpdating = true;
        //tileMgr.SettingMap(stageId);
        cameraPos = MainCamera.transform.localPosition;
        tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isCleared = false;
    }

    public void SetPlayerId(Vector3 id) {
        playerPos = id;
    }

    private void SetPlayerPosition() {
        AActor player = GameObject.Find("player").GetComponent<AActor>();

        player.Position = tileMgr.GetTile(playerPos).GetComponent<TileObject>().Position + new Vector3(0, 0.5f, 0);
        player.positionId = playerPos;
        player.resetPosition();
    }
}
