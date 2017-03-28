using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameSystemMgr : MonoBehaviour {

    public GameObject PlayerObject;
    public GameObject TileManager;

    public bool isCleared;

    private TileMgr tileMgr;
    private CameraMgr cameraMgr;
    private APlayer Player;

    public bool isPlayerMoving;
    public bool isPlayerMovingUp;

    public int m_playerMovingCount;

    private Vector3 selectedTilePosition;
    private Vector3 prevPlayerPosition;
    private float prevPlayerStartTime;

    private Vector3 mapLength;

	// Use this for initialization
	void Start () {
	    tileMgr = TileManager.GetComponent<TileMgr>();
	    Player = PlayerObject.GetComponent<APlayer>();
	    cameraMgr = tileMgr.CameraManager.GetComponent<CameraMgr>();
        mapLength = Vector3.zero;


	    isCleared = false;
	    m_playerMovingCount = 0;
	}
	
	// Update is called once per frame
	void Update () {
	    if (isPlayerMoving) {
            OnPlayerMove();
	        return;
	    }
        if (tileMgr.isClicked) {
            OnSelected();
	        tileMgr.isClicked = false;
	    }
	}

    void LateUpdate() {
        if (isPlayerMovingUp) {
            isPlayerMovingUp = false;

        }

        if (prevPlayerStartTime >= 1) {
            prevPlayerStartTime = 0;
            isPlayerMovingUp = true;
            isPlayerMoving = false;
        }
    }

    private void OnSelected() {
        if (Player.isBlocked) return;
        if (tileMgr.GetClickedTileVector3() == -Vector3.one) return;
        if (Player.positionId == tileMgr.GetClickedTileVector3()) return;

        if (!OnJudgePlayerMovable(tileMgr.GetClickedTileVector3())) {
            return;
        }

        m_playerMovingCount++;

        selectedTilePosition = tileMgr.GetClickedTile().transform.position + new Vector3(0, 0.5f, 0);
        Player.positionId = tileMgr.GetClickedTileVector3();
        Player.Position = Id2Position(Player.positionId) + new Vector3(0, 0.5f, 0);
        prevPlayerPosition = Player.transform.position;
        prevPlayerStartTime = 0;
        isPlayerMoving = true;

        Text textObj = GameObject.Find("PlayerMovngCountText").GetComponent<Text>();
        textObj.text = "이동횟수 : " + m_playerMovingCount;

    }

    private bool OnJudgePlayerMovable(Vector3 tileId) {
        Vector3 clickedTileId = tileId;
        int rotationY = (int) cameraMgr.FinalRotationY;
        int heightLevel = (int) (clickedTileId.y - Player.positionId.y);
        bool isPositionX = Math.Abs(rotationY) != 90;

        bool isFront = false;

        switch (rotationY) {
            case 0:
            case 90:
                isFront = true;
                break;
            case 180:
            case -90:
                isFront = false;
                break;
        }

        if (mapLength == Vector3.zero) {
            mapLength = tileMgr.GetIdLength();
        }
        

        //클릭한 타일 위에 다른 타일이 있을 경우 예외처리
        if (clickedTileId.y < mapLength.y - 1 && tileMgr.GetSurfaceTile(isPositionX, isFront, clickedTileId + new Vector3(0, 1, 0)) != null) {
            return false;
        }


        //플레이어 위
        if (heightLevel == 1) {
            Vector3 tempTile = Player.positionId + new Vector3(0, 2, 0);
            //플레이어 바로 위에 천장이 없을 경우
            if (tempTile.y > mapLength.y - 1 || tileMgr.GetSurfaceTile(isPositionX, isFront, tempTile) == null) {

                //플레이어가 벽에 막혀있는데 올라가려고 할 경우
                if (tileMgr.GetSurfaceTile(isPositionX, isFront, Player.positionId + new Vector3(0, 1, 0)) != null) {
                    return false;
                }

                //x축 기준 조건이 맞는 경우
                if (isPositionX && Math.Abs(clickedTileId.x - Player.positionId.x) <= 1) {
                    return true;
                }
                //z축 기준 조건이 맞는 경우
                else if (!isPositionX && Math.Abs(clickedTileId.z - Player.positionId.z) <= 1) {
                    return true;
                }
                //조건에 만족하지 않는 경우
                else {
                    return false;
                }
            }
        //플레이어 중심
        }else if (heightLevel == 0) {

            float gap = Math.Abs(clickedTileId.x - Player.positionId.x);

            //x축 기준 조건이 맞는 경우
            if (isPositionX && gap <= 1 && gap > 0) {
                return true;
            }

            gap = Math.Abs(clickedTileId.z - Player.positionId.z);
            //z축 기준 조건이 맞는 경우
            if (!isPositionX && gap <= 1 && gap > 0) {
                return true;
            }
            //조건에 만족하지 않는 경우
            else {
                return false;
            }
        }
        //플레이어 아래
        else if(heightLevel == -1) {
            Vector3 tempTile = clickedTileId + new Vector3(0, 2, 0);
            //플레이어 옆에 있는 장애물이 존재하지 않을 경우
            if (tempTile.y > mapLength.y - 1 || tileMgr.GetSurfaceTile(isPositionX, isFront, tempTile) == null) {
                //x축 기준 조건이 맞는 경우
                if (isPositionX && Math.Abs(clickedTileId.x - Player.positionId.x) <= 1) {
                    return true;
                }
                //z축 기준 조건이 맞는 경우
                else if (!isPositionX && Math.Abs(clickedTileId.z - Player.positionId.z) <= 1) {
                    return true;
                }
                //조건에 만족하지 않는 경우
                else {
                    return false;
                }
            }

        }
        

        return false;
    }

    private void OnPlayerMove() {
        Vector3 playerPos = prevPlayerPosition;
        prevPlayerStartTime += Time.deltaTime;

        Player.transform.position = new Vector3(Mathf.Lerp(playerPos.x, selectedTilePosition.x, prevPlayerStartTime),
            Mathf.Lerp(playerPos.y, selectedTilePosition.y, prevPlayerStartTime),
            Mathf.Lerp(playerPos.z, selectedTilePosition.z, prevPlayerStartTime));

    }

    public Vector3 Id2Position(Vector3 id) {
        Vector3 result = id - tileMgr.GetIdLength()/2f;

        return result;
    }

    public Vector3 Id2Position(int x, int y, int z) {
        Vector3 result = new Vector3(x, y, z);
        result -= tileMgr.GetIdLength()/2f;

        return result;
    }
}
