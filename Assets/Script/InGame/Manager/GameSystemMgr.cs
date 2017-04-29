using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameSystemMgr : MonoBehaviour {

    public GameObject PlayerObject;
    public GameObject TileManager;

    public bool isCleared;
    public bool isFailed;
    public bool isPause;

    private TileMgr tileMgr;
    private CameraMgr cameraMgr;
    private APlayer Player;

    public bool isPlayerMoving;
    public bool isPlayerMovingUp;

    public int m_playerMovingCount;
    public int m_nextStageId;

    public int[] m_playerMovementMaximum;

    private Vector3 selectedTilePosition;
    private Vector3 prevPlayerPosition;
    private Vector3 lerpedPosition;
    private float prevPlayerStartTime;

    private Vector3 mapLength;

    private float MovingTime;
    private Quaternion PlayerLocalRotation;

	// Use this for initialization
	void Start () {
	    tileMgr = TileManager.GetComponent<TileMgr>();
	    Player = PlayerObject.GetComponent<APlayer>();
	    PlayerLocalRotation = Player.transform.rotation;
	    cameraMgr = tileMgr.CameraManager.GetComponent<CameraMgr>();
        mapLength = Vector3.zero;

        m_playerMovementMaximum = new int[2];


	    Init();
	}

    public void Init() {
        isCleared = false;
        isFailed = false;
        isPause = false;
        m_playerMovingCount = 0;

        MovingTime = 1;

        UILabel textObj = GameObject.Find("ingame_accrue_move_count").GetComponent<UILabel>();
        textObj.text = "0";
    }
	
	// Update is called once per frame
	void Update () {
	    if (!tileMgr.isLoad) return;


	    if (isPlayerMoving) {
	        OnPlayerMove();
	        return;
	    }

        if (tileMgr.isClicked) {
            OnSelected();
	        tileMgr.isClicked = false;
	    }

	    if (isCleared) {
	        tileMgr.StageManager.GetComponent<StageMgr>().SetStageChanged(m_nextStageId);
	    }

	    if (isFailed && !Player.animator.GetBool("isArrest")) {
	        Player.animator.SetBool("isArrest", true);
	    }
	}

    void LateUpdate() {
        if (isPlayerMovingUp) {
            Player.animator.SetBool("isRun", false);
            Player.animator.SetBool("isLand", false);
            Player.animator.SetBool("isJump", false);
            isPlayerMovingUp = false;

        }

        if (prevPlayerStartTime >= MovingTime) {
            prevPlayerStartTime = 0;
            isPlayerMovingUp = true;
            isPlayerMoving = false;
            Player.transform.position = selectedTilePosition;
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

        bool isFront = false;
        switch ((int)cameraMgr.FinalRotationY) {
            case 0:
            case 90:
                isFront = true;
                break;
            case 180:
            case -90:
                isFront = false;
                break;
        }

        if (Math.Abs((int)cameraMgr.FinalRotationY) != 90) {
            bool tmp = Player.transform.position.x - selectedTilePosition.x > 0;

            if (!isFront) {
                lerpedPosition = new Vector3((tmp
                    ? 1
                    : -1), 0, tmp ? 0 : 2);
            }
            else {
                lerpedPosition = new Vector3((tmp
                                 ? 1
                                 : -1), 0, tmp ? -2 : 0);
            }
        }
        else {
            bool tmp = Player.transform.position.z - selectedTilePosition.z > 0;

            if (!isFront) {
                lerpedPosition = new Vector3(tmp ? 2 : 0, 0, (tmp
                                 ? 1
                                 : -1));
            }
            else {
                lerpedPosition = new Vector3(tmp ? 0 : -2, 0, (tmp
                                 ? 1
                                 : -1));
            }
            
        }

        GameObject.Find("ingame_accrue_move_count").GetComponent<GaugeMoveCount>().SetCountValue(m_playerMovingCount);

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


        GameObject tmpTile = null;

        //클릭한 타일 위에 다른 타일이 있을 경우 예외처리
        if (clickedTileId.y < mapLength.y - 1) {
            tmpTile = tileMgr.GetSurfaceTile(isPositionX, isFront, clickedTileId + new Vector3(0, 1, 0));
            if (tmpTile != null) {
                if (!tmpTile.GetComponent<TileObject>().IsCanPlayerIgnoreBlock())
                    return false;
            }
        }
        


        //플레이어 위
        if (heightLevel == 1) {
            Vector3 tempTile = Player.positionId + new Vector3(0, 2, 0);
            //플레이어 바로 위에 천장이 없을 경우
            if (tempTile.y > mapLength.y - 1 || tileMgr.GetSurfaceTile(isPositionX, isFront, tempTile) == null) {

                //플레이어가 벽에 막혀있는데 올라가려고 할 경우
                if (tempTile.y <= mapLength.y - 1) {
                    tmpTile = tileMgr.GetSurfaceTile(isPositionX, isFront, Player.positionId + new Vector3(0, 1, 0));
                    if (tmpTile != null) {
                        if (!tmpTile.GetComponent<TileObject>().IsCanPlayerIgnoreBlock())
                            return false;
                    }
                }
                

                //x축 기준 조건이 맞는 경우
                if (isPositionX && Math.Abs(clickedTileId.x - Player.positionId.x) <= 1) {
                    Player.animator.SetBool("isJump", true);
                    return true;
                }
                //z축 기준 조건이 맞는 경우
                else if (!isPositionX && Math.Abs(clickedTileId.z - Player.positionId.z) <= 1) {
                    Player.animator.SetBool("isJump", true);
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
                Player.animator.SetBool("isRun", true);
                return true;
            }

            gap = Math.Abs(clickedTileId.z - Player.positionId.z);
            //z축 기준 조건이 맞는 경우
            if (!isPositionX && gap <= 1 && gap > 0) {
                Player.animator.SetBool("isRun", true);
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

            //플레이어 옆이 제한 높이보다 높을 경우
            if (tempTile.y > mapLength.y - 1) {
                //x축 기준 조건이 맞는 경우
                if (isPositionX && Math.Abs(clickedTileId.x - Player.positionId.x) <= 1) {
                    Player.animator.SetBool("isLand", true);
                    return true;
                }
                //z축 기준 조건이 맞는 경우
                if (!isPositionX && Math.Abs(clickedTileId.z - Player.positionId.z) <= 1) {
                    Player.animator.SetBool("isLand", true);
                    return true;
                }
                //조건에 만족하지 않는 경우
                return false;
            }

            //플레이어 옆에 있는 장애물이 존재하지 않을 경우
            tmpTile = tileMgr.GetSurfaceTile(isPositionX, isFront, tempTile);
            if (tempTile.y > mapLength.y - 1 || tmpTile == null || tmpTile.GetComponent<TileObject>().IsCanPlayerIgnoreBlock()) {
                //x축 기준 조건이 맞는 경우
                if (isPositionX && Math.Abs(clickedTileId.x - Player.positionId.x) <= 1) {
                    Player.animator.SetBool("isLand", true);
                    return true;
                }
                //z축 기준 조건이 맞는 경우
                else if (!isPositionX && Math.Abs(clickedTileId.z - Player.positionId.z) <= 1) {
                    Player.animator.SetBool("isLand", true);
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
        prevPlayerStartTime += Time.deltaTime * 1.5f;

        bool run = Player.animator.GetBool("isRun");
        bool land = Player.animator.GetBool("isLand");
        bool jump = Player.animator.GetBool("isJump");
        bool isPositionX = Math.Abs((int)cameraMgr.FinalRotationY) != 90;


        Vector3 dir = Player.transform.position - selectedTilePosition;
        Quaternion lookRot = Quaternion.LookRotation(lerpedPosition);
        lookRot.x = 0; lookRot.z = 0;
        Player.transform.rotation = Quaternion.Slerp(Player.transform.rotation, lookRot, 0.1f);

        

        if (jump) {
            Player.transform.position = new Vector3(Mathf.Lerp(playerPos.x, selectedTilePosition.x, prevPlayerStartTime),
            Mathf.Lerp(playerPos.y, playerPos.y + ( -(1 / (9 * (prevPlayerStartTime + 0.073f))) + 1.1f) + Mathf.Sin(prevPlayerStartTime * Mathf.PI) * 0.1f, 1),
            Mathf.Lerp(playerPos.z, selectedTilePosition.z, prevPlayerStartTime));
        }else if (land) {
            Player.transform.position = new Vector3(Mathf.Lerp(playerPos.x, selectedTilePosition.x * Mathf.Sin(prevPlayerStartTime / 2 * Mathf.PI), prevPlayerStartTime),
            Mathf.Lerp(playerPos.y, selectedTilePosition.y + Mathf.Sin(prevPlayerStartTime * Mathf.PI) * 1.5f, prevPlayerStartTime),
            Mathf.Lerp(playerPos.z, selectedTilePosition.z * Mathf.Sin(prevPlayerStartTime / 2 * Mathf.PI), prevPlayerStartTime));
        }
        else if(run){
            Player.transform.position = new Vector3(Mathf.Lerp(playerPos.x, selectedTilePosition.x, prevPlayerStartTime),
            Mathf.Lerp(playerPos.y, selectedTilePosition.y, prevPlayerStartTime),
            Mathf.Lerp(playerPos.z, selectedTilePosition.z, prevPlayerStartTime));
        }
        

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
