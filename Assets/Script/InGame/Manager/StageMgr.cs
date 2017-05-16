using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StageMgr : MonoBehaviour {
    private Vector3 cameraPos;
    private bool isAnimationUpdating;
    private bool isSameStage;

    private bool isStageMoved;
    private bool isStageMoving;
    private Vector3 localCameraPos;

    private int m_currentStage;
    private Dictionary<int, ArrayList> m_stageString;
    public GameObject MainCamera;

    private APlayer player;
    private Vector3 playerPos;
    private float PortalTime;
    public GameObject TileManager;
    private TileMgr tileMgr;

    // Use this for initialization
    private void Start() {
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
        isSameStage = false;
        cameraPos = Vector3.zero;
        playerPos = -Vector3.one;
    }

    // Update is called once per frame
    private void Update() {
        if (!tileMgr.isLoad) return;

        if (isAnimationUpdating) {
            if (PortalTime == 0)
                player.OnTakePortal();

            PortalTime += Time.deltaTime;

            if (PortalTime >= 1) {
                isAnimationUpdating = false;
                PortalTime = 0;
            }

            return;
        }

        if (isStageMoved)
            if (localCameraPos.y > 15.9f) {
                cameraPos.y = 0;
                tileMgr.SettingMap(m_currentStage);
                isStageMoved = false;
                if (playerPos != -Vector3.one) {
                    SetPlayerPosition();
                    tileMgr.CameraManager.GetComponent<CameraMgr>().ResetRotation();
                    playerPos = -Vector3.one;
                }
            }
            else
                cameraPos.y = 16;

        if (isStageMoving) {
            localCameraPos = Vector3.LerpUnclamped(localCameraPos, cameraPos, 0.1f);
            if (!isSameStage)
                MainCamera.transform.localPosition = localCameraPos;

            if (!isStageMoved &&
                (Math.Abs(Math.Abs(localCameraPos.y) - Math.Abs(cameraPos.y)) < 0.1f)) {
                cameraPos.y = 0;
                localCameraPos = cameraPos;
                isStageMoving = false;
                isAnimationUpdating = true;
                isSameStage = false;
            }
        }
    }

    public int GetCurrentStageId() {
        return m_currentStage;
    }

    public void SetCurrentStageId(int id) {
        m_currentStage = id;
    }

    public void AddStageString(ArrayList list, int stageId) {
        if (list.Count == 0) return;
        if (m_stageString == null) m_stageString = new Dictionary<int, ArrayList>();
        m_stageString.Add(stageId, list);
    }

    public ArrayList GetStageStringReader(int index) {
        return m_stageString[index];
    }

    public void SetStageChanged(int stageId) {
        if (stageId == -1) return;
        if (stageId == -2) {
            tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isPortalArrived = false;
            tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isFailed = false;
            return;
        }
        if (m_currentStage == stageId)
            isSameStage = true;
        m_currentStage = stageId;
        isStageMoved = true;
        isStageMoving = true;
        isAnimationUpdating = true;
        //tileMgr.SettingMap(stageId);
        cameraPos = localCameraPos = MainCamera.transform.localPosition;
        tileMgr.GameSystemManager.GetComponent<GameSystemMgr>().isPortalArrived = false;
    }

    public void SetPlayerId(Vector3 id) {
        playerPos = id;
    }

    public void Destroy() {
    }

    private void SetPlayerPosition() {
        var player = GameObject.Find("player").GetComponent<AActor>();

        player.Position = tileMgr.GetTile(playerPos).GetComponent<TileObject>().Position + new Vector3(0, 0.5f, 0);
        player.positionId = playerPos;
        player.resetPosition();
    }
}